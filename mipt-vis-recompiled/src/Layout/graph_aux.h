/**
 * File: graph_aux.h - Auxiliary graph class definition/implementation, part of
 * Layout library in MiptVis tool.
 * Copyright (C) 2009  MIPTVIS
 */

/* wrong: #include <QTCore/QVector.h>*/
#include <QtCore/QVector>

#ifndef GRAPH_AUX_H
#define GRAPH_AUX_H

/**
 * Implementation of auxiliary graph class
 */
class GraphAux: public Graph
{
private:

	int max_rank;
	Numeration dfs_num;

	/* Rank structure for nodes ordering */
	QVector<AdjRank> rank;

	bool delVirtualNodes();

	NodeAux* findRoot();
	int markReachable (Node* root, Marker by_what);
	NodeAux* makeVirtualRoot (QList <Node*>& roots);

	NodeAux* makeAcyclic();//needs for Rank, changes types of edges

	void clearRanks();
	void rankImp (NodeAux* from, int cur_rank, Marker passed);
	bool passedAllPred (NodeAux* what, Marker passed);
	void   passAllSucc (NodeAux* from, int cur_rank, Marker passed);

	void addVirtualChains();

	void computeLayerHeights (int heights[]);
	void computeLayerYs (int ys[]);
	void arrangeVertical();

	/* Horizontal arrangement */
	void arrangeHorizontal();
	void forceDirectedPosition();
	void resultantForce( int rank_num);
	void saveMinDist( int rank_num);
	void decompact( int rank_num, int dir);

	/* Horizontal coordinates calculation based on median value positioning */
	void medianPosition();


public://!!! Only for tests, it must be closed
	bool ranking();
	bool ordering();
	bool position();
	bool make_splines();

public:
    bool doLayout();

	void iterateGravity();
public:
    /** Constructor */
    GraphAux(): Graph()
    {};

	/**Maximum rank */
	int maxRank()
	{
		return max_rank;
	}

	/** DFS num for ordering */
	Numeration dfsNum()
	{
		return dfs_num;
	}

	void setDfsNum( Numeration num)
	{
		dfs_num = num;
	}

    /** Allocation of memory for Edge */
    EdgeAux * createEdge( Node * pred, Node * succ);
				//???type NodeAux not lead to Node, and call "creatEdge (Node *, Node*)" not work
    /** Allocation of memory for Node */
    NodeAux * createNode();

    /**
    * Creation node in graph
    */
    NodeAux * newNode()
    {
        return static_cast< NodeAux*>( Graph::newNode());
    }

	NodeAux * firstNode()
	{
		return static_cast< NodeAux*>( Graph::firstNode());
	}
    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    EdgeAux * newEdge( NodeAux * pred, NodeAux * succ)
    {
        return static_cast< EdgeAux*>( Graph::newEdge( static_cast< Node*>( pred), static_cast< Node*>( succ)));
    };

	inline EdgeAux* firstEdge() 
    {
		return static_cast < EdgeAux*> ( Graph::firstEdge());
    }

    /**
	* Insert new node on edge in graph
	*/
    virtual NodeAux * insertNodeOnEdge( EdgeAux* e)
    {
        NodeAux* n = static_cast< NodeAux*>( Graph::insertNodeOnEdge( static_cast< Edge*>( e)));
        n->firstSucc()->type = n->firstPred()->type;
        return n;
    }

	virtual bool applayLayout()//implemented by graphic interface branch
	{
		return false;
	}
	int backedgeNumStat();
	int edgeCrossNumStat();
	bool interSection(EdgeAux* one_edge, EdgeAux* another_edge);
	void debugPrint();
};

#endif