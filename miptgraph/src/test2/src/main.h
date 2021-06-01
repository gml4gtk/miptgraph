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
 * @file: main.h
 */

#ifndef MAIN_H
#define MAIN_H 1

struct usrgraph;
struct usrnode;
struct usredge;

/* */
struct usrgraph {
	struct usrnode *rawnodelist;
	struct usrnode *rawnodelistend;
	struct usredge *rawedgelist;
	struct usredge *rawedgelistend;
};

/* */
struct usrnode {
	int gmlid;		/* id number from gml data */
	int lid;		/* id number from layouter */
};

/* */
struct usredge {
	int fromgmlid;		/* id number from gml data */
	int fromlid;		/* id number from layouter */
	int togmlid;		/* id number from gml data */
	int tolid;		/* id number from layouter */
};

/* in gml.peg */
extern int gmlparse(struct usrgraph *g, FILE * f, char *name, int debug);

#endif

/* end. */
