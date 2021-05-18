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
 * @file: Node.h
 */
#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

/// Class for representation of nodes
/**
 * A node has two lists of incoming to it and outgoing from it edges,
 * pointer to the graph that the node relates to, and unique id.
 */
class Node {
    pGraph m_graph;
    list<pEdge> m_in_edges_list;
    list<pEdge> m_out_edges_list;
    int m_id;

protected:
    Node(pGraph graph);

public:
    /// Get node id
    int id() { return m_id; }

    list<pEdge>* in_edges_list()
    {
        return &m_in_edges_list;
    }

    list<pEdge>* out_edges_list()
    {
        return &m_out_edges_list;
    }

    /// Print info about the node
    virtual void Dump();
    friend class Edge;
    friend class Graph;
};
#endif
