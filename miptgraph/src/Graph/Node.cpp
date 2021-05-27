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
 * @file: Node.cpp
 */

#include "predecls.h"
/* */
#include "Edge.h"
#include "Graph.h"
#include "Node.h"

/**
 * print node info with in/out edges
 */
void Node::Dump()
{
    list<pEdge>::iterator edge_iter;
    printf("Node id %d: (%d self-edges size (%d,%d))\n", m_id, nselfedges(), nxsize(), nysize());
    printf("  In  edges:");
    for (edge_iter = m_in_edges_list.begin();
         edge_iter != m_in_edges_list.end();
         edge_iter++) {
        pEdge pe = *edge_iter;
        if (pe) {
            printf(" ");
            pe->Print();
        }
    }

    printf("\n  Out edges:");

    for (edge_iter = m_out_edges_list.begin();
         edge_iter != m_out_edges_list.end();
         edge_iter++) {
        pEdge pe = *edge_iter;
        if (pe) {
            printf(" ");
            pe->Print();
        }
    }
    printf("\n");

    return;
}

/**
 * add node to graph
 */
Node::Node(pGraph graph)
{
    assert(graph);

    // node belongs to this graph
    m_graph = graph;

    // set node uniq number
    m_id = graph->next_node_id;

    // incoming edges to this node
    m_in_edges_list.clear();

    // outgoing edges from this node
    m_out_edges_list.clear();

    // number of self-edges at this node
    m_selfedges = 0;

    // x size
    m_nxsize_num = 30;

    // y size
    m_nysize_num = 20;

    // update graph node amount
    graph->m_total_nodes_num++;

    /// update next node number
    graph->next_node_id++;

    /// add node to graph
    graph->m_nodes_list.push_back(this);

    return;
}

/* end. */
