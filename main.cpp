/** 
 * @file: main.cpp
 */

#include "Graph/predecls.h"
#include "Deriv.h"
#include "Layout/Layout.h"
#include "Gui/mainscene.h"
/**
 * Create a graph and call some of its methods.
 */

void Test_Graph()
{
	printf("\nBase graph test started..\n");

	Graph g;
	int num = 10;
	pNode *p = new pNode[num];
	for(int i = 0; i < num; i++) {
		p[i] = g.AddNode();
	}
	for(int i = 0; i < num-1; i++) {
		g.AddEdge(p[i], p[i+1]);
	}
	for(int i = 2; i < num; i+=3) {
		g.AddEdge(p[i], p[i-2]);
	}
	g.Dump();
	g.DeleteNode(p[5]);
	g.DeleteNode(p[7]);
	g.DeleteEdge(p[3],p[4]);
	g.AddEdge(p[3], p[6]);

	g.Dump();
	g.Destroy();
	delete []p;
	printf("Base graph test passed!\n");
}


void Run_Unit_Tests()
{
	// Test base graph
	Test_Graph();

	// Test derived graph
	Derivation_Example();
	
	//Layout tests
	LayoutTest();
}

/**
 * Main function
 */
int main(int argc, char **argv)
{
       Run_Unit_Tests();
	return 0;
}
