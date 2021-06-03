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
 */

/**
 * @file: cmiptgraph.h
 */

#ifdef __cplusplus
extern "C" {
#endif

/* return version number */
extern const char *cmipt_version (void);

/* new graph */
extern void *cmipt_newgraph (void);

/* delete graph */
extern void *cmipt_deletegraph (void *g);

/* new node */
extern void *cmipt_newnode (void *g);

/* run layout */
extern void cmipt_layout (void *g, int iter, int transpose, int iter2, int debug);

#ifdef __cplusplus
}
#endif

/* end. */
