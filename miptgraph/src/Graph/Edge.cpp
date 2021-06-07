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
 * @file: Edge.cpp
 */

#include "predecls.h"

/**
 * Create new edge between two pNode's
 * the edge gets uniq number
 * the edge is added to graph edgelist
 * the edge is added as outgoing edge of from node
 * the edge is added as incoming edge of to node
 * self-edge has always id number -a
 * self-edges are in own m_selfedges_list
 * self-edges are not in m_edges_list or at node in/out lists
 * edge can cross graphs which is not allowed or can be made a feature
 * \sa AddEdge
 */
Edge::Edge(pNode from, pNode to)
    : m_from(from)
    , m_to(to)
{
    assert(from);
    assert(to);

    // graph where edge is must be same in from/to node
    // todo this can turned into a feature
    assert(from->m_graph == to->m_graph);

    // edge is not reversed by default
    reverse = false;

    // get graph of edge based on from node
    pGraph pg = from->m_graph;

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

    return;
};

/**
 * reverse edge direction
 * set reverse to true
 * swap from/to node and in/out edges
 */
void Edge::Reverse()
{
    pNode temp_node;

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
void Edge::Dump()
{

    printf("Edge id %d: %d->%d reversed=%d horizontal=%d\n", m_id, m_from->m_id, m_to->m_id, reverse, hedge);

    return;
};

/**
 * Print short edge info
 */
void Edge::Print()
{

    printf("%d->%d", m_from->m_id, m_to->m_id);

    return;
};

/* end. */
