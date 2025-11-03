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

#ifndef _POSIX_CLI_HANDLER_H_
#define _POSIX_CLI_HANDLER_H_

/* responsible for managing child processes of new commands */
#include "conf.h"
#include "function.h"

#define POSIX_MAX_READ_IN_OUT 2

POSIX_BEGIN_DECLS

/* https://stackoverflow.com/questions/7292642/grabbing-output-from-exec */

int
pcli_fork_init_msg(int[POSIX_MAX_READ_IN_OUT]);

int
pcli_fork_msg_send(int, const void*, size_t);

int
pcli_fork_msg_read(int, void*, size_t);

void
pcli_fork_command(int*, int*, int, const pcli_handler_callback);

int
pcli_fork_is_running(void);

POSIX_END_DECLS

#endif /* _POSIX_CLI_HANDLER_H_ */
