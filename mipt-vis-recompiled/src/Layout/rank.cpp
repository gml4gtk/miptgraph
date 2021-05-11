/**
* File: rank.cpp - Implementation of rank structure and nodes ordering within it.
* Copyright (C) 2009  MIPTVIS
*/

#include "layout_iface.h"

/** Compare two nodes by their DFS numeration */
int posCmp( const void *x, const void *y)
{
	int res;
	NodeAux* nodeX = ( NodeAux*)x;
	NodeAux* nodeY = ( NodeAux*)y;
	Numeration dfs_num = nodeX->getGraph()->dfsNum();
	int posX = nodeX->number( dfs_num);
	int posY = nodeY->number( dfs_num);
	if ( posX < posY) res =  -1;
	if ( posX > posY) res = 1;
	if ( posX == posY) res = 0;
	return res;
}

/** Compare two nodes by median/barycenter value */
int valCmp( const void *x, const void *y)
{
	int res;
	NodeAux* nodeX = ( NodeAux*)x;
	NodeAux* nodeY = ( NodeAux*)y;
	int valX = nodeX->x();
	int valY = nodeY->x();
	if ( valX < valY) res =  -1;
	if ( valX > valY) res = 1;
	if ( valX == valY) res = 0;
	return res;
}

int AdjRank::rang()
{
	return adj_rank.head()->data()->rang();
}

/** Median value for node v from connected nodes in current rank */
int AdjRank::medianValue( NodeAux* v)
{
	return 0;
};

/**
 * Barycenter function for node v from connected nodes in current rank
 * All nodes are equal. Some esthetic mass equivalent can be added later.
 * We use here node x coordinate as barycenter value, but in this method
 * it is just a comparative value, not drawable.
 */
int AdjRank::baryValue( NodeAux* v)
{
	int bary = 0;
	int i = 0;
	for ( EdgeAux* iter = v->firstSucc(); iter != NULL; iter = iter->nextSucc())
	{
		if( iter->succ()->rang() == rang()) 
		{
			bary += iter->succ()->x();
			i++;
		}
	}
	for ( EdgeAux* iter = v->firstPred(); iter != NULL; iter = iter->nextPred())
	{
		if( iter->pred()->rang() == rang())
		{
			bary += iter->pred()->x();
			i++;
		}
	}
	if (i == 0) return v->x();
	return bary / i;
}

/** Transpose ordering algorithm */
void AdjRank::transpose()
{};

/** 
 * Ordering nodes
 * Uses median/barycenter and transpose methods
 */
void AdjRank::doOrder( AdjRank* comp_rank)
{
	for( ListItem< NodeAux>* iter = adj_rank.head(); iter != NULL; iter = iter->next())
	{
		iter->data()->msetX( comp_rank->baryValue( iter->data())/* + iter->data()->width() / 2*/);
	}
	sortByVal();
	setInitX();
};

/** Print all nodes off layer with some info */

void AdjRank::debugPrint()
{
	for( ListItem< NodeAux>* iter=adj_rank.head(); iter!=NULL; iter = iter->next())
	{
		iter->data()->debugPrint();
	}
}

/** Set initial X coordinates */
void AdjRank::setInitX()
{
	int cur = 0;
	int i = 0;
	const int offset = 50;
	for( ListItem< NodeAux>* iter=adj_rank.head(); iter!=NULL; iter = iter->next())
	{
		iter->data()->setPosAux( i++);
		iter->data()->msetX(cur/* + iter->data()->width() / 2*/);
		cur+= iter->data()->width() + offset; // iter->data()->width() + offset;
	}
}
