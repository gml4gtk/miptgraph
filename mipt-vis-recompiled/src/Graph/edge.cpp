/**
 * File: edge.cpp - Edge class implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

/**
 * Edge destructor.
 * delete edge from graph's list of edges
 */

Edge::~Edge()
{
	out ("deleting edge: "); debugPrint();
	out ("\n\n\n");

	detachFromNode (GRAPH_DIR_UP);
	detachFromNode (GRAPH_DIR_DOWN);
	graph->detachEdge( this);
}

/**
 * Print edge in DOT format to stdout
 */

void
Edge::debugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements graph states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
	int predid = -1;
	int succid = -1;
	if (isNotNullP( pred())) predid = pred()->id();
	if (isNotNullP( succ())) succid = succ()->id();
//    graphassert( isNotNullP( pred()));
//    graphassert( isNotNullP( succ()));

    out("%u->%u;", predid, succid);
}

/**
 * Low level correction of node's edge list in corresponding direction
 */

void
Edge::detachFromNode( GraphDir dir)
{
    Node *n = getNode( dir);
    if (n) n->deleteEdgeInDir( revDir( dir), &n_it[ dir]);
	nodes[dir] = 0;
	
}

/**
 * Write edge by xml writer
 */
void Edge::writeByXmlWriter( xmlTextWriterPtr writer)
{
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%d", id());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "from", "%d", pred()->userId());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "to", "%d", succ()->userId());

}
/**
 *  Read from xml
 */
void Edge::readByXml (xmlNode * cur_node)
{
	for(xmlAttr * props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup("id")))
		{
			setUserId( strtoul( ( const char *)( props->children->content), NULL, 0) );
		}
	}
}