/**
 * File: node.cpp - Node class implementation, part of Graph library, 
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"

/** 
 * Destructor. Corrects list of nodes in corresponding graph and deletes adjacent edges
 */
Node::~Node()
{
	out ("deleting node# "); debugPrint();
	out ("\n\n\n");

    Edge *edge;
    
    /** delete incidient edges */
    for ( edge = firstSucc(); isNotNullP( edge);)
    {
        Edge* next = edge->nextSucc();
        edge->detachFromNode( GRAPH_DIR_DOWN);// Edge is detached from succ node
        delete edge;
        edge = next;
    }
    for ( edge = firstPred(); isNotNullP( edge);)
    {
        Edge* next = edge->nextPred();
        edge->detachFromNode( GRAPH_DIR_UP);// Edge is detached from pred node
        delete edge;
        edge = next;
    }
    /** delete myself from graph */
    graph->detachNode( this);
}

/**
 * Add an edge to this node in specified direction
 */
void
Node::addEdgeInDir( Edge *edge, GraphDir dir)
{
    graphassert( isNotNullP( edge));
    EdgeListItem *it = edge->getNodeIt( revDir( dir));
    it->attach( edges[ dir]);
    edges[ dir] = it;
}

/**
 * delete edge pointed by iterator in specidied direction
 */
void
Node::deleteEdgeInDir( GraphDir dir, EdgeListItem* it)
{
    graphassert( isNotNullP( it));
    if( edges[ dir] == it)
    {
        edges[ dir] = it->next();
    }
    if( e_it[ dir] == it)
    {
        e_it[ dir] = it->next();
    }
    it->detach();
}

/**
 * Print node in Dot format to stdout
 */
void
Node::debugPrint()
{
    out("%u;", id());
}

/**
 *  Write to xml
 */
void
Node::writeByXmlWriter( xmlTextWriterPtr writer)
{
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%d", userId());
}

/**
 *  Read from xml
 */
void Node::readByXml (xmlNode * cur_node)
{
	for (xmlAttr * props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup("id")))
		{
			setUserId( strtoul( ( const char *)( props->children->content), NULL, 0));
			if (userId() > graph->maxNodeId())	graph->setMaxNodeId( userId());
		}
	}
}