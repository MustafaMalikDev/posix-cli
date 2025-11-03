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

#ifndef _POSIX_CLI_ENV_H_
#define _POSIX_CLI_ENV_H_

#define POSIX_CPP_ONLY
#include "conf.h"

POSIX_BEGIN_DECLS

const char*
pcli_get_env(const char*);

POSIX_END_DECLS

#endif /* _POSIX_CLI_ENV_H_ */
