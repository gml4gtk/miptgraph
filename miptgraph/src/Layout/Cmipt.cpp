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
 * @file: Cmipt.cpp
 */

#include "../cmiptgraph.h"
#include "Layout.h"
#include "config.h"

/* return lib version */
extern "C" const char* cmipt_version(void)
{
    return (PACKAGE_VERSION);
}

/* create new graph */
extern "C" void* cmipt_newgraph(void)
{
    pLGraph pg = new LGraph;
    return ((void*)pg);
}

/* delete graph */
extern "C" void* cmipt_deletegraph(void* g)
{
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        delete pg;
    }
    return (NULL);
}

/* get number of real nodes in graph without dummy nodes */
extern "C" int cmipt_graphnrealnodes(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->nrealnodes();
    }
    return (ret);
}

/* get number of nodes in graph including dummy nodes */
extern "C" int cmipt_graphnnodes(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->nnodes();
    }
    return (ret);
}

/* get number of dummy nodes in graph */
extern "C" int cmipt_graphndummynodes(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->ndummynodes();
    }
    return (ret);
}

/* get number of edges in graph without self-edges */
extern "C" int cmipt_graphnedges(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->nedges();
    }
    return (ret);
}

/* get number of horizontal edges in graph without self-edges */
extern "C" int cmipt_graphnhoredges(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->nhoredges();
    }
    return (ret);
}

/* set number of horizontal edges in graph without self-edges */
extern "C" void cmipt_graphsetnhoredges(void* g, int value)
{
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        if (value >= 0) {
            pg->Set_Nhoredges(value);
        }
    }
    return;
}

/* get number of reversed edges in graph without self-edges */
extern "C" int cmipt_graphnreversededges(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->nreversededges();
    }
    return (ret);
}

/* get number of self-edges in graph */
extern "C" int cmipt_graphnselfedges(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->nselfedges();
    }
    return (ret);
}

/* get max node id in graph */
extern "C" int cmipt_graphmaxnodeid(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->maxnodeid();
    }
    return (ret);
}

/* get max edge id in graph */
extern "C" int cmipt_graphmaxedgeid(void* g)
{
    int ret = -1;
    pLGraph pg = NULL;
    if (g) {
        pg = (pLGraph)g;
        ret = pg->maxedgeid();
    }
    return (ret);
}

/* create new node */
extern "C" void* cmipt_newnode(void* g)
{
    pLNode p = NULL;
    pLGraph pg = NULL;
    pg = (pLGraph)g;
    if (pg) {
        p = pg->AddNode();
    }
    return ((void*)p);
}

/* id of node */
extern "C" int cmipt_nodeid(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = p->id();
    }
    return (ret);
}

/* xsize of node */
extern "C" int cmipt_nodexsize(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = p->getxsize();
    }
    return (ret);
}

/* ysize of node */
extern "C" int cmipt_nodeysize(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = p->getysize();
    }
    return (ret);
}

/* set (x,y) size of node */
extern "C" void cmipt_setnodesize(void* n, int xs, int ys)
{
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        if (xs >= 0 && ys >= 0) {
            p->setNodeSize(xs, ys);
        }
    }
    return;
}

/* dummy node status of node */
extern "C" int cmipt_nodeisdummy(void* n)
{
    int ret = -1;
    bool status = false;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        status = p->IsDummy();
        if (status == true) {
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return (ret);
}

/* relative x pos of node */
extern "C" int cmipt_noderxpos(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = p->getPos();
    }
    return (ret);
}

/* relative y pos of node */
extern "C" int cmipt_noderypos(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = p->Rank();
    }
    return (ret);
}

/* x pos of node */
extern "C" int cmipt_nodexpos(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = (int)p->getX();
    }
    return (ret);
}

/* y pos of node */
extern "C" int cmipt_nodeypos(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = (int)p->getY();
    }
    return (ret);
}

/* number of selfedges at node */
extern "C" int cmipt_nodenselfedges(void* n)
{
    int ret = -1;
    pLNode p = NULL;
    if (n) {
        p = (pLNode)n;
        ret = p->nselfedges();
    }
    return (ret);
}

/* find node */
extern "C" void* cmipt_findnode(void* g, int num)
{
    pLNode p = NULL;
    pLGraph pg = NULL;
    pg = (pLGraph)g;
    if (pg) {
        p = pg->FindNode(num);
    }
    return ((void*)p);
}

/* new edge */
extern "C" void* cmipt_newedge(void* g, void* f, void* t, void* usrdata)
{
    void* ret = NULL;
    pLGraph pg = NULL;
    pLNode pf = NULL;
    pLNode pt = NULL;
    pLEdge e = NULL;
    if (g && f && t) {
        pg = (pLGraph)g;
        pf = (pLNode)f;
        pt = (pLNode)t;
        e = pg->AddEdge(pf, pt, usrdata);
        ret = (void*)e;
    }
    return (ret);
}

/* find edge */
extern "C" void* cmipt_findedge(void* g, int num)
{
    pLEdge p = NULL;
    pLGraph pg = NULL;
    pg = (pLGraph)g;
    if (pg) {
        p = pg->FindEdge(num);
    }
    return ((void*)p);
}

/* get from node of edge */
extern "C" int cmipt_edgefromnode(void* e)
{
    pLEdge p = NULL;
    int ret = -1;
    if (e) {
        p = (pLEdge)e;
        ret = p->from()->id();
    }
    return (ret);
}

/* get to node of edge */
extern "C" int cmipt_edgetonode(void* e)
{
    pLEdge p = NULL;
    int ret = -1;
    if (e) {
        p = (pLEdge)e;
        ret = p->to()->id();
    }
    return (ret);
}

/* return if edge is horizontal */
extern "C" int cmipt_edgeishor(void* e)
{
    pLEdge p = NULL;
    bool status = false;
    int ret = -1;
    if (e) {
        p = (pLEdge)e;
        status = p->IsHedge();
        if (status == true) {
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return (ret);
}

/* return if edge is reversed */
extern "C" int cmipt_edgeisrev(void* e)
{
    pLEdge p = NULL;
    bool status = false;
    int ret = -1;
    if (e) {
        p = (pLEdge)e;
        status = p->IsReverse();
        if (status == true) {
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return (ret);
}

/* return if edge is split */
extern "C" int cmipt_edgeissplit(void* e)
{
    pLEdge p = NULL;
    bool status = false;
    int ret = -1;
    if (e) {
        p = (pLEdge)e;
        status = p->Composite();
        if (status == true) {
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return (ret);
}

/* return usrdata of edge or NULL */
extern "C" void* cmipt_edgeusrdata(void* e)
{
    pLEdge p = NULL;
    void* ret = NULL;
    if (e) {
        p = (pLEdge)e;
        ret = p->usrdata;
    }
    return (ret);
}

/* run layout */
extern "C" void cmipt_layout(void* g, int iter, int transpose, int iter2, int debug, int bary)
{
    bool dotr = false;
    bool dodb = false;
    int itera = 0;
    int iterb = 0;
    pLGraph pg = NULL;
    pg = (pLGraph)g;
    if (iter > 0) {
        itera = iter;
    } else {
        itera = 100;
    }
    if (iter2 > 0) {
        iterb = iter2;
    } else {
        iterb = 100;
    }
    if (transpose > 0) {
        dotr = true;
    } else {
        dotr = false;
    }
    if (debug > 0) {
        dodb = true;
    } else {
        dodb = false;
    }
    pg->Layout(itera, dotr, iterb, dodb);
    return;
}

/* end. */
