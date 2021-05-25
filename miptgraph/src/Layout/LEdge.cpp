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

    list<pLEdge>* edges_list = new list<pLEdge>;

    // start splitting at the head node
    pLNode prevnode = ((pLNode)from());
    pLNode newnode;

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
        // update count of dummy nodes in graph
        graph()->AddDummyNode();
        // set rank of dummy node
        newnode->rank = (((pLNode)from())->Rank()) + i;
        // add new edge to dummy node
        new_edge = graph()->AddEdge(prevnode, newnode);
        // this is a split edge
        ((pLEdge)(new_edge))->SetComposite(true);
        // copy reversed status of orig edge
        new_edge->SetReverse(is_reverse);
        // add to main edges list
        edges_list->push_back((pLEdge)new_edge);
        // follow the chain
        prevnode = newnode;
    }

    // create last edge
    new_edge = graph()->AddEdge(prevnode, to());
    new_edge->SetReverse(is_reverse);
    ((pLEdge)new_edge)->SetComposite(true);
    // add last split edge to min edge list
    edges_list->push_back((pLEdge)new_edge);

    // delete the old long edge
    graph()->DeleteEdge(from(), to());

    // set in every split edge the whole list of split edges
    for (list<pLEdge>::iterator edge_iter = edges_list->begin();
         edge_iter != edges_list->end();
         edge_iter++) {
        // set split edges list
        ((pLEdge)*edge_iter)->m_composite_edges = edges_list;
    }

    // this is a split edge
    composite = true;

    return;
}

/**
 * print composite edges of this edge
 */
void LEdge::CompositeEdgesDump()
{
    printf("Edge: %d. Composite Edges id numbers:\n", id());
    if (m_composite_edges != NULL) {
        for (list<pLEdge>::iterator edge_iter = m_composite_edges->begin();
             edge_iter != m_composite_edges->end();
             edge_iter++)
            printf("%d   ", (*edge_iter)->id());
    }
    printf("\n");

    return;
}

/**
 * delete edge and its composite edges
 */
LEdge::~LEdge()
{
    // if there are composite edges delete manual
    if (m_composite_edges != NULL) {
        list<pLEdge>* temp_list = m_composite_edges;
        for (list<pLEdge>::iterator edge_iter = temp_list->begin();
             edge_iter != temp_list->end();
             edge_iter++) {
            ((pLEdge)*edge_iter)->m_composite_edges = NULL;
        }
        delete temp_list;
    }

    return;
}

/* end. */
