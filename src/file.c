/*
 * This file is part of the PosixCLI distribution (https://github.com/MustafaMalikDev/posix-cli)
 * Copyright (c) 2024 Mustafa Malik (mm2440@kent.ac.uk).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "file.h"
#include "env.h"
#include "strutil.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/stat.h>

static DIR*
__pcli_opendir(const char* path)
{
    if (!path)
    {
        return NULL;
    }

    return opendir(path);
}

int
pcli_file_is_file(const char* path)
{
    if (path == NULL)
    {
        return false;
    }

    struct stat st;
    stat(path, &st);
    return S_ISREG(st.st_mode);
}

int
pcli_file_dir_exists(const char* path)
{
    DIR* dir = __pcli_opendir(path);
    if (dir)
    {
        closedir(dir);
        return true;
    }

    return false;
}

int
pcli_file_exists(const char* path)
{
    return access(path, F_OK) == 0;
}

char*
pcli_file_get_pwd(char* arr, size_t len, const char* cwd)
{
    memset(arr, 0, len);
    strcat(arr, "..");
    strcat(arr, cwd);
    arr[len] = '\0';

    char* pwd = dirname(arr);
    strremove(pwd, "..");

    memset(arr, 0, len);
    strcpy(arr, pwd);
    arr[strlen(pwd) + 1] = '\0';

    return arr;
}

int
pcli_file_dir_contains_dir(const char* path, const char* dirname)
{
    struct dirent* d;
    DIR*           dir = __pcli_opendir(path);

    if (!dir)
    {
        return false;
    }

    while ((d = readdir(dir)) != NULL)
    {
        if (streq(d->d_name, dirname))
        {
            closedir(dir);
            return true;
        }
    }

    return false;
}

void
pcli_file_list_dir(const char* path, int ignore_hidden)
{
    struct dirent* d;
    DIR*           dir = __pcli_opendir(path);

    if (!dir)
    {
        perror("could not open directory");
        return;
    }

    while ((d = readdir(dir)) != NULL)
    {
        if (*d->d_name == '.' && ignore_hidden)
        {
            continue;
        }

        printf("%s\n", d->d_name);
    }

    closedir(dir);
}

/* TODO: consider dirname() and basename() if time available */
pcli_file_path_t
pcli_file_resolve_path(const char* current, const char* path)
{
    if (!path || !current)
    {
        return (pcli_file_path_t) {.path = NULL, .heap = 0};
    }

    if (strstartswith("../", path) || strstartswith("..", path))
    {
        /* single parent */
        if (streq(path, "../") || streq(path, ".."))
        {
            char* buf = (char*) malloc(PATH_MAX);
            return (pcli_file_path_t) {.path = pcli_file_get_pwd(buf, PATH_MAX, current),
                                       .heap = 1};
        }

        char        cur_copy[PATH_MAX];
        char*       npath      = (char*) malloc(PATH_MAX);
        char*       copy       = (char*) path;
        const char* parent_tok = strtok(copy, "/");

        memset(cur_copy, 0, PATH_MAX);
        strcpy(cur_copy, current);

        pcli_file_get_pwd(npath, PATH_MAX, cur_copy);
        strcpy(cur_copy, npath);
        while (parent_tok)
        {
            parent_tok = strtok(NULL, "/");
            if (parent_tok)
            {
                pcli_file_get_pwd(npath, PATH_MAX, cur_copy);
                strcpy(cur_copy, npath);
            }
        }

        return (pcli_file_path_t) {.path = npath, .heap = 1};
    }
    else if (*path == '.')
    {
        const char* copy  = path;
        uint8_t     valid = false;

        while (*copy != '\0')
        {
            if (*copy == '.' || *copy == '/')
            {
                valid = true;
            }

            copy++;
        }

        return (pcli_file_path_t) {.path = (valid) ? current : NULL, .heap = 0};
    }

    if (*path == '~')
    {
        char* root = (char*) malloc(PATH_MAX);

        memset(root, 0, PATH_MAX);
        strcat(root, pcli_get_env("HOME"));
        strcat(root, "/");

        return (pcli_file_path_t) {.path = root, .heap = 1};
    }

    if (*path != '/')
    {
        char* cur_copy = (char*) malloc(PATH_MAX);
        memset(cur_copy, 0, PATH_MAX);
        strcpy(cur_copy, current);

        uint32_t fc  = false;
        uint32_t fs  = false;
        uint32_t idx = PATH_MAX - 1;

        while (!fc || !fs)
        {
            if (isalpha(cur_copy[idx]))
            {
                fc = true;
                break;
            }

            if (cur_copy[idx] == '/')
            {
                fs = true;
                break;
            }

            idx--;
        }

        if (!fs)
        {
            strcat(cur_copy, "/");
        }

        strcat(cur_copy, strtrim((char*) path));
        return (pcli_file_path_t) {.path = cur_copy, .heap = 1};
    }

    return (pcli_file_path_t) {.path = NULL, .heap = 0};
}