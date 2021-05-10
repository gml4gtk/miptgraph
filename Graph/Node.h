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
class Node{
	pGraph m_graph;
	list<pEdge> m_in_edges_list;
	list<pEdge> m_out_edges_list;
	int m_id;
protected:
	Node(pGraph graph);

public:
	/// Get node id
	int id() { return m_id;}
	
	list<pEdge> * in_edges_list(){
		return &m_in_edges_list;
	}

	list<pEdge> * out_edges_list(){
		return &m_out_edges_list;
	}

	/// Print info about the node
	virtual void Dump();
	friend class Edge;
	friend class Graph;
};
#endif
