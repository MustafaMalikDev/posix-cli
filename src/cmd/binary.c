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

#include "cmd/binary.h"
#include "file.h"
#include "strutil.h"

/*  basic edge cases have been handled,
    main goal is to implement forks,
    process management and piping for
    IPC understanding and demonstrating.
    Did not handle string literals fully
    for the same reason
 */

static char    __buffer[4096];
static ssize_t __rs = -1;
static int     __pfd[2];
static int     __pipeid = -1;

int
pcli_binary_find(const char* cmd, int piped, int readstdin)
{
    if (!cmd)
    {
        return false;
    }

    if (streq(cmd, "") || strlen(cmd) == 1)
    {
        return false;
    }

    char*   args[255];
    char*   copy  = (char*) cmd;
    char*   token = strtok(copy, " ");
    uint8_t idx   = 1;

    memset(args, 0, 255);
    args[0] = copy;

    while (token && idx < 255)
    {
        if (strcmp(token, copy) != 0)
        {
            args[idx] = token;
            idx++;
        }
        token = strtok(NULL, " ");
    }

    if (piped)
    {
        __pipeid = pipe(__pfd);
        if (__pipeid == -1)
        {
            perror("failed to make pipe");
            return false;
        }
    }

    int chproc = fork();
    if (chproc == -1)
    {
        pcli_printerr("failed to spawn new process");
        return false;
    }

    if (chproc == 0)
    {
        if (piped)
        {
            dup2(__pfd[1], STDOUT_FILENO);
            if (readstdin)
            {
                dup2(__pfd[0], STDIN_FILENO);
                close(__pfd[1]);
            }
            else
            {
                close(__pfd[0]);
            }
        }

        int eres = execvp(copy, args);
        if (eres == -1)
        {
            perror("failed to run executable");
            return false;
        }
    }
    if (chproc > 0)
    {
        wait(NULL);
        return true;
    }

    return false;
}

pcli_binary_t
pcli_binary_is_piped(const char* cmd)
{
    if (!cmd)
    {
        return (pcli_binary_t) {.arg_count = 0, .pipe_count = 0, .status = false};
    }

    if (*cmd == '|' || cmd[strlen(cmd - 1)] == '|')
    {
        return (pcli_binary_t) {.arg_count = 0, .pipe_count = 0, .status = false};
    }

    if (streq(cmd, "") || strlen(cmd) == 1)
    {
        return (pcli_binary_t) {.arg_count = 0, .pipe_count = 0, .status = false};
    }

    memset(__buffer, 0, sizeof(__buffer));

    int  cmd_len = (int) strlen(cmd) + 1;
    char pipe_split[cmd_len];
    char arg_split[cmd_len];

    pipe_split[cmd_len - 1] = '\0';
    arg_split[cmd_len - 1]  = '\0';

    strcpy(pipe_split, cmd);
    strcpy(arg_split, cmd);

    char* arg_tok = strtok(arg_split, " ");

    uint8_t pipe_count = 0;
    uint8_t arg_count  = 0;
    uint8_t idx        = 0;

    while (idx < cmd_len)
    {
        if (pipe_split[idx] == '|')
        {
            pipe_count++;
        }
        idx++;
    }

    while (arg_tok)
    {
        if (strlen(arg_tok) != 1 && *arg_tok != '|')
        {
            arg_count++;
        }
        arg_tok = strtok(NULL, " ");
    }

    return (pcli_binary_t) {.arg_count  = arg_count,
                            .pipe_count = pipe_count,
                            .status =
                                (arg_count > 0 && pipe_count > 0) && (arg_count > pipe_count)};
}

void
pcli_binary_exec_pipe(const char* cmd, const pcli_binary_t pipe_data)
{
    if (!cmd)
    {
        pcli_printerr("invalid commands/arguments");
        return;
    }

    if (streq(cmd, "") || strlen(cmd) == 1 || !pipe_data.status)
    {
        pcli_printerr("no commands/arguments, pipe failed");
        return;
    }

    size_t cmd_len = strlen(cmd) + 1;
    char   cmd_copy[cmd_len];

    int   arg_count = pipe_data.arg_count + 1;
    int   idx       = 0;
    char* commands[arg_count];

    strcpy(cmd_copy, cmd);
    cmd_copy[cmd_len - 1]   = '\0';
    commands[arg_count - 1] = NULL;

    char* token = strtok(cmd_copy, "|");

    while (token)
    {
        if (token)
        {
            token = strtrim(token);
            if (idx < arg_count)
            {
                commands[idx] = token;
            }

            idx++;
        }
        token = strtok(NULL, "|");
    }

    for (int i = 0; i < arg_count; i++)
    {
        int bin_res = pcli_binary_find(commands[i], true, i > 0 ? true : false);
        if (!bin_res)
        {
            pcli_printerr("failed to run piped operations");
            break;
        }

        if (__buffer[0] == '\0')
        {
            break;
        }
    }

    __rs = read(__pfd[0], __buffer, sizeof(__buffer));
    printf("%s\n", __buffer);

    if (__pipeid > 0)
    {
        close(__pfd[0]);
        close(__pfd[1]);
    }
}
