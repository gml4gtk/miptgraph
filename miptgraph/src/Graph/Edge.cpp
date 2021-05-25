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
 * @file: Edge.cpp
 */

#include "predecls.h"

/**
 * Create new edge between two pNode's
 * the edge gets uniq number
 * the edge is added to graph edgelist
 * the edge is added as outgoing edge of from node
 * the edge is added as incoming edge of to node
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

    // give edge uniq number in this graph
    m_id = pg->next_edge_id;

    // update edge id counter
    pg->next_edge_id++;

    // set graph of edge
    m_graph = pg;

    if (from->id() == to->id()) {
        // this is a self-edge and not in the edges list

        // update number of edges in this graph
        pg->m_total_selfedges_num++;

        // push on selfedges list
        pg->m_selfedges_list.push_back(this);

        // incr. number of self-edges at this node
        from->m_selfedges++;

    } else {
        // regular edge

        // update number of edges in this graph
        pg->m_total_edges_num++;

        // push on edges list
        pg->m_edges_list.push_back(this);

        // add edge to outgoing edges ot from node
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

    printf("Edge id %d: %d->%d rev=%d\n", m_id, m_from->m_id, m_to->m_id, reverse);

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
