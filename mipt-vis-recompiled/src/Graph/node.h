/**
 * File: node.h - Node class definition, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_H
#define NODE_H

/**
 * Node representation class. 
 */
class Node: public Marked, public Numbered
{
	int user_id;
    /** Graph is closely connected class by implementation */
    friend class Graph;
    
    /** Connection with inclusive graph */
    int unique_id; // Unique id is given by graph and cannot be modified
	protected:
		Graph * graph;// Pointer to graph
	private:
    NodeListItem my_it;//Item of graph's list
    
    //Lists of edges and iterators for them
    EdgeListItem *edges[ GRAPH_DIRS_NUM];
    EdgeListItem *e_it[ GRAPH_DIRS_NUM];

    inline NodeListItem* getGraphIt()
    {
        return &my_it;
    }
	inline void detachFromGraph()
    {
        my_it.detach();
    }
protected:
	virtual void writeByXmlWriter( xmlTextWriterPtr writer);
	virtual void readByXml (xmlNode * cur_node);
protected:

    /** We can't create nodes separately, do it through NewNode method of graph */
    Node( Graph *graph_p, int _id):
         unique_id( _id), graph( graph_p), my_it()
    {
        edges[ GRAPH_DIR_UP] = NULL;
        edges[ GRAPH_DIR_DOWN] = NULL;
        e_it[ GRAPH_DIR_UP] = NULL;
        e_it[ GRAPH_DIR_DOWN] = NULL;
        my_it.setData( ( Node*)this);
        setUserId( _id);
    }
    
public:

    /**
     * Destructor
     */
    virtual ~Node();
    
    /**
     * get node's unique ID
     */
    inline int id() const
    {
        return unique_id;
    }

    inline int userId() const
    {
        return user_id;
    }
    inline void setUserId( int i)
    {
        user_id = i;
    }

    /**
     * get node's corresponding graph
     */
    inline Graph * getGraph() const
    {
        return graph;
    }

    /**
     * Add edge to node in specified direction
     */
    virtual void addEdgeInDir( Edge *edge, GraphDir dir);

    /**
     * Add predecessor edge
     */
    virtual inline void addPred( Edge *edge)
    {
        addEdgeInDir( edge, GRAPH_DIR_UP);
    }

    /**
     * Add successor edge
     */
    virtual inline void addSucc( Edge *edge) 
    {
        addEdgeInDir( edge, GRAPH_DIR_DOWN);
    }

    /**
     *  Iteration through edges routines.
     *
     *  set iterator to beginning of edge list and return first edge
     */
    virtual inline Edge* firstEdgeInDir( GraphDir dir)
    {
        e_it[ dir ] = edges[ dir ];
        
        if ( e_it[ dir] == NULL)
        {
            return NULL;
        }
        return e_it[ dir ]->data();
    }
    /**
     * Advance iterator and return next edge in specified direction
     * NOTE: If end of list is reached we return NULL for first time and fail if called once again
     */
    virtual inline Edge* nextEdgeInDir( GraphDir dir)
    {
        e_it[ dir] = e_it[ dir]->next();
        return (e_it[ dir] != NULL )? e_it[ dir]->data() : NULL;
    }
    /**
     * Return true if iterator of list points to one-after-last element
     */
    inline bool endOfEdgesInDir( GraphDir dir)
    {
        return e_it [ dir] == NULL;
    }

    /** 
     * Corresponding iterators for successors/predeccessors.
     * NOTE: See firstEdgeInDir and other ...InDir routines for details
     */
    virtual inline Edge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    virtual inline Edge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    inline bool endOfSuccs()
    {
        return endOfEdgesInDir( GRAPH_DIR_DOWN);
    }
    virtual inline Edge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    virtual inline Edge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    }
    inline bool endOfPreds()
    {
        return endOfEdgesInDir( GRAPH_DIR_UP);
    }

    /**
     * Deletion of edge in specified direction
     */
    void deleteEdgeInDir( GraphDir dir, EdgeListItem* it);
    
    /**
     * delete predecessor edge
     */
    inline void deletePred( EdgeListItem* it)
    {
        deleteEdgeInDir( GRAPH_DIR_UP, it);
    }
    
    /**
     * delete successor edge
     */
    inline void deleteSucc( EdgeListItem* it)
    {
        deleteEdgeInDir( GRAPH_DIR_DOWN, it);
    }

    /**
	 * Return next node of a graph
	 */
	inline Node* nextNode()
    {
        return ( my_it.next() != NULL )? my_it.next()->data() : NULL;
    }

	/**
	 * Return previous node of a graph
	 */
    inline Node* prevNode()
    {
        return ( my_it.prev() != NULL )? my_it.prev()->data() : NULL;
    }

	/**
     * Print node in DOT format to stdout
     */
    virtual void debugPrint();
};
#endif
