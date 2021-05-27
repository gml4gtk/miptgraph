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
 * @file: Node.h
 */
#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

/**
 * Class for representation of nodes
 * A node has two lists of incoming to it and outgoing from it edges,
 * pointer to the graph that the node relates to, and unique id.
 */
class Node {

    /**
     * node belong to this graph
     */
    pGraph m_graph;

    /**
     * incoming edges to this node
     */
    list<pEdge> m_in_edges_list;

    /**
     * outgoing edges from this node
     */
    list<pEdge> m_out_edges_list;

    /**
     * number of self-edges at this node
     */
    int m_selfedges;

    /**
     * uniq node number
     */
    int m_id;

    /**
     * x size
     */
    int m_nxsize_num;

    /**
     * y size
     */
    int m_nysize_num;

protected:
    Node(pGraph graph);

public:

    /**
     * Get uniq node id
     */
    int id() { return m_id; }

    /**
     * get nuber of self-edgs at this node
     */
    int nselfedges () { return m_selfedges; }

    /**
     * get x size
     */
    int nxsize() { return m_nxsize_num; }

    /**
     * set x size
     */
    void Setnxsize(int value) { m_nxsize_num = value; }

    /**
     * get y size
     */
    int nysize() { return m_nysize_num; }

    /**
     * set y size
     */
    void Setnysize(int value) { m_nysize_num = value; }

    /**
     * Get incoming edges
     */
    list<pEdge>* in_edges_list()
    {
        return &m_in_edges_list;
    }

    /**
     * Get outgoing edges
     */
    list<pEdge>* out_edges_list()
    {
        return &m_out_edges_list;
    }

    /**
     * Print info about the node
     */
    virtual void Dump();

    friend class Edge;
    friend class Graph;
};
#endif

/* end. */
