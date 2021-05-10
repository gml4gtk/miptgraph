/** 
 * @file: Deriv.cpp
 * Files Deriv.* contain an example of how one could expand class Graph, 
 * deriving a new class from it.
 * In this file we'll create class EGraph, representing extended graph. Instead
 * of ordinary nodes (class Node) and edges (class Edge) it will be consisted
 * of nodes and edges with additional fields: ENode and EEdge.
 *
 * This file contains implementations.
 */
#include "Deriv.h"

/**
 */
void EGraph::FreeNode(pNode p)
{
	assert(p != NULL);
	
	// We should cast p to pENode, because otherwise ~Node will be called instead of ~ENode.
	// It will lead to memory leakages.
	delete (pENode)p;
}

void EGraph::FreeEdge(pEdge p)
{
	assert(p != NULL);
	delete (pEEdge)p;
}

pENode EGraph::AddNode()
{
	pENode new_node = new ENode(this);
	return new_node;
}

pEEdge EGraph::AddEdge(pNode from, pNode to)
{
	pEEdge new_edge = new EEdge((pENode)from,(pENode)to);
	return new_edge;
}

void ENode::Dump()
{
	Node::Dump();
	printf("  X,Y: %d,%d\n",x,y);
}

void EEdge::Dump()
{
	printf("Edge %d(w = %d): %d-->%d\n", id(), w, from()->id(), to()->id());
}

/**
 * Create a EGraph object and call some of its methods.
 */
void Derivation_Example()
{
	printf("\nDerivation test started..\n");

	EGraph g;

	int len = 10;
	pENode *p = new pENode[len];
	
	// Creating a new extended graph
	for(int i = 0; i < len; i++) {
		p[i] = (pENode)g.AddNode();
	}
	for(int i = 0; i < len-1; i++) {
		g.AddEdge(p[i], p[i+1]);
	}
	for(int i = 2; i < len; i+=3) {
		g.AddEdge(p[i], p[i-2]);
	}
	g.Dump();

	// Performing some transformations
	g.DeleteNode(p[5]);
	g.DeleteNode(p[7]);
	g.DeleteEdge(p[3],p[4]);

	g.Dump();

	// Clean up
	g.Destroy();
	delete []p;

	printf("Derivation test passed!\n");
}



