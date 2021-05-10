/**
 * File: graph_uTest.cpp - Implementation of testing of Graph library, 
 * internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"
#include <list>
#include <vector>
#include "direct.h"
#include <string.h>


using namespace std;

/*** Simple joint testing for markers/numerations */
static bool uTestDFS( Graph *graph)
{
    Numeration n = graph->newNum();
    NodeListItem* nodes = graph->DFS( n);
    graph->freeNum( n);
    return true;
}

static bool uTestBFS( Graph *graph)
{
	//if ( !graph->BFS())
    //    return false;
	//return true;
	Numeration n = graph->newNum();
    NodeListItem* nodes = graph->BFS( n);
    graph->freeNum( n);
    return true;
}

 /**
  * TODO: Check graph's data structures being consistent with node and edge functionality
  */
static bool uTestGraphOwn()
{
    return true;
}
/**
 * TODO: Check consistency of Node and Edge classes interoperation
 */
static bool uTestNodeEdge()
{
    /**
     * TODO: Check that node and edge remain correct after basic edge/node creation/deletion
     */
    /**
     * 
     */
    return true;
}

/**
 * Check marker functionality
 */
static bool uTestNumerations()
{
    /** 
     * Every class that can be a numerations manager should implement
     * the routine for clearing numerations in objects
     */
    class NumMgrInst: public NumManager
    {
        /** Implementation of clearing - empty TODO: implement and test it */
        void clearNumerationsInObjects()
        {
        
        }
    };
    /**
     * Check correct error reporting
     *  1. Too many numerations
     */
    NumMgrInst mgr1;
    try
    {
        for ( int i = 0; i < MAX_NUMERATIONS + 1; i++)
        {
            mgr1.newNum();
        }
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        assert( error == NUM_ERROR_OUT_OF_INDEXES);
    }

    /** 2. Too big number */
    NumMgrInst mgr2;
    Numeration num2 = mgr2.newNum();
    Numbered obj2; 
    try
    {
        obj2.setNumber( num2, -1);
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        assert( error == NUM_ERROR_NUMBER_OUT_OF_RANGE);
    }
    mgr2.freeNum( num2);

    /** 3. Functional testing */
    NumMgrInst mgr;
    for ( int i = 0; i < MAX_NUMERATIONS + 2; i++)
    {
        Numeration n = mgr.newNum();
        mgr.freeNum( n);
    } 
    Numeration num = mgr.newNum();
    Numeration num_unused = mgr.newNum();
    Numbered obj; 
    assert( obj.number( num) == NUMBER_NO_NUM);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.setNumber( num, 1);
    assert( obj.isNumbered( num));
    assert( obj.number( num) == 1);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.unNumber( num);
    assert( obj.number( num) == NUMBER_NO_NUM);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);    
    return true;
}

/**
 * Check marker functionality
 */
static bool uTestMarkers()
{
    Graph graph;
    Node *dummy = graph.newNode();
    delete dummy;
    Node *pred = graph.newNode();
    Node *succ = graph.newNode();
    Edge *edge = graph.newEdge( pred, succ);
    Marker m = graph.newMarker();
    Marker m2 = graph.newMarker();

    Marker m_array[ MAX_GRAPH_MARKERS];
    
    assert( !pred->isMarked( m));
    assert( !succ->isMarked( m));
    assert( !edge->isMarked( m));
    assert( !pred->isMarked( m2));
    
    pred->mark( m);
    succ->mark( m);
    edge->mark( m);
    edge->mark( m2);

    assert( pred->isMarked( m));
    assert( succ->isMarked( m));
    assert( edge->isMarked( m));
    assert( edge->isMarked( m2));
    edge->unmark( m);

    /** Check that different markers have different behaviour */
    assert( edge->isMarked( m2));
    assert( !edge->isMarked( m));
    
    graph.freeMarker( m);
    graph.freeMarker( m2);
    
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        m_array [ i] = graph.newMarker();
    }
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        graph.freeMarker( m_array[ i]);
    }
    m = graph.newMarker();
    graph.freeMarker( m);
    
    Node *n;
    for (  n = graph.firstNode(); isNotNullP( n); )
    {
		Node *next = n->nextNode();
		delete n;
		n = next;
    }
    return true;
}


/**
 * Test for BFS
 */
bool uTestforBFS()
{
	Graph graph;
	vector<Node *> nodes;
	/**
	 *Create nodes and edges
	 */

	for ( int i =0; i<20; i++)
    {
        nodes.push_back( graph.newNode());
        if ( i > 0)
        {
            graph.newEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.newEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    graph.newEdge( nodes[ 8], nodes[ 4]);
    delete nodes[ 8];
    graph.debugPrint();

	printf("\n");
	if ( !uTestBFS( &graph))
		return false;
	printf("\n");
    return true;
}

/**
 * Unit tests for Graph library
 */
bool uTestGraph()
{
    Graph graph;

    /** 
     *  Check basic operation of graph library
     */
    vector<Node *> nodes;

    /** Create nodes and edges */
    for ( int i =0; i<20; i++)
    {
        nodes.push_back( graph.newNode());
        if ( i > 0)
        {
            graph.newEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.newEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    graph.newEdge( nodes[ 8], nodes[ 4]);
    delete nodes[ 8];
    graph.debugPrint();
    

    /**
     * Check graph's data structures consistency
     */
     if ( !uTestGraphOwn())
         return false;
    /**
     * Check node-edge consistency
     */
    if ( !uTestNodeEdge())
         return false;

    /**
     * Check markers
     */
    if ( !uTestMarkers())
        return false;

    /**
     * Check numerations 
     */
    if ( !uTestNumerations())
        return false;
   
    /**
     * Simple joint testing of graph's functionality/markers/numerations
     */
    if ( !uTestDFS( &graph))
        return false;

    //assert<Error>( 0);
    return true;
}


bool uTestGui( int argc, char * argv[])
{
    QApplication app( argc, argv);
/*
    char * file = _getcwd( NULL, 1024);
    strcat_s( file, 1024, "/test_graph2.xml");
    
	
    //GuiGraph * graph = new GuiGraph( file);
    GuiGraph * graph = new GuiGraph();
    
    if ( graph == NULL) return false;

    graph->debugPrint();
*/
    /** Test Scene with nodes */
    showScene( argc, argv, &app);

    /** Test writing to xml */

/*
    file = _getcwd( NULL, 1024);
    strcat_s( file, 1024,"/test_out.xml");
    graph->writeToXML( file);

    delete graph;
*/
    return true;
}