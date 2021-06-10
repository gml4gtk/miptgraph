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
 * @file: LEdge.cpp
 */

#include "Layout.h"

/**
 * split longer edges across multiple ranks into
 * list of edges of one vertical rank
 */
void LEdge::BreakLongEdge()
{
    // Difference between rank of the "in" and "out"  nodes
    int n = ((pLNode)to())->Rank() - ((pLNode)from())->Rank();
    void* ud = NULL;

    // get original end points of this edge
    pLNode ofrom = (pLNode)from();
    pLNode oto = (pLNode)to();

    // id numbers are ofrom->id() and oto->id()

    if (n == 0) {
        // this is a horizontal edge
        return;
    }

    if (n < 0) {
        // this should not happen
        printf("fixme1 breaklongedge\n");
        return;
    }

    if (n == 1) {
        // this is already oke, nothing to split here
        return;
    }

    // start splitting at the head node
    pLNode prevnode = ((pLNode)from());
    pLNode newnode;
    ud = usrdata;

    // copy reversed edge status
    bool is_reverse = reverse;

    pEdge new_edge;

    // Adding n-1 nodes to do all rank differences equal 1
    for (int i = 1; i < n; i++) {
        // create dummy node for connection
        newnode = (pLNode)graph()->AddNode();
        // this is a dummy node
        newnode->dummy = true;
        // set original end points of edge where this dummynode belongs to
        newnode->origfrom = ofrom;
        newnode->origto = oto;
        // set size (1,1) for dummy node
        newnode->Setnxsize(1);
        newnode->Setnysize(1);
        // update count of dummy nodes in graph
        graph()->AddDummyNode();
        // set rank of dummy node
        newnode->rank = (((pLNode)from())->Rank()) + i;
        // add new edge to dummy node
        new_edge = graph()->AddEdge(prevnode, newnode, ud);
        // this is a split edge
        ((pLEdge)(new_edge))->SetComposite(true);
        // copy reversed status of orig edge
        new_edge->SetReverse(is_reverse);
        // indicate this is a inner edge
        if ((prevnode->dummy == true) && (newnode->dummy == true)) {
            ((pLEdge)(new_edge))->inner = true;
        } else {
            ((pLEdge)(new_edge))->inner = true;
        }
        // indicate at first edge this is head of a split edge
        if (n == 1) {
            ((pLEdge)(new_edge))->splitedgehead = true;
        } else {
            ((pLEdge)(new_edge))->splitedgehead = false;
        }
        // follow the chain
        prevnode = newnode;
    }

    // create last edge
    new_edge = graph()->AddEdge(prevnode, to(), ud);
    new_edge->SetReverse(is_reverse);
    ((pLEdge)new_edge)->SetComposite(true);
    // this is tail of a split edge
    ((pLEdge)(new_edge))->splitedgetail = true;

    // delete the old long edge
    graph()->DeleteEdge(from(), to());

    // this is a split edge
    composite = true;

    return;
}

/**
 * delete edge and its composite edges
 */
LEdge::~LEdge()
{
    return;
}

/* end. */
