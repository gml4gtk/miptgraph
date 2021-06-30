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
    int n = 0;
    void* ud = NULL;

    // get original end points of this edge
    pLNode ofrom = from();
    pLNode oto = to();

    n = (oto->Rank() - ofrom->Rank());

    // id numbers are ofrom->id() and oto->id()

    if (n == 0) {
        // this is a horizontal edge
        return;
    }

    if (n < 0) {
        // this should not happen
        printf("%s() delta < 0 should be >0 and is %d\n", __func__, n);
        return;
    }

    if (n == 1) {
        // this is already oke, nothing to split here
        return;
    }

    // start splitting at the head node
    pLNode prevnode = from();
    pLNode newnode;
    ud = usrdata;

    // copy reversed edge status
    bool is_reverse = reverse;

    pLEdge new_edge;

    // Adding n-1 nodes to do all rank differences equal 1
    for (int i = 1; i < n; i++) {
        // create dummy node for connection
        newnode = graph()->AddNode();
        // this is a dummy node
        newnode->dummy = true;
        // set original end points of edge where this dummynode belongs to
        newnode->origfrom = ofrom;
        newnode->origto = oto;
        // set size (1,1) for dummy node
        newnode->setNodeSize(1, 1);
        // update count of dummy nodes in graph
        graph()->AddDummyNode();
        // set rank of dummy node
        newnode->rank = (from()->Rank() + i);
        // add new edge to dummy node
        new_edge = graph()->AddEdge(prevnode, newnode, ud);
        // this is a split edge
        new_edge->SetComposite(true);
        // copy reversed status of orig edge
        new_edge->SetReverse(is_reverse);
        // indicate this is a inner edge
        if ((prevnode->dummy == true) && (newnode->dummy == true)) {
            new_edge->inner = true;
        } else {
            new_edge->inner = true;
        }
        // indicate at first edge this is head of a split edge
        if (n == 1) {
            new_edge->splitedgehead = true;
        } else {
            new_edge->splitedgehead = false;
        }
        // follow the chain
        prevnode = newnode;
    }

    // create last edge
    new_edge = graph()->AddEdge(prevnode, to(), ud);
    new_edge->SetReverse(is_reverse);
    new_edge->SetComposite(true);
    // this is tail of a split edge
    new_edge->splitedgetail = true;

    // delete the old long edge
    graph()->DeleteEdge(from(), to());

    return;
}

/**
 * delete edge and its composite edges
 */
LEdge::~LEdge()
{
    return;
}

/**
     * create edge with defaults
     */
LEdge::LEdge(pLNode from, pLNode to)
{
    assert(from);
    assert(to);

    // set from/to nodes
    m_from = from;
    m_to = to;

    if (from->id() == to->id()) {
        // this is a self-edge handled in edge creation
    }
    splitedgehead = false;
    splitedgetail = false;
    conflict = false;
    composite = false;
    reverse = false;
    hedge = false;
    iicross = 0;
    ircross = 0;
    rrcross = 0;
    inner = false;

    // graph where edge is must be same in from/to node
    // todo this can turned into a feature
    assert(from->m_graph == to->m_graph);

    // edge is not reversed by default
    reverse = false;

    // get graph of edge based on from node
    pLGraph pg = from->m_graph;

    // graph must be defined
    assert(pg);

    // set graph of edge
    m_graph = pg;

    if (from->id() == to->id()) {
        // this is a self-edge and not in the edges list

        // self edges do not need uniq id
        m_id = -1;

        // update number of edges in this graph
        pg->m_total_selfedges_num++;

        // push on selfedges list
        pg->m_selfedges_list.push_back(this);

        // incr. number of self-edges at this node
        from->m_selfedges++;

    } else {
        // regular edge

        // give edge uniq number in this graph
        m_id = pg->next_edge_id;

        // update edge id counter
        pg->next_edge_id++;

        // update number of edges in this graph
        pg->m_total_edges_num++;

        // push on edges list
        pg->m_edges_list.push_back(this);

        // add edge to outgoing edges of from node
        from->m_out_edges_list.push_back(this);

        // add edge to incoming edges of to node
        to->m_in_edges_list.push_back(this);
    }
}

/**
 * reverse edge direction
 * set reverse to true
 * swap from/to node and in/out edges
 */
void LEdge::Reverse()
{
    pLNode temp_node;

    // Swap from<->to nodes for the edges;
    temp_node = m_to;
    m_to = m_from;
    m_from = temp_node;

    // Swap in<->out edges for the nodes;
    m_to->m_in_edges_list.push_back(this);
    m_from->m_out_edges_list.push_back(this);

    // todo. after this out edges are deleted. oke?
    m_to->m_out_edges_list.remove(this);
    m_from->m_in_edges_list.remove(this);

    // edge is reversed
    reverse = true;

    return;
}

/**
 * Print edge info
 */
void LEdge::Dump()
{

    printf("Edge id %d: %d->%d reversed=%d horizontal=%d\n", m_id, m_from->m_id, m_to->m_id, reverse, hedge);

    return;
};

/**
 * Print short edge info
 */
void LEdge::Print()
{
    pLNode fn;
    pLNode tn;

    fn = from();
    tn = to();

    if (fn && tn) {

        printf("%d->%d", fn->id(), tn->id());
    } else {
        printf("fn=%p tn=%p ", (void*)fn, (void*)tn);
    }

    return;
};

/* end. */
