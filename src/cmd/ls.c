/**
 * This file is part of the PosixCLI distribution (https://github.com/MustafaMalikDev/posix-cli)
 * Copyright (c) 2025 Mustafa Malik (mm2440@kent.ac.uk).
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

#include "cmd/ls.h"
#include "env.h"
#include "file.h"
#include "strutil.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static char __path[PATH_MAX];
static char __hostname[POSIX_HOSTNAME_MAX_LEN];

void
__pcli_ls_set(const char* newpath)
{
    if (!newpath)
    {
        pcli_printerr("invalid path");
        return;
    }

    memset(__path, 0, PATH_MAX);
    strcpy(__path, newpath);
}

void
__pcli_ls_init(void)
{
    memset(__hostname, 0, POSIX_HOSTNAME_MAX_LEN);
    memset(__path, 0, PATH_MAX);
    strcpy(__path, pcli_get_env("HOME"));

    int result = gethostname(__hostname, POSIX_HOSTNAME_MAX_LEN);
    if (result == -1)
    {
        perror("cannot get hostname");
        exit(EXIT_FAILURE);
    }
}

void
__pcli_ls_new_line(void)
{
    char tmp[PATH_MAX];
    strcpy(tmp, __path);

    char* last_dir = strrchr(tmp, '/');
    if (!last_dir)
    {
        last_dir = "/???";
    }

    printf("%s %s %s ", strremove(__hostname, ".local"), strremove(last_dir, "/"), "%");
}

void
pcli_ls_show(const char* dir)
{
    if (!dir)
    {
        pcli_printerr("invalid path");
        return;
    }

    size_t len = strlen(dir) + 1;
    char   args[len];
    strrmcmd(args, len, "ls", dir);

    if (streq(args, ""))
    {
        pcli_file_list_dir(__path, true);
        return;
    }

    const char*      trimmed  = strtrim(args);
    pcli_file_path_t resolver = pcli_file_resolve_path(__path, trimmed);

    if (!resolver.path)
    {
        pcli_printerr("No such file or directory");
        return;
    }

    if (pcli_file_is_file(resolver.path))
    {
        printf("%s\n", resolver.path);
        goto done;
    }

    if (pcli_file_dir_exists(resolver.path))
    {
        pcli_file_list_dir(resolver.path, true);
        goto done;
    }

done:
    POSIX_FREE_RESOLVER(resolver)
}

const char*
pcli_ls_get_path(void)
{
    return __path;
}
