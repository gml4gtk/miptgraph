/**
 * @file: Deriv.h
 * Files Deriv.* contain an example of how one could expand class Graph, 
 * deriving a new class from it.
 * In this file we'll create class EGraph, representing extended graph. Instead
 * of ordinary nodes (class Node) and edges (class Edge) it will be consisted
 * of nodes and edges with additional fields: ENode and EEdge.
 *
 * This file contains declarations.
 */
#ifndef DERIV_H
#define DERIV_H
#include "Graph/predecls.h"
#include <QtGui/QGraphicsTextItem>
#include <QtGui/QtGui>
#include <QtGui/QApplication>
#include <QtCore/QLineF>

class EGraph;
class ENode;
class EEdge;

typedef EGraph* pEGraph;
typedef ENode*  pENode;
typedef EEdge*  pEEdge;

/// Example of a class derived from Node
/**
 * This class contains two new variables: x,y.
 * Note that such class might be derived not only from class Node.
 * In this example, it's derived from Node and QGraphicsTextItem.
 */
class ENode: public Node, QGraphicsTextItem {
    public:
	int x,y; //< New fields
	ENode(pEGraph pg): Node((pGraph)pg){
	    x = y = 0;
	}
	virtual void Dump();
        friend class EEdge;
        friend class EGraph;
};

/// Example of a class derived from Edge
/**
 * This class contains one new variable: w.
 */
class EEdge: public Edge {
    public:
	int w; //< New field
	EEdge(pENode from, pENode to): Edge((pNode)from, (pNode)to){
	    w = 0;
	}
	virtual void Dump();
        friend class EGraph;
        friend class ENode;
};

/// Example of a class derived from Graph
/**
 * We need to reimplement functions for creating and deleting new nodes/edges, 
 * because we ought to use new types (ENode instead of Node and EEdge instead
 * of Edge).
 */
class EGraph: public Graph {
    public:
	virtual pENode AddNode();
	virtual pEEdge AddEdge(pNode from, pNode to);
	virtual void FreeNode(pNode p);
	virtual void FreeEdge(pEdge p);
        friend class EEdge;
        friend class ENode;
};

/// This function illustrates how to use class EGraph.
void Derivation_Example();
#endif
