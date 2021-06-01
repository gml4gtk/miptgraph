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
 * @file: mem.c
 */

/* all memory usage calls are redirected to here. */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

/**
 * malloc memory, exit if no mem
 */
void *do_malloc(size_t n)
{
	void *ret = NULL;
	if (n == 0) {
		return (NULL);
	}
	ret = calloc(1, n);
	if (ret == NULL) {
		exit(1);
	}
	return (ret);
}

/**
 * free mem and return (void *)0
 */
void *do_free(void *ptr)
{
	if (ptr) {
		free(ptr);
	}
	return (NULL);
}

/* end. */
