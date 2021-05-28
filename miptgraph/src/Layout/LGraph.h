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
 * @file: LGraph.h
 */

#ifndef LAYOUT_GRAPH_H
#define LAYOUT_GRAPH_H

/**
 * Graph with Layout methods
 * Includes ranking, median and transposition heuristic
 */

class LGraph : public Graph {
private:

    /**
     * max y level, number of levels is (maxrank+1)
     */
    unsigned int maxrank;

    /**
     * if >0 graph is already layouted
     */
    unsigned int layouted;

    /**
     * number of starter nodes
     */
    unsigned int m_nstarter_num;

    /**
     * horizontal node ordering
     */
    Ordering* order;

public:

    /**
     * create graph
     */
    LGraph()
    {
        maxrank = 0;
	m_nstarter_num = 0;
	layouted = false;
        order = NULL;
    }

    /**
     * Replacing revert edges from the ReverseEdges list.
     */
    void ReverseReverseEdges(list<pEdge>& ReverseEdges);

    /**
     * get number of vertical y levels
     */
    int getMaxrank() { return maxrank; }

    /**
     * This main Layout function do whole procedure of LAYOUT:
     * 1. Reversing "Reverse" edges
     * 2. Initing Rank for all nodes
     * 3. Find and breaks "long" edges
     * 4. Makes ordering (horizontal placement)
     * 5. Place node at absolute positions
     */
    void Layout(unsigned long int number_of_iterations = 3,
        bool do_transpose = true,
        int transpose_range = -1,
	bool verbose = false);

    /**
     * Init Rank value for each node.
     *
     * rank = 0 for "head" nodes;
     * rank = max(rank(adj_nodes)) for others.
     */
    virtual void InitRank();

    /**
     * Breaks all "long" edges with dummy nodes
     */
    void AddDummyNodes(list<pEdge>& LongEdges);

    /**
     * Finds all "long" edges in graph and puts it to the LondEdges list.
     */
    void FindLongEdges(list<pEdge>& LongEdges);

    /**
     * Init pos value for each node using order.
     */
    void InitPos(Ordering* order);

    /**
     * Init coordiates for each node.
     * \todo this should be flexible
     */
    void InitCoordinates(Ordering* order,
        int normalwide = 50,
        int dummywide = 25,
        int vertical_size = 80);

    /**
     * The weighted median heuristic for reducing edge crossings.
     * At each rank a vertex is assigned a median based on the adjacent
     * vertices on the previous rank. Then, the vertices in the rank are
     * sorted by their medians. returns true if did not changed.
    */
    bool WeightedMedianHeuristic(int iter, bool verbose);

    /**
     * The transposition heuristic for reducing edge crossings.
     * Transpose repeatedly exchanges adjacent vertices on the
     * same rank if this decreases the number of crossings.
     */
    void Transpose(unsigned long int maxtry, bool verbose);

    /**
     * Calculate all edges crossings in the whole graph.
     */
    int countCrossing(Ordering* order);

    /**
     * Calculate all crossings between rank an rank+1.
     */
    int countCrossingOnRank(Ordering* order, int rank);

    /**
     * The initial (horizontal)ordering.
     * Should be called after InitRank().
     */
    vector<vector<pLNode>> InitOrder();

    /**
     * add real node
     */
    virtual pLNode AddNode();

    /**
     * add edge
     */
    virtual pLEdge AddEdge(pNode from, pNode to);

    /**
     * free node
     */
    virtual void FreeNode(pNode p);

    /**
     * free edge
     */
    virtual void FreeEdge(pEdge p);

    friend class LEdge;
    friend class LNode;
    friend class Ordering;
};

#endif

/* end. */
