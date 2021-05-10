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
	
/// Implementation of depth-first search.
/**
 * param node - the node from which DFS starts.
 * param isused - the map in which passed nodes were marked.
 * param dfs - the map of in which ways of DFS were marked.
 * param num - number of previous node.
*/
	void DFS(pNode node,
		map<pNode, bool> *isused,
		map<pNode, int> *dfs,
		int *num);
	list<pNode> m_nodes_list;
	list<pEdge> m_edges_list;
	int m_total_nodes_num;
	int m_total_edges_num;
	int next_node_id;
	int next_edge_id;
public:

	list<pNode> * nodes_list(){
		return  &m_nodes_list;
	}

	list<pEdge> *edges_list(){
		return  &m_edges_list;
	}

	Graph() :
		m_total_nodes_num(0), 
		m_total_edges_num(0),
		next_node_id(0),
		next_edge_id(0)
	{};

	virtual ~Graph() {
		Destroy();
	}

	/// Checks the integrity of data of the graph.
    virtual bool Verify();

    /// If this is not connected graph, CreateSingleEntry() will
	/// create single graph using DFS.
    virtual void CreateSingleEntry();

	/// Search reverse edges and puts them into ReverseEdges list.
	/// Graph must be connected.
	virtual bool FindReverseEdges(list<pEdge> &ReverseEdges);

	/// Add a new node to the graph
	/**
	 * Allocate memory, create new node and insert it to nodes list of the graph
	 *  \sa AddEdge, DeleteNode, DeleteEdge
	 */
	virtual pNode AddNode();

	/// Connect nodes from and to
	/**
	 * Allocate needed memory, create new edge and insert it to 
	 * edges lists of nodes from and to, and to edges list of graph
	 *	\param from - start node of the edge
	 *	\param to   - end node of the edge
	 *  \sa AddNode, DeleteNode, DeleteEdge
	 */
        virtual pEdge AddEdge(pNode from, pNode to);

	/// Delete node from the graph and all  its incoming and outgoing edges, 
	/// and free memory, allocated for node
	/** 
	 *	\param node - not to be deleted
	 *  \sa AddNode, AddEdge, DeleteEdge
	 */
	virtual void DeleteNode(pNode node);
	
	/// Delete edge between nodes from and to, and free allocated memory
	/** 
	 *	\param from - node from
	 *	\param to   - node to
	 *  \sa AddNode, AddEdge, DeleteNode
	 *  \return True if edge from->to had existed, False otherwise.
	 */
	virtual bool DeleteEdge(pNode from, pNode to);

	/// Free memory used for Node p
	virtual void  FreeNode(pNode p);
	/// Free memory used for Edge p
	virtual void  FreeEdge(pEdge p);

	virtual void Dump(); ///< Dump all nodes and edges
	virtual void Destroy(); ///< Delete all nodes and edges
	friend class Node;
	friend class Edge;

};

#endif
