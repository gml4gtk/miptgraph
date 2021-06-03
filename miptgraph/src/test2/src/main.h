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
	void *lg;
};

/* */
struct usrnode {
	int gmlid;		/* id number from gml data */
	char *gmlidstr;		/* as string */
	int lid;		/* id number from layouter */
	char *nlabel;		/* optional node label */
	char *fill;		/* node fill color as #rrggbb */
	int fillcolor;		/* rgb node fill color */
	char *outline;		/* node outline color as #rrggbb */
	int outlinecolor;	/* rgb node outline color */
	struct usrnode *next;	/* linkage */
};

/* */
struct usredge {
	int fromgmlid;		/* id number from gml data */
	int fromlid;		/* id number from layouter */
	int togmlid;		/* id number from gml data */
	int tolid;		/* id number from layouter */
	char *elabel;		/* opt. edge label */
	char *fill;		/* edge line color as #rrggbb */
	int fillcolor;		/* edge line color as rgb */
	struct usredge *next;	/* linkage */
};

/* in gml.peg */
extern int gmlparse(struct usrgraph *g, FILE * f, char *name, int debug);

#endif

/* end. */
