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

#ifndef _POSIX_CLI_COMMANDS_H_
#define _POSIX_CLI_COMMANDS_H_

#include "conf.h"
#include "strutil.h"

POSIX_BEGIN_DECLS

static const char* __posix_cli_commands__[] = {"ls", "cd", "exit", "clear", 0};

uint8_t
pcli_cmd_count(void);
uint8_t
pcli_cmd_is_valid(const char*);

#ifdef POSIX_COMMAND_IMPLEMENTATION

uint8_t
pcli_cmd_count(void)
{
    uint8_t count = 0;
    for (;;)
    {
        if (__posix_cli_commands__[count] == 0)
        {
            break;
        }
        count++;
    }
    return count;
}

uint8_t
pcli_cmd_is_valid(const char* command)
{
    uint8_t count = pcli_cmd_count();
    for (int i = 0; i < count; i++)
    {
        if (strstartswith(command, __posix_cli_commands__[i]))
        {
            return true;
        }
    }
    return false;
}

#endif /* POSIX_COMMAND_IMPLEMENTATION */

POSIX_END_DECLS

#endif /* _POSIX_CLI_COMMANDS_H_ */
