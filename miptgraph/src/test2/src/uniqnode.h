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
 * @file: uniqnode.h
 */

#ifndef UNIQNODE_H
#define UNIQNODE_H 1

extern struct usrnode *uniqnode_gid(int nr);

extern void uniqnode_gid_add(struct usrnode *n);

extern struct usrnode *uniqnode_lid(int nr);

extern void uniqnode_lid_add(struct usrnode *n);

extern struct drawnode *uniqdrawnode_lid(int nr);

extern void uniqdrawnode_lid_add(struct drawnode *n);

extern void uniqnode_clear_gid(void);

extern void uniqnode_clear_lid(void);

extern void uniqdrawnode_clear_lid(void);

#endif

/* end. */
