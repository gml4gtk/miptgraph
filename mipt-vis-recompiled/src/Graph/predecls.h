/**
 * File: Graph/predecls.h - Predeclarations for interface of Graph library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GRAPH_PREDECLS_H
#define GRAPH_PREDECLS_H

#include <list>
using namespace std;

class Graph;
class Node;
class Edge;

/** Number type used for numbering nodes and edges in graph */
typedef unsigned int GraphNum;

#include "../Utils/utils_iface.h"
#include "num.h"
#include "marker.h"
#include "graph_error.h"
#include <libxml/xmlreader.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

/** 
 * namespaces import
 */
using namespace Utils;

/**
 * Directions type in graph
 */
enum GraphDir
{
    GRAPH_DIR_UP,
    GRAPH_DIR_DOWN,
    GRAPH_DIRS_NUM
};

/**
 * Return direction that is reverse to given one
 */
inline GraphDir
revDir( GraphDir dir)
{
    graphassert( GRAPH_DIRS_NUM == 2);
    return ( dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN: GRAPH_DIR_UP; 
}

const GraphNum GRAPH_MAX_NODE_NUM  = ( GraphNum)( -1);
const GraphNum GRAPH_MAX_EDGE_NUM  = ( GraphNum)( -1);

typedef ListItem< Node> NodeListItem;
typedef ListItem< Edge> EdgeListItem;

#endif 