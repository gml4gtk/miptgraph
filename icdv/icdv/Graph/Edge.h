/** 
 * @file: Edge.h
 */
#ifndef GRAPH_EDGE_H
#define GRAPH_EDGE_H

/// Class for representation of edges
/**
 * An edge has two Nodes as its end points, pointer to graph that the edge relates to, and unique id.
 */
class Edge{
	pGraph m_graph;
	pNode m_from, m_to;
	int m_id;
protected:
	/// Ctor
	Edge(pNode from, pNode to);

        // flag is true if we alredy revert this edge for layout.
        bool reverse;
public:
        bool IsReverse() {
            return reverse;
        }

        void SetReverse(bool is_reverse) {
            reverse = is_reverse;
        }

	void Reverse();
	/// Get m_graph
	pGraph graph() {return m_graph;}

	/// Get edge id
	int  id() {return m_id;}

	/// Get pointer to edge start node
	pNode from() { return m_from;}

	/// Get pointer to edge end node
	pNode to() { return m_to;}

	/// Print info about the edge
	virtual void Dump();

	/// Concise version of Dump
	virtual void Print();

	friend class Node;
	friend class Graph;
};

#endif
