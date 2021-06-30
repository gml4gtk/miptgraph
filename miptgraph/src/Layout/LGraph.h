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
 * node positions of connected nodes and used for positioning
 * upper_left = 0
 * lower_left = 1
 * upper_right = 2
 * lower_right = 3
 */
enum orient { upper_left, lower_left, upper_right, lower_right };

/**
 * Graph with Layout methods
 * Includes ranking, median and transposition heuristic
 */

class LGraph {

    /**
     * list of all nodes including dummy nodes
     */
    list< pLNode > m_nodes_list;

    /**
     * list of all edges without self edges
     */
    list< pLEdge > m_edges_list;

    /**
     * list of all self-edges
     */
    list< pLEdge > m_selfedges_list;

    /**
     * total number of nodes including dummy nodes
     */
    int m_total_nodes_num;

    /**
     * total number of dummy nodes
     */
    int m_total_dummynodes_num;


    /**
     * total number of edges without self-edges
     */
    int m_total_edges_num;

    /**
     * total number of horizontal edges without self-edges
     */
    int m_total_horedges_num;

    /**
     * total number of self-edges
     */
    int m_total_selfedges_num;

    /**
     * total number of reversed edges without self-edges
     */
    int m_total_reversededges_num;

    /**
     * serial number for uniq node id
     */
    int next_node_id;

    /**
     * serial number for uniq edge id
     */
    int next_edge_id;

    /**
     * uniq graph number if set by user or 0 if not set
     */
    int m_id;

    /**
     * x spacing in the graph
     */
    int m_xspacing_num;

    /**
     * y spacing in the graph
     */
    int m_yspacing_num;

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
     * 4 arrays with node pointers for every node as start of block where node belongs to
     */
    std::array< vector < pLNode > , 4 > root;

    /**
     * 4 arrays with node pointers for every node as node to align with for node
     */
    std::array< vector < pLNode > , 4 > align;

    /**
     * 4 arrays with node pointers
     */
    std::array< vector < pLNode > , 4 > sink;

    /**
     * 4 arrays with node pointers
     */
    std::array< vector < pLNode > , 4 > medians;

    /**
     * 4 arrays with node shift
     */
    std::array< vector < double > , 4 > shift;

    /**
     * splay with node id and node
     */
    splay_tree nodesplay = (splay_tree)0;

    /**
     * splay with edge id and edge
     */
    splay_tree edgesplay = (splay_tree)0;

    /**
     * create graph
     */
    LGraph();

    /**
     * delete graph
     */
    ~LGraph();

    /**
     * Replacing revert edges from the ReverseEdges list.
     */
    void ReverseReverseEdges(list<pLEdge>& ReverseEdges);

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
	bool verbose = false,
	bool usebary = false
	);

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
    void AddDummyNodes(list<pLEdge>& LongEdges);

    /**
     * Finds all "long" edges in graph and puts it to the LondEdges list.
     */
    void FindLongEdges(list<pLEdge>& LongEdges);

    /**
     * Init pos value for each node using order.
     */
    void InitPos(Ordering* order);

    /**
     * Check node in order data
     */
    void CheckOrder(Ordering* order);

    /**
     * Init coordiates for each node
     */
    void InitCoordinates(Ordering* order,
        int normalwide = 15,
        int dummywide = 5,
        int vertical_size = 40);

    /**
     * set nodes at final (x,y) position in the graph
     */
    void FinalCoordinates(Ordering* order);

    /**
     * The weighted median heuristic for reducing edge crossings.
     * At each rank a vertex is assigned a median based on the adjacent
     * vertices on the previous rank. Then, the vertices in the rank are
     * sorted by their medians. returns true if did not changed.
    */
    bool WeightedMedianHeuristic(int iter, bool verbose, bool usebary);

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
     * Calculate all edges crossings in the whole graph.
     */
    int FinalcountCrossing(Ordering* order);

    /**
     * Calculate all crossings between rank an rank+1.
     */
    int FinalcountCrossingOnRank(Ordering* order, int rank);

    /**
     * The initial (horizontal)ordering.
     * Should be called after InitRank().
     */
    vector<vector<pLNode>> InitOrder();

    /**
     * init edge crossing info
     */
    vector<int> InitOrder2();

    /**
     * find node with id
     */
    virtual pLNode FindNode (int num);

    /**
     * find edge with id
     */
    virtual pLEdge FindEdge (int num);

virtual void FinalCoordinates_init_medians(Ordering* order);

    /**
     * get list of all nodes in graph including dummy nodes
     */
    list<pLNode>* nodes_list()
    {
        return &m_nodes_list;
    }

    /**
     * get list of edges in graph without self-edges
     */
    list<pLEdge>* edges_list()
    {
        return &m_edges_list;
    }

    /**
     * get list of self-edges in graph
     */
    list<pLEdge>* selfedges_list()
    {
        return &m_selfedges_list;
    }

    /**
     * get uniq graph id if set by user or 0
     */
    int id() { return m_id; }

    /**
     * get max. node id possible
     */
    int maxnodeid () { return next_node_id; }

    /**
     * get max. edge id possible
     */
    int maxedgeid () { return next_edge_id; }

    /**
     * get number of real nodes in graph without dummy nodes
     */
    int nrealnodes() { return (m_total_nodes_num - m_total_dummynodes_num); }

    /**
     * get number of nodes in graph including dummy nodes
     */
    int nnodes() { return m_total_nodes_num; }

    /**
     * get number of dummy nodes in graph
     */
    int ndummynodes() { return m_total_dummynodes_num; }

    /**
     * get number of edges in graph without self-edges
     */
    int nedges() { return m_total_edges_num; }

    /**
     * get number of horizontal edges in graph without self-edges
     */
    int nhoredges() { return m_total_horedges_num; }

    /**
     * set number of horizontal edges in graph without self-edges
     */
    void Set_Nhoredges(int value) { m_total_horedges_num = value; }

    /**
     * get number of reversed edges in graph without self-edges
     */
    int nreversededges() { return m_total_reversededges_num; }

    /**
     * get number of self-edges in graph
     */
    int nselfedges() { return m_total_selfedges_num; }

    /**
     * get x spacing
     */
    int xspacing() { return m_xspacing_num; }

    /**
     * set x spacing
     */
    void Setxspacing(int value ) { m_xspacing_num = value; }

    /**
     * get y spacing
     */
    int yspacing() { return m_yspacing_num; }

    /**
     * set y spacing
     */
    void Setyspacing(int value ) { m_yspacing_num = value; }


    /**
     * Checks the integrity of data of the graph
     * \return tru if data is oke
     */
    virtual bool Verify();

/**
 * run dfs to assing node dfs number
 */
void DFS(pLNode node,
    map<pLNode, bool>* isused,
    map<pLNode, int>* dfs,
    int* num);

    /**
     * Search reverse edges and puts them into ReverseEdges list.
     * returns true if reversed edges increased
     */
    virtual bool FindReverseEdges(list<pLEdge>& ReverseEdges);

    /**
     * Add a new node to the graph
     * Allocate memory, create new node and insert it to nodes list of the graph
     * \sa AddEdge, DeleteNode, DeleteEdge
     */
    pLNode AddNode();

    /**
     * add node as dummy node
     */
    virtual void AddDummyNode();

    /**
     * Connect nodes from and to
     * Allocate needed memory, create new edge and insert it to 
     * edges lists of nodes from and to, and to edges list of graph
     * \param from - start node of the edge
     * \param to   - end node of the edge
     * \sa AddNode, DeleteNode, DeleteEdge
     */
    virtual pLEdge AddEdge(pLNode from, pLNode to, void *e);

    /**
     * Delete node from the graph and all  its incoming and outgoing edges,
     * and free memory, allocated for node
     * \param node - node to be deleted
     * \sa AddNode, AddEdge, DeleteEdge
     */
    virtual void DeleteNode(pLNode node);

    /**
     * Delete edge between nodes from and to, and free allocated memory
     * \param from - node from
     * \param to   - node to
     * \sa AddNode, AddEdge, DeleteNode
     * \return True if edge from->to had existed, False otherwise.
     */
    virtual bool DeleteEdge(pLNode from, pLNode to);

    /**
     * Free memory used for Node p
     * \param pNode pointer to free
     */
    virtual void FreeNode(pLNode p);

    /**
     * Free memory used for Edge p
     * \param pEdge pointer to free
     */
    virtual void FreeEdge(pLEdge p);

    /**
     * set uniq id number of graph
     * \param int graph number
     */
    virtual void Setid (int id);

    /**
     * Dump all nodes and edges
     */
    virtual void Dump();

    /**
     * Delete all nodes and edges
     */
    virtual void Destroy();

    friend class LEdge;
    friend class LNode;
    friend class Ordering;
};

#endif

/* end. */
