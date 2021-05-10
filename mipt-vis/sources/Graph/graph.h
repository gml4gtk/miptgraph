/**
 * File: graph.h - Graph class definition/implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>


/**
 * Represents properties of a graph+
 */
class GraphProperties
{
protected:
	int default_node_size;
	char * name_priv;
	int max_node_id;    
public:
    GraphProperties()
    {
        default_node_size = 10;
	    name_priv = NULL;
	    max_node_id = 0; 
    }
    /**
     * Data retrieving routines
     */
    inline int defaultNodeSize() const
    {
        return default_node_size;
    }
    inline char * name() const
    {
        return name_priv;
    }
	inline int maxNodeId() const
    {
        return max_node_id;  
    }
    /**
     * Data saving routines
     */
    inline void setDefaultNodeSize( int sz)
    {
        default_node_size = sz;
    }
    inline void setName( char *nm)
    {
        name_priv = nm;
    }
	inline void setMaxNodeId( int max_id)
    {
        max_node_id = max_id;  
    }
};

/**
 * Graph class decribes a graph.
 *  Like classical definition G = ( N, E) where N is set of nodes n and E is set of edges e = {n_i, n_j}
 */
class Graph: public MarkerManager, public NumManager, public GraphProperties
{
    /* List of nodes and its iterator */
    Node* first_node;
    unsigned long int node_num;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphNum node_next_id;

    /* List of edges and its iterator */
    Edge* first_edge;
    unsigned long int edge_num;
    
    /** Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphNum edge_next_id;

	/** Initialization */
	void init();
protected:
    /** Increment node_next_id and return previous one */
    inline GraphNum incNodeId()
    {
        return node_next_id++;
    }
    /** Increment edge_next_id and return previous one */
    inline GraphNum incEdgeId()
    {
        return edge_next_id++;
    }
        
    /** Allocation of memory for Edge */
    virtual Edge * createEdge( Node * pred, Node * succ);
    
    /** Allocation of memory for Node */
    virtual Node * createNode();

public:
    /** Constructor */
    Graph();

	/** Constructor by XML file */
	Graph( char * filename);

	~Graph();

	/**
	 *  Reading form XML
	 */
	xmlDoc *xml_doc;
	virtual void readAttribsFromXml (xmlNode * a_node) {}//for additional properties
	void readNodesFromXmlDoc( xmlNode * a_node);
	void readEdgesFromXmlDoc( xmlNode * a_node, vector<Node *> nodes);
	void readFromXmlDoc( xmlNode * a_node);
	void readFromXml(const char *filename);

	void writeToXml(const char *filename);
	void writeNodesByXmlWriter( xmlTextWriterPtr writer);
	void writeEdgesByXmlWriter( xmlTextWriterPtr writer);
	virtual void writeAttribsByXmlWriter (xmlTextWriterPtr writer) {}//for additional properties
    
    /** Create new node in graph */
    Node * newNode();

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    Edge * newEdge( Node * pred, Node * succ);
    
    /**
     * Remove node from node list of graph
     */
    inline void detachNode( Node* n)
    {
        graphassert( isNotNullP( n));
        if( first_node == n)
        {
           first_node = n->nextNode();
        }
		n->detachFromGraph();
		--node_num;
    }

    /**
     * Remove edge from edge list of graph
     */
    inline void detachEdge( Edge* e)
    {
        graphassert( isNotNullP( e));
        if( first_edge == e)
        {
            first_edge = e->nextEdge();
        }
        e->detachFromGraph();
		--edge_num;
    }

	virtual void removeEdge (Edge* e);
	virtual void removeNode (Node* n);

	/**
	* Insert new node on edge in graph
	*/
	Node * insertNodeOnEdge( Edge* e);

    /**
     * Return node quantity
     */
    inline size_t getNodeCount() const
    {
        return node_num;
    }

    /**
     * Return edge quantity
     */
    inline size_t getEdgeCount() const
    {
        return edge_num;
    }
    /*** 
     * Iteration through edges implementation
     *
     * Initialize iterator with first edge and return this edge
     */
    inline Edge* firstEdge() 
    {
        return first_edge;
    }
    
    /*** 
     * Iteration through nodes implementation
     *
     * Initialize iterator with first node and return this node
     */
    inline Node* firstNode()
    {
        return first_node;
    }

    /**
     * Print graph to stdout in DOT format
     */
    virtual void debugPrint();

    /**
     * Obtain list of nodes in depth-first search order
     */
    virtual NodeListItem* DFS( Numeration n); 
	/**
     * Obtain list of nodes in breadth-first search order
     */
    virtual NodeListItem* BFS( Numeration n);

    /**
     * Clear unused markers from marked objects
     */
    void clearMarkersInObjects();

    /**
     * Clear unused numerations from numbered objects
     */
    void clearNumerationsInObjects();
};

#endif
