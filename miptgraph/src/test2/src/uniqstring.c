/** \file
 * \brief routines
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the root directory for details.
 *
 * \par
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 *
 * SPDX-License-Identifier: GPL-3.0+
 * License-Filename: LICENSE
 */

/**
 * @file: uniqstring.c
 */

#include "config.h"

#include <stdio.h>
#include <string.h>

#include "mem.h"
#include "uniqstring.h"

/**
 * make uniq copy of string
 */
char *uniqstr(char *str)
{
	char *s = NULL;
	if (str == NULL) {
		return ((char *)0);
	}
	if ((*str) == 0) {
		return ((char *)"");
	}
	s = do_malloc(strlen(str) + 1);

	if (s) {
		strcpy(s, str);
	}

	return s;
}

/* end. */
