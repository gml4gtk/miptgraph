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
 * @file: Graph.h
 */
#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

/// Class for representation of directed graphs
/**
 * A directed graph or digraph is a pair G = (N,E) of: <br>
 *        * a set N, whose elements are called nodes, <br>
 *        * a set E of ordered pairs of vertices, directed edges <br><br>
 *
 * Graph contains list of nodes, list of edges, and the corresponding counters.<br>
 * It incapsulates all the work with memory allocation/deallocation, thus 
 * the user has no need to worry about it.
 */
class Graph {

protected:
    /**
     * Implementation of depth-first search.
     * param node - the node from which DFS starts.
     * param isused - the map in which passed nodes were marked.
     * param dfs - the map of in which ways of DFS were marked.
     * param num - number of previous node.
    */
    void DFS(pNode node,
        map<pNode, bool>* isused,
        map<pNode, int>* dfs,
        int* num);

    /**
     * list of all nodes including dummy nodes
     */
    list<pNode> m_nodes_list;

    /**
     * list of all edges without self edges
     */
    list<pEdge> m_edges_list;

    /**
     * list of all self-edges
     */
    list<pEdge> m_selfedges_list;

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

public:

    /**
     * get list of all nodes in graph including dummy nodes
     */
    list<pNode>* nodes_list()
    {
        return &m_nodes_list;
    }

    /**
     * get list of edges in graph without self-edges
     */
    list<pEdge>* edges_list()
    {
        return &m_edges_list;
    }

    /**
     * get list of self-edges in graph
     */
    list<pEdge>* selfedges_list()
    {
        return &m_selfedges_list;
    }

    /**
     * create graph with defaults
     */
    Graph()
        : m_total_nodes_num(0)
        , m_total_dummynodes_num(0)
        , m_total_edges_num(0)
        , m_total_selfedges_num(0)
        , m_total_reversededges_num(0)
        , next_node_id(0)
        , next_edge_id(0)
	, m_id(0) {};

    /**
     * get uniq graph id if set by user or 0
     */
    int id() { return m_id; }

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
     * get number of reversed edges in graph without self-edges
     */
    int nreversededges() { return m_total_reversededges_num; }

    /**
     * get number of self-edges in graph
     */
    int nselfedges() { return m_total_selfedges_num; }


    /**
     * free graph
     */
    virtual ~Graph()
    {
        Destroy();
    }

    /**
     * Checks the integrity of data of the graph
     * \return tru if data is oke
     */
    virtual bool Verify();

    /// If this is not connected graph, CreateSingleEntry() will
    /// create single graph using DFS.
    virtual void CreateSingleEntry();

    /// Search reverse edges and puts them into ReverseEdges list.
    /// Graph must be connected.
    virtual bool FindReverseEdges(list<pEdge>& ReverseEdges);

    /**
     * Add a new node to the graph
     * Allocate memory, create new node and insert it to nodes list of the graph
     * \sa AddEdge, DeleteNode, DeleteEdge
     */
    virtual pNode AddNode();

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
    virtual pEdge AddEdge(pNode from, pNode to);

    /**
     * Delete node from the graph and all  its incoming and outgoing edges,
     * and free memory, allocated for node
     * \param node - node to be deleted
     * \sa AddNode, AddEdge, DeleteEdge
     */
    virtual void DeleteNode(pNode node);

    /**
     * Delete edge between nodes from and to, and free allocated memory
     * \param from - node from
     * \param to   - node to
     * \sa AddNode, AddEdge, DeleteNode
     * \return True if edge from->to had existed, False otherwise.
     */
    virtual bool DeleteEdge(pNode from, pNode to);

    /**
     * Free memory used for Node p
     * \param pNode pointer to free
     */
    virtual void FreeNode(pNode p);

    /**
     * Free memory used for Edge p
     * \param pEdge pointer to free
     */
    virtual void FreeEdge(pEdge p);

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

    friend class Node;
    friend class Edge;
};

#endif

/* end. */
