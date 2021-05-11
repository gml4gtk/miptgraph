/**
* File: rank.h - Implementation of rank structure and nodes ordering within it.
* Copyright (C) 2009  MIPTVIS
*/

#ifndef RANK_H
#define RANK_H

/* wrong: #include <QtCore/QVector.h>*/
#include <QtCore/QVector>

/**
* Implementation of layer with nodes of the same rank
* and ordering methods on it
*/

int posCmp( const void*, const void*);
int valCmp( const void*, const void*);

class AdjRank
{
private:

	/** List of nodes within one layer */
	List<NodeAux> adj_rank;

	/** Median value for node v from connected nodes in rank r */
	int medianValue( NodeAux* v);

	/** Barycenter function for node v from connected nodes in rank r */
	int baryValue( NodeAux* v);

	/** Transpose ordering algorithm */
	void transpose();
public:

	inline int rang();

	/** Get first element of the layer */

	void debugPrint();

	/** Set initial X coordinates */
	void setInitX();

	/** Ordering nodes
	* Uses median/barycenter and transpose methods maxIter times
	*/
	void doOrder( AdjRank* comp_rank);

	/** Add node to layer */
	void addNode( NodeAux* node)
	{
		adj_rank.addItem( node);
		node->setLayerIter( adj_rank.tail());
	}

	/** Get first node of the layer */
	NodeAux* first()
	{
		return adj_rank.head()->data();
	}

	/** Get first node of the layer */
	NodeAux* last()
	{
		return adj_rank.tail()->data();
	}

	/** Sorting nodes depends on ordering DFS numeration */
	void sortByNum()
	{
		adj_rank.sort( &posCmp);
	}

	/** Sorting nodes depends on median/barycenter value */
	void sortByVal()
	{
		adj_rank.sort( &valCmp);
	}
};
#endif