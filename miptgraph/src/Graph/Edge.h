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
 * @file: Edge.h
 */
#ifndef GRAPH_EDGE_H
#define GRAPH_EDGE_H

/// Class for representation of edges
/**
 * An edge has two Nodes as its end points, pointer to graph that the edge relates to, and unique id.
 */
class Edge {
    pGraph m_graph; // graph where edge belongs to
    pNode m_from; // from node
    pNode m_to; // to node
    int m_id; // uniq number in this graph

protected:
    /// Ctor
    Edge(pNode from, pNode to);

    // flag is true if we alredy revert this edge for layout.
    bool reverse;

public:

    /**
     * Get reversed direction of edge
     * \return true if edge is reversed
     */
    bool IsReverse()
    {
        return reverse;
    }

    /**
     * Set bool edge has reversed direction
     */
    void SetReverse(bool is_reverse)
    {
        reverse = is_reverse;
    }

    /**
     * swap direction of edge
     */
    void Reverse();

    /**
     * Get pGraph pointer of graph where edge is
     * \return graph of edge
     */
    pGraph graph() { return m_graph; }

    /**
     * Get uniq edge number
     * \return int edge number
     */
    int id() { return m_id; }

    /**
     * Get pNode pointer to edge start node
     * \return from-node of edge
     */
    pNode from() { return m_from; }

    /**
     * Get pNode pointer to edge end node
     * \return to-node of edge
     */
    pNode to() { return m_to; }

    /**
     * Print info about the edge
     */
    virtual void Dump();

    /**
     * Print short info about the edge
     */
    virtual void Print();

    friend class Node;
    friend class Graph;
};

#endif

/* end. */
