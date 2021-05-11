/**
 * File: Layout/layout_iface.h - Interface of layout library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef LAYOUT_IFACE_H
#define LAYOUT_IFACE_H

#include "layout_predecls.h"
#include "edge_aux.h"
#include "node_aux.h"
#include "rank.h"
#include "graph_aux.h"
#include "node_group.h"

const int Big_way_len_value = 10000000;

/**
 * Get node's corresponding auxiliary graph
 */
inline GraphAux * NodeAux::getGraph() const
{
    return static_cast< GraphAux *>( Node::getGraph());
}

	/**
* get node in specified direction
*/
inline NodeAux* EdgeAux::getNode( GraphDir dir)
{
	return static_cast< NodeAux*> ( Edge::getNode( dir));
}

/**
* get predecessor of edge
*/
inline NodeAux* EdgeAux::pred()
{
	return getNode( GRAPH_DIR_UP);
}
/**
* get successor of edge
*/
inline NodeAux* EdgeAux::succ()
{
	return getNode( GRAPH_DIR_DOWN);
}

inline EdgeAux *GraphAux::createEdge( Node * pred, Node * succ)
{
    return new EdgeAux( this, incEdgeId(), static_cast<NodeAux*> (pred), static_cast<NodeAux*> (succ));
}
  
inline NodeAux *GraphAux::createNode()
{
    return new NodeAux( this, incNodeId());
}


#define CUT_AUX_DEFINITION(type)			\
inline type* cutAux (type##Aux *what)		\
{											\
	return static_cast <type*> (what);		\
}
#define ADD_AUX_DEFINITION(type)			\
inline type##Aux* addAux (type* what)		\
{											\
	return static_cast <type##Aux*> (what);	\
}
#define ADD_CUT_AUX_DEFINITION(type) CUT_AUX_DEFINITION(type) ADD_AUX_DEFINITION(type)

ADD_CUT_AUX_DEFINITION(Node)
ADD_CUT_AUX_DEFINITION(Graph)
ADD_CUT_AUX_DEFINITION(Edge)


#undef ADD_CUT_AUX_DEFINITION
#undef ADD_AUX_DEFINITION
#undef CUT_AUX_DEFINITION
    

#endif
