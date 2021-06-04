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
struct drawnode;
struct drawedge;
struct usrxy;

struct usrxy {
	int x;
	int y;
};

/* */
struct usrgraph {
	struct usrnode *rawnodelist;
	struct usrnode *rawnodelistend;
	struct usredge *rawedgelist;
	struct usredge *rawedgelistend;
	struct drawnode *drawnodelist;
	struct drawnode *drawnodelistend;
	struct drawedge *drawedgelist;
	struct drawedge *drawedgelistend;
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
	void *ln;		/* */
	int xsize;		/* */
	int ysize;		/* */
	int txsize;		/* */
	int tysize;		/* */
	struct usrnode *next;	/* linkage */
};

/* */
struct usredge {
	int fromgmlid;		/* id number from gml data */
	int togmlid;		/* id number from gml data */
	char *elabel;		/* opt. edge label */
	char *fill;		/* edge line color as #rrggbb */
	int fillcolor;		/* edge line color as rgb */
	void *le;		/* */
	struct usredge *next;	/* linkage */
};

/* */
struct drawnode {
	int lid;		/* layouter id of node */
	int rxpos;		/* relative x pos */
	int rypos;		/* relative y pos */
	int xpos;		/* x pos */
	int ypos;		/* y pos */
	int dummy;		/* set if dummy node */
	int selfedges;		/* number of self edges at node */
	struct usrnode *un;	/* opt. other node data */
	struct drawnode *next;	/* linkage */
};

/* */
struct drawedge {
	int lid;		/* layouter id of edge */
	int hor;		/* 1 if edge is horizontal */
	int rev;		/* 1 if edge is reversed */
	int split;		/* 1 if edge is split */
	struct drawnode *fn;	/* from node */
	struct drawnode *tn;	/* to node */
	struct drawedge *next;	/* linkage */
};

/* in gml.peg */
extern int gmlparse(struct usrgraph *g, FILE * f, char *name, int debug);

#endif

/* end. */
