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

#ifndef _POSIX_CLI_CONF_H_
#define _POSIX_CLI_CONF_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef POSIX_CPP_ONLY

#define POSIX_HOSTNAME_MAX_LEN 255
#define true 1
#define false 0
#define pcli_printerr(msg) printf("* %s\n", msg)

// clang-format off
#if defined( __unix__ ) || defined( __unix ) || defined( unix )
#   define POSIX_UNIX_API
#elif defined( __APPLE__ )
#   include <mach-o/dyld.h>
#   include <sys/syslimits.h>
#   define POSIX_APPLE_API
#endif

#endif

#if __cplusplus
#   define POSIX_BEGIN_DECLS extern "C" {
#   define POSIX_END_DECLS }
#else
#   define POSIX_BEGIN_DECLS
#   define POSIX_END_DECLS
#endif
// clang-format on

#endif /* _POSIX_CLI_CONF_H_ */