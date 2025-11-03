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

#include "proc/handler.h"
#include <pthread.h>
#include <stdio.h>

static int __proc_running = false;

int
pcli_fork_init_msg(int pipe_fd[POSIX_MAX_READ_IN_OUT])
{
    return pipe(pipe_fd) != -1;
}

int
pcli_fork_msg_send(int fd, const void* buf, size_t sz)
{
    return write(fd, buf, sz) != -1;
}

int
pcli_fork_msg_read(int fd, void* buf, size_t sz)
{
    return read(fd, buf, sz) != -1;
}

void
pcli_fork_command(int* nproc, int* fd, int should_wait, const pcli_handler_callback callback)
{
    *nproc = fork();

    if (*nproc == -1)
    {
        perror("cannot spawn new process");
        return;
    }

    if (*nproc == 0)
    {
        callback(*nproc, fd);
    }

    if (*nproc > 0 && should_wait)
    {
        wait(NULL);
    }
}

int
pcli_fork_is_running(void)
{
    return __proc_running;
}
