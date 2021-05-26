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
 * @file: Graph.cpp
 */
#include "predecls.h"
#include <assert.h>

/**
 * print graph data
 */
void Graph::Dump()
{
    list<pNode>::iterator node_iter;
    list<pEdge>::iterator edge_iter;

    printf("Dumping graph id:%d (%d nodes, %d real nodes, %d dummy nodes, %d edges, %d selfedges %d reversed edges)\n", id(), nnodes(), nrealnodes(), ndummynodes(), nedges(), nselfedges(), nreversededges());
    printf("Nodes:\n");
    for (node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {
        pNode pn = *node_iter;
        if (pn) {
            /* print single node */
            pn->Dump();
        }
    }

    // edge list does not have self edges
    printf("Edges:\n");
    for (edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {
        pEdge pe = *edge_iter;
        if (pe) {
            /* print single edge */
            pe->Dump();
        }
    }

    return;
}

/**
 * free node
 */
void Graph::FreeNode(pNode p)
{
    assert(p != NULL);
    delete (pNode)p;
}

/**
 * free edge
 */
void Graph::FreeEdge(pEdge p)
{
    assert(p != NULL);
    delete (pEdge)p;
}

/**
 * clear graph
 * \todo check
 */
void Graph::Destroy()
{
    //    list<pNode>::iterator node_iter;
    //    list<pEdge>::iterator edge_iter;

    while (m_nodes_list.empty() == false) {
        pNode pn = m_nodes_list.front();
        // delete node and it in/out edges
        DeleteNode(pn);
    }

    assert(m_nodes_list.empty());
    assert(m_edges_list.empty());
    assert(m_total_edges_num == 0);
    assert(m_total_nodes_num == 0);

    return;
}

/**
 * create node
 */
pNode Graph::AddNode()
{
    pNode new_node = new Node(this);
    return new_node;
}

/**
 * increase count of dummy nodes
 */
void Graph::AddDummyNode()
{
    m_total_dummynodes_num++;
    return;
}

/**
 * create edge
 */
pEdge Graph::AddEdge(pNode from, pNode to)
{
    pEdge new_edge = new Edge(from, to);
    return new_edge;
}

/**
 * delete node and it in/out edges
 */
void Graph::DeleteNode(pNode p)
{
    bool found = false;

    // todo if dummy node, update graph dummy node count

    // clear incoming edges
    while (!p->m_in_edges_list.empty()) {
        found = DeleteEdge(p->m_in_edges_list.front()->m_from, p);
        if (found == false) { // edge not found
        }
    }

    // clear outgoing edges
    while (!p->m_out_edges_list.empty()) {
        found = DeleteEdge(p, p->m_out_edges_list.front()->m_to);
        if (found == false) { // edge not found
        }
    }

    // remove node from nodelist
    m_nodes_list.remove(p);

    // decr. number of total nodes
    if (m_total_nodes_num > 0) {
        m_total_nodes_num--;
    }

    // clear node
    FreeNode(p);

    return;
}

/**
 * remove edge between node from and node to
 * return tru if edge deleted, false when edge not found
 */
bool Graph::DeleteEdge(pNode from, pNode to)
{
    assert(from && from->m_graph == this);
    assert(to && to->m_graph == this);

    // delete self edge if specified
    if (from->id() == to->id()) {
        // this is a self edge
        if (from->nselfedges() <= 0) {
            // shouldnothappen
            from->m_selfedges = 0;
            return false;
        }
        from->m_selfedges--;
        // update in graph data
        if (from->m_graph->m_total_selfedges_num > 0) {
            from->m_graph->m_total_selfedges_num--;
        }
        return true;
    }

    list<pEdge>::iterator edge_iter;

    // scan edge list
    for (edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {
        pEdge pe = *edge_iter;
        // if this is edge with from/to number
        if (pe->m_from == from && pe->m_to == to) {
            m_edges_list.erase(edge_iter);
            // remove as outgoing edge at from node
            pe->m_from->m_out_edges_list.remove(pe);
            // remove as incoming edge at to node
            pe->m_to->m_in_edges_list.remove(pe);
            pe->m_from = NULL;
            pe->m_to = NULL;
            m_total_edges_num--;
            FreeEdge(pe);
            // edge is found and deleted
            return true;
        }
    }

    // return false if edge not found.
    return false;
}

/**
 * return false if some integrity error of graph data
 */
bool Graph::Verify(void)
{
    // error if no nodes, but there are edges.
    if (m_edges_list.size() > 0 && m_nodes_list.size() == 0) {
        return false;
    }

    map<pEdge, bool> edge_map;

    // scan all nodes
    for (list<pNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {

        // error if node does not belong to this graph
        if ((*node_iter)->m_graph != this) {
            return false;
        }

        // scan incoming edges at this node
        for (list<pEdge>::iterator edge_iter = (*node_iter)->m_in_edges_list.begin();
             edge_iter != (*node_iter)->m_in_edges_list.end();
             edge_iter++) {

            // error if edge does not belong to this graph
            if ((*node_iter)->m_graph != (*edge_iter)->m_graph) {
                return false;
            }
        }

        // scan outgoing edges at this node
        for (list<pEdge>::iterator edge_iter = (*node_iter)->m_out_edges_list.begin();
             edge_iter != (*node_iter)->m_out_edges_list.end();
             edge_iter++) {

            // error if edge does not belong to this graph
            if ((*node_iter)->m_graph != (*edge_iter)->m_graph) {
                return false;
            }
        }
    }

    // scan all edges
    for (list<pEdge>::iterator edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {

        edge_map[(*edge_iter)] = true;
    }

    // scan all edges
    for (list<pEdge>::iterator edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {

        // error if edge has no from node or no end node
        if ((*edge_iter)->m_from == NULL || (*edge_iter)->m_to == NULL) {
            return false;
        }

        // error if edge does not belong to a graph
        if ((*edge_iter)->m_graph == NULL) {
            return false;
        }

        // error if one of nodes does not belong to a graph
        if ((*edge_iter)->m_from->m_graph == NULL || (*edge_iter)->m_to->m_graph == NULL) {
            return false;
        }

        // error if edge does not belong to this graph
        if ((*edge_iter)->m_graph != this) {
            return false;
        }

        // error if iter does not belong to this graph
        if (!((*edge_iter)->m_graph == (*edge_iter)->m_to->m_graph && (*edge_iter)->m_graph == (*edge_iter)->m_from->m_graph)) {
            return false;
        }

        bool flag = false;

        // Check for list of incoming edges.
        for (list<pEdge>::iterator edge_local = (*edge_iter)->m_to->m_in_edges_list.begin();
             edge_local != (*edge_iter)->m_to->m_in_edges_list.end();
             edge_local++) {

            // Sets if this is exist incoming edge
            if ((*edge_iter) == (*edge_local)) {
                flag = true; // named edge_iter.
            }

            // If there are some edges without declaration
            if (edge_map[(*edge_iter)] == false) {
                return false; // in m_edges_list, graph isn't correct.
            }
        }

        /* stop at error */
        if (flag == false) {
            return false;
        }

        flag = false;

        // The same check for list of outgoing edges.
        for (list<pEdge>::iterator edge_local = (*edge_iter)->m_from->m_out_edges_list.begin();
             edge_local != (*edge_iter)->m_from->m_out_edges_list.end();
             edge_local++) {

            // Sets if this is exist outgoing edge
            if ((*edge_iter) == (*edge_local)) {
                flag = true;
            }

            // If there are some edges without declaration
            if (edge_map[(*edge_iter)] == false) {
                return false;
            }
        }

        // stop at error
        if (flag == false) {
            return false;
        }
    }

    // graph data is correct
    return true;
}

/**
 * run dfs to assing node dfs number
 */
void Graph::DFS(pNode node,
    map<pNode, bool>* isused,
    map<pNode, int>* dfs,
    int* num)
{

    if ((*isused)[node] == true) {
        // return if node has already set
        return;
    }

    (*isused)[node] = true;

    // set dfs count
    (*dfs)[node] = ++(*num);

    // follow outgoing edges of this node
    for (list<pEdge>::iterator edge_iter = node->m_out_edges_list.begin();
         edge_iter != node->m_out_edges_list.end();
         edge_iter++) {
        // if to edge is not done, recurse
        if (((*isused)[(*edge_iter)->m_to]) == false) {
            DFS((*edge_iter)->m_to, isused, dfs, num);
        }
    }

    return;
}

/**
 * return true if revesed edges count changed
 * find cycles in the graph and reverse edge
 */
bool Graph::FindReverseEdges(list<pEdge>& ReverseEdges)
{
    map<pNode, int>* dfs = new map<pNode, int>; // dfs count numbers
    map<pNode, bool>* isused = new map<pNode, bool>; // dfs flags
    int num = 0; // Current dfs counter is 0
    size_t count_rev_edges = ReverseEdges.size();

    // clear dfs
    for (list<pNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {
        (*dfs)[*node_iter] = 0;
    }

    // clear used
    for (list<pNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {
        (*isused)[(*node_iter)] = false;
    }

    for (list<pNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {

        if ((*isused)[*node_iter] == false) {
            // Starts from every new node.
            DFS((*node_iter), isused, dfs, &num);
        }
    }

    delete isused;

    // detect cycles
    for (list<pEdge>::iterator edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {

        if ((*dfs)[(*edge_iter)->m_to] < (*dfs)[(*edge_iter)->m_from]) {
            // this is a cycle
            ReverseEdges.push_back((*edge_iter));
        }
    }

    delete dfs;

    // total number of reversed edges without self-edges
    m_total_reversededges_num = ReverseEdges.size();

    if (ReverseEdges.size() == count_rev_edges) {
        return false; // No changes
    }

    /* reversed edges changed  */
    return true;
}

/**
 * set uniq graph id as int number
 */
void Graph::Setid(int id)
{

    m_id = id;

    return;
}

/* end. */
