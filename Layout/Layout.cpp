/** 
 * @file: Layout.cpp
 */

#include "Layout.h"

/**
* Create a LGraph object and call some of its methods.
*/

void LayoutTest()
{
	printf("\nLayout test started..\n");

	LGraph g;

	int len = 10;
	pLNode *p = new pLNode[len];

	// Creating a new graph
	for(int i = 0; i < len; i++) {
		p[i] = (pLNode)g.AddNode();
	}

	// Performing some transformations
	g.AddEdge(p[0],p[1]);
	g.AddEdge(p[0],p[2]);
	g.AddEdge(p[0],p[3]);
	g.AddEdge(p[1],p[5]);
	g.AddEdge(p[2],p[6]);
	g.AddEdge(p[2],p[5]);
	g.AddEdge(p[3],p[4]);
	g.AddEdge(p[4],p[7]);
	g.AddEdge(p[4],p[9]); 
	g.AddEdge(p[5],p[8]);
	g.AddEdge(p[6],p[9]);
	g.AddEdge(p[1],p[8]);
	g.AddEdge(p[1],p[7]);
	g.AddEdge(p[1],p[6]);
	g.AddEdge(p[1],p[5]);
	g.AddEdge(p[1],p[9]);
	
	g.Layout();
	g.Dump();

	printf("maxrank = %i\n",g.getMaxrank());
	
	// Clean up
	g.Destroy();
	delete []p;

	printf("Layout test passed!\n");
}
