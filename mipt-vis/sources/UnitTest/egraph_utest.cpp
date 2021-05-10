/**
 * File: egraph_utest.cpp - implementation and definition of functions 
 * to be tested in UnitTest, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#include "utest_impl.h"

static bool uTestMakeEntryExit(void)
{
	bool utest_passed = true;
    EGraph *egraph = new EGraph;
	vector<ENode *> enodes;
	printf("\n  Creating graph 1:\n");
	for ( int i =0; i<6; i++)
    {
        enodes.push_back( egraph->newNode());
	}
    egraph->newEdge( enodes[ 0], enodes[ 1]);
    egraph->newEdge( enodes[ 2], enodes[ 3]);
    egraph->newEdge( enodes[ 2], enodes[ 4]);
    egraph->newEdge( enodes[ 3], enodes[ 5]);
    egraph->newEdge( enodes[ 4], enodes[ 5]);

    egraph->debugPrint();

	if(!egraph->isGraphConnected())
	{  printf("Graph is NOT connected\n");
	} else
	{	printf("Graph IS connected\n");
	}

	printf("  Creating single-entry and single-exit\n");
	egraph->makeGraphSingleExit();
    egraph->makeGraphSingleEntry();

	printf("    Entry:\n");
	egraph->getEntry()->debugPrint();
	printf("    Exit:\n");
	egraph->getExit()->debugPrint();

	printf("  Final Graph:\n");
    egraph->debugPrint();
    
	/* The following code checks correctness only of makeGraphSingleEntry.
	 * TODO: It's needed to check if makeGraphSingleExit also works fine.
	 */
	if(!egraph->isGraphConnected())
	{ 
		utest_passed = false;
	    printf("Graph is NOT connected\n");
	}
	else
	{	
		printf("Graph IS connected\n");
	}

	printf("  Deleting graph1...\n");
	delete egraph;
	enodes.clear();

	printf("\n  Creating graph 2:\n");
	egraph = new EGraph;
	for ( int i =0; i<4; i++)
    {
        enodes.push_back( egraph->newNode());
	}

    egraph->newEdge( enodes[ 0], enodes[ 1]);
    egraph->newEdge( enodes[ 1], enodes[ 0]);
    egraph->newEdge( enodes[ 2], enodes[ 3]);
    egraph->newEdge( enodes[ 3], enodes[ 2]);

    egraph->debugPrint();

	if(!egraph->isGraphConnected())
	{  printf("Graph is NOT connected\n");
	} else
	{	printf("Graph IS connected\n");
	}

	printf("  Creating single-entry and single-exit\n");
	egraph->makeGraphSingleExit();
    egraph->makeGraphSingleEntry();

	printf("    Entry:\n");
	egraph->getEntry()->debugPrint();
	printf("    Exit:\n");
	egraph->getExit()->debugPrint();

	printf("  Final Graph:\n");
    egraph->debugPrint();

	/* The following code checks correctness only of makeGraphSingleEntry.
	 * TODO: It's needed to check if makeGraphSingleExit also works fine.
	 */
	if(!egraph->isGraphConnected())
	{ 
		utest_passed = false;
	    printf("Graph is NOT connected\n");
	}
	else
	{	
		printf("Graph IS connected\n");
	}

	printf("\nSingle-entry, single-exit unit test %s\n", 
		   utest_passed ? "successfully passed" : "failed");
    return utest_passed;
}

/**
 * Unit tests for EGraph functionalities
 */
bool uTestEGraph()
{
	printf("\nBegin EGraph Test\n");
    if ( !uTestMakeEntryExit())
        return false;
    
	printf("\nEnd Of EGraph Test\n");
	
	return true;
}

bool uTestDomSearch()
{
	printf("Testing dominators search\n");
	/* Graph with one branching */
	EGraph* graph = new EGraph;
	vector< ENode*> nodes;
	for( int i = 0; i < 7; i++)
	{
		nodes.push_back( graph->newNode());
	}
	graph->newEdge( nodes[0], nodes[1]);
	graph->newEdge( nodes[1], nodes[2]);
	graph->newEdge( nodes[1], nodes[3]);
	graph->newEdge( nodes[2], nodes[4]);
	graph->newEdge( nodes[3], nodes[5]);
	graph->newEdge( nodes[4], nodes[6]);
	graph->newEdge( nodes[5], nodes[6]);

	graph->domSearch();
	if( nodes[1]->getidom() != nodes[0])
		return false;
	if( nodes[2]->getidom() != nodes[1])
		return false;
	if( nodes[3]->getidom() != nodes[1])
		return false;
	if( nodes[4]->getidom() != nodes[2])
		return false;
	if( nodes[5]->getidom() != nodes[3])
		return false;
	if( nodes[6]->getidom() != nodes[1])
		return false;
	/* Empty graph */
	EGraph* graph2 = new EGraph;
	graph2->domSearch();
	for( ENode* v = graph2->firstNode(); v != NULL; v = v->nextNode())
	{
		if( v->getidom() != NULL)
			return false;
	}
	/* Graph with 2 branches and one cycle */
	EGraph* graph3 = new EGraph;
	vector< ENode*> nodes3;
	for(int i = 0; i < 9; i++)
	{
		nodes3.push_back( graph3->newNode());
	}
	graph3->newEdge(nodes3[0], nodes3[1]);
	graph3->newEdge(nodes3[1], nodes3[2]);
	graph3->newEdge(nodes3[1], nodes3[3]);
	graph3->newEdge(nodes3[2], nodes3[4]);
	graph3->newEdge(nodes3[3], nodes3[5]);
	graph3->newEdge(nodes3[3], nodes3[7]);
	graph3->newEdge(nodes3[4], nodes3[6]);
	graph3->newEdge(nodes3[5], nodes3[6]);
	graph3->newEdge(nodes3[6], nodes3[8]);
	graph3->newEdge(nodes3[7], nodes3[8]);
	graph3->newEdge(nodes3[8], nodes3[1]);
	
	graph3->domSearch();
	if( nodes3[1]->getidom() != nodes3[0])
	{
		out( "Node 1 - FAIL");
		return false;
	}
	if( nodes3[2]->getidom() != nodes3[1])
	{
		out( "Node 2 - FAIL");
		return false;
	}
	if( nodes3[3]->getidom() != nodes3[1])
	{
		out( "Node 3 - FAIL");
		return false;
	}
	if( nodes3[4]->getidom() != nodes3[2])
	{
		out( "Node 4 - FAIL");
		return false;
	}
	if( nodes3[5]->getidom() != nodes3[3])
	{
		out( "Node 5 - FAIL");
		return false;
	}
	if( nodes3[6]->getidom() != nodes3[1])
	{
		out( "Node 6 - FAIL");
		return false;
	}
	if( nodes3[7]->getidom() != nodes3[3])
	{
		out( "Node 7 - FAIL");
		return false;
	}
	if( nodes3[8]->getidom() != nodes3[1])
	{
		out( "Node 8 - FAIL");
		return false;
	}
	out( "\nSearching dominators succeded\n");
	return true;
}
