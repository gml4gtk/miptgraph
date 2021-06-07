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

/* get number of real nodes in graph without dummy nodes */
extern int cmipt_graphnrealnodes(void* g);

/* get number of nodes in graph including dummy nodes */
extern int cmipt_graphnnodes(void* g);

/* get number of dummy nodes in graph */
extern int cmipt_graphndummynodes(void* g);

/* get number of edges in graph without self-edges */
extern int cmipt_graphnedges(void* g);

/* get number of horizontal edges in graph without self-edges */
extern int cmipt_graphnhoredges(void* g);

/* set number of horizontal edges in graph without self-edges */
extern void cmipt_graphsetnhoredges(void* g, int value);

/* get number of reversed edges in graph without self-edges */
extern int cmipt_graphnreversededges(void* g);

/* get number of self-edges in graph */
extern int cmipt_graphnselfedges(void* g);

/* get max node id in graph */
extern int cmipt_graphmaxnodeid(void* g);

/* get max edge id in graph */
extern int cmipt_graphmaxedgeid(void* g);

/* new node */
extern void *cmipt_newnode (void *g);

/* id of node */
extern int cmipt_nodeid (void *n);

/* xsize of node */
extern int cmipt_nodexsize (void *n);

/* ysize of node */
extern int cmipt_nodeysize (void *n);

/* set (x,y) size of node */
extern void cmipt_setnodesize (void *n, int xs, int ys);

/* dummy node status of node */
extern int cmipt_nodeisdummy (void *n);

/* relative x pos of node */
extern int cmipt_noderxpos (void *n);

/* relative y pos of node */
extern int cmipt_noderypos (void *n);

/* x pos of node */
extern int cmipt_nodexpos (void *n);

/* y pos of node */
extern int cmipt_nodeypos (void *n);

/* number of selfedges at node */
extern int cmipt_nodenselfedges (void *n);

/* find node */
extern void* cmipt_findnode(void* g, int num);

/* new edge */
extern void *cmipt_newedge (void *g, void *f, void *t, void *e);

/* find edge */
extern void* cmipt_findedge(void* g, int num);

/* get from node of edge */
extern int cmipt_edgefromnode (void *e);

/* get to node of edge */
extern int cmipt_edgetonode (void *e);

/* return if edge is horizontal */
extern int cmipt_edgeishor (void *e);

/* return if edge is reversed */
extern int cmipt_edgeisrev (void *e);

/* return if edge is split */
extern int cmipt_edgeissplit (void *e);

/* return usrdata of edge */
extern void *cmipt_edgeusrdata (void *e);

/* run layout */
extern void cmipt_layout (void *g, int iter, int transpose, int iter2, int debug, int usebary);

#ifdef __cplusplus
}
#endif

/* end. */
