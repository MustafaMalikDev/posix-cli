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

#define POSIX_COMMAND_IMPLEMENTATION

#include <limits.h>
#include <signal.h>
#include <stdio.h>

#include "cmd/binary.h"
#include "cmd/cd.h"
#include "cmd/ls.h"
#include "commands.h"
#include "strutil.h"

static void
__safe_terminate(int __attribute__((unused)) _)
{
    /* 0 = all processes in parent process group */
    kill(0, SIGKILL);
}

int
main(void)
{
    signal(SIGINT, __safe_terminate);
    __pcli_ls_init();

    printf("-----------------------------------------------------------------\n");
    printf("%s\n%s\n%s\n",
           "| posix-cli - toy re-implementation of the UNIX Shell.          |",
           "| Copyright (c) 2024-Present, Mustafa Malik (mm2440@kent.ac.uk).|",
           "| Licensed under GNU GPL v3.                                    |");
    printf("-----------------------------------------------------------------\n");

    char cmd_buf[ARG_MAX];
    int  terminate = false;

    while (!terminate)
    {
        __pcli_ls_new_line();
        fgets(cmd_buf, ARG_MAX, stdin);
        strtrim(cmd_buf);

        /* terminal specific commands */
        if (streq(cmd_buf, "exit"))
        {
            printf("terminating session...\n");
            terminate = true;
            __safe_terminate(SIGKILL);
        }

        else if (streq(cmd_buf, "clear"))
        {
            /* \033[H move cursor to home position, \033[J erase lines below */
            printf("\033[H\033[J");
        }

        /* path related */
        else if (strstartswith("ls", cmd_buf))
        {
            pcli_ls_show(cmd_buf);
        }

        else if (strstartswith("cd", cmd_buf))
        {
            pcli_cd(cmd_buf);
        }

        else if (!pcli_cmd_is_valid(cmd_buf))
        {
            pcli_binary_t bin;
            if ((bin = pcli_binary_is_piped(cmd_buf)).status)
            {
                pcli_binary_exec_pipe(cmd_buf, bin);
            }
            else if (!pcli_binary_find(cmd_buf, false, false))
            {
                pcli_printerr("invalid command");
                continue;
            }
        }
    }

    return EXIT_SUCCESS;
}
