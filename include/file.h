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

#ifndef _POSIX_CLI_FILE_H_
#define _POSIX_CLI_FILE_H_

#include "conf.h"

#define POSIX_FREE_RESOLVER(resolver)                                                              \
    if (resolver.heap)                                                                             \
    {                                                                                              \
        free((void*) resolver.path);                                                               \
    }

POSIX_BEGIN_DECLS

typedef struct
{
    const char* path;
    uint8_t     heap : 1;
} pcli_file_path_t;

int
pcli_file_is_file(const char*);

/**
 * @brief Checks if the given directory path exists
 *
 * @param path - the path to check
 * @return true if present, otherwise false
 */
int
pcli_file_dir_exists(const char*);

/**
 * @brief Checks if the given file path exists
 *
 * @param path - the path to check
 * @return true if present, otherwise false
 */
int
pcli_file_exists(const char*);

/**
 * @brief Gets the parent working directory of the current directory
 *
 * @param array - The array buffer to store the path in
 * @param size - The size of the array buffer
 * @param path - The current working directory path
 * @return The new path, the parent working directory
 */
char*
pcli_file_get_pwd(char*, size_t, const char*);

int
pcli_file_dir_contains_dir(const char*, const char*);

void
pcli_file_list_dir(const char*, int);

pcli_file_path_t
pcli_file_resolve_path(const char*, const char*);

POSIX_END_DECLS

#endif /* _POSIX_CLI_FILE_H_ */