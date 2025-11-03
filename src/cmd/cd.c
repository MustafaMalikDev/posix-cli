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

#include "cmd/cd.h"
#include "cmd/ls.h"
#include "file.h"
#include "strutil.h"

extern void
__pcli_ls_set(const char* newpath);

void
pcli_cd(const char* dir)
{
    if (!dir)
    {
        pcli_printerr("invalid path");
        return;
    }

    char args[PATH_MAX];
    strrmcmd(args, PATH_MAX, "cd", dir);

    if (streq(args, ""))
    {
        pcli_printerr("no arguments provided");
        return;
    }

    const char*      trimmed  = strtrim(args);
    pcli_file_path_t resolver = pcli_file_resolve_path(pcli_ls_get_path(), trimmed);

    if (!resolver.path)
    {
        pcli_printerr("No such file or directory");
        return;
    }

    if (!pcli_file_dir_exists(resolver.path))
    {
        perror("cannot change path");
        POSIX_FREE_RESOLVER(resolver)
    }

    __pcli_ls_set(resolver.path);
    POSIX_FREE_RESOLVER(resolver)
}
