/**
 * File: graph.cpp - Graph class implementation, part of
 * Graph library, internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "graph_impl.h"
#include <deque>
#include <stack>

/**
 *  Initialize
 */
void Graph::init()
{
    node_next_id = 0;
    edge_next_id = 0;
    node_num = 0;
    edge_num = 0;
	first_node = NULL;
    first_edge = NULL;
    xml_doc = NULL;
}

/**
 * Constructor.
 */
Graph::Graph()
{
	init();
}
/**
 * Allocation of memory for Edge
 */
Edge *Graph::createEdge( Node *pred, Node *succ)
{
    return new Edge( this, incEdgeId(), pred, succ);
}
/**
 * Allocation of memory for Node
 */
Node *Graph::createNode()
{
    return new Node( (Graph *)this, incNodeId());
}

/**
 * Creation node in graph
 */
Node * 
Graph::newNode()
{
    /**
     * Check that we have available node id 
     */
    graphassert( node_next_id < GRAPH_MAX_NODE_NUM);
    Node *node_p = ( Node *) createNode();
    NodeListItem* it = node_p->getGraphIt();
	
	/* Attach new node to the list */
	if ( isNotNullP( first_node))
	{
		it->attach( first_node->getGraphIt());
	}
    
	first_node = node_p;
    node_num++;
    return node_p;
}


/**
* Insert new node on edge in graph
*/
void Graph::removeEdge (Edge* e)
{
	e->detachFromNode (GRAPH_DIR_UP);
	e->detachFromNode (GRAPH_DIR_DOWN);
	detachEdge (e);
}
/**
* Insert new node on edge in graph
*/
void Graph::removeNode (Node* n)
{
    Edge *edge;
    
    /** delete incidient edges */
    for ( edge = n->firstSucc(); isNotNullP( edge);)
    {
        Edge* next = edge->nextSucc();
        edge->detachFromNode( GRAPH_DIR_DOWN);// Edge is detached from succ node
        removeEdge (edge);
        edge = next;
    }
    for ( edge = n->firstPred(); isNotNullP( edge);)
    {
        Edge* next = edge->nextPred();
        edge->detachFromNode( GRAPH_DIR_UP);// Edge is detached from pred node
        removeEdge (edge);
        edge = next;
    }
    /** delete myself from graph */
    detachNode (n);
}
/**
* Insert new node on edge in graph
*/

Node * Graph::insertNodeOnEdge( Edge* e)
{
	Node* n = this->newNode();
	Node* succ = e->succ();
	e->setSucc( n);
	this->newEdge( n, succ);
	return n;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
Edge * 
Graph::newEdge( Node * pred, Node * succ)
{
    /**
     * Check that we have available edge id 
     */
    graphassert( edge_next_id < GRAPH_MAX_NODE_NUM);
    Edge *edge_p = ( Edge *) createEdge( pred, succ);
    EdgeListItem* it = edge_p->getGraphIt();
	
	if( isNotNullP( first_edge))
	{
		it->attach( first_edge->getGraphIt());
	}
    first_edge = edge_p;
    edge_num++;
    return edge_p;
}

/**
 * Print graph to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
void 
Graph::debugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->debugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->debugPrint();
    }
    out( "}");
}

/**
  * Recursive implementation of depth-first search visit routine
  */
static inline NodeListItem*
dfsVisitRec( Node* node,
             NodeListItem* item,
             Marker m,
             Numeration n,
             GraphNum* number)
{
    /** attach the element to the list with already visited elements*/
    NodeListItem* new_item = new NodeListItem( item, LIST_DIR_RDEFAULT, node);
    Edge *e;

    /** mark node to prevent search from visiting it again */
    node->mark( m);
    node->setNumber( n, *number);
    *number = (*number) + 1;

    /** Visit Succs skipping already visited */
    for ( e = node->firstSucc(); !node->endOfSuccs(); e = node->nextSucc())
    {
        Node* succ = e->succ();
        if ( !succ->isMarked( m))
        {
            /** Continue DFS recursively with the successor */
            new_item = dfsVisitRec( succ, new_item, m, n, number);
        }
    }
    return new_item;
}

/**
 * Implementation of depth-first search visit routine with using a stack
 */
static inline NodeListItem*
dfsVisitStack( Node* node,
             NodeListItem* item,
             Marker m,
             Numeration n,
             GraphNum* number)
{
    /** attach the element to the list with already visited elements*/
    NodeListItem* new_item = new NodeListItem( item, LIST_DIR_RDEFAULT, node);
    Edge *e;
	stack<Node *> st;
	st.push( node);
	while ( !st.empty())
	{
		/** Execute the node from a stack 
         * If the top of stack isn't the node without predecessors,
	     * then current node is the last successor of its predecessor
	     */
		node = st.top();
		st.pop();
        /** Mark node to prevent search from visiting it again */
        node->mark( m);
        node->setNumber( n, *number);
        *number = (*number) + 1;

        /** Visit Succs skipping already visited */
        for ( e = node->firstSucc(); !node->endOfSuccs(); e = node->nextSucc())
        {
            Node* succ = e->succ();
            if ( !succ->isMarked( m))
            {
                /** Writing Succs in a stack */
				st.push( succ);
                
            }
        }
	}
    return new_item;
}

/**
 * Implementation of breadth-first search. Starts from node without predecessors.
 * Fuction return last node without predessors
 */
NodeListItem* Graph::BFS( Numeration num)
{
	deque<Node*> q;    //queue with two ends
    GraphNum number = 0;
    NodeListItem *item = NULL;
	Node *child;
	Node *n;
	Edge *e;
	/** Create a marker to distinguish visited nodes from unvisited ones */
    Marker m = newMarker();
	
	for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        /** 
         * If firstPred is Null, then the node n has no predecessors, =>
         * we should start BFS from this node.
         */

    if( isNullP( n->firstPred()))
        {
            graphassert( !n->isMarked( m));
			item = new NodeListItem( item, LIST_DIR_RDEFAULT, n);
            /** 
             * Start BFS from node n, using marker m and numeration num.
             * item is a list that at the end will contain all nodes in BF order.
             * number is the number of the last visited node.
             */
            q.push_back( n);
	       while ( !q.empty())
	       {
			   n = q.front();
			   q.pop_front();
			   n->setNumber( num,number);
		       number = ( number) + 1;
		       n->mark( m);
		       for ( e = n->firstSucc(); !n->endOfSuccs(); e = n->nextSucc())
		       {
			       child = e->succ();
			       if ( !child->isMarked( m)) continue;
			       q.push_back( child);
			   }
		   }
		}
	}      
	freeMarker( m);
	return item;
}

/**
 * Implementation of depth-first search. Starts from nodes without predecessors.
 */
NodeListItem* Graph::DFS( Numeration num)
{
    Node *n;
    GraphNum number = 0;
    NodeListItem *item = NULL;
    /** Create a marker to distinguish visited nodes from unvisited ones */
    Marker m = newMarker();

    /** 
     * Visit all nodes and call dfsVisitRec for each node that doesn't have predecessors.
     * The other nodes are skipped here, but they will be visited in dfsVisitRec.
     * If the graph is connected, we'll call dfsVisitRec only once - for the start node.
     */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        /** 
         * If firstPred is Null, then the node n has no predecessors, =>
         * we should start DFS from this node.
         */
        if( isNullP( n->firstPred()))
        {
            graphassert( !n->isMarked( m));
            /** 
             * Start DFS from node n, using marker m and numeration num.
             * item is a list that at the end will contain all nodes in DF order.
             * number is the number of the last visited node.
             */
            /** 
             * Start DFS using a stack
             */
            item = dfsVisitStack( n, item, m, num, &number);
            /** 
             * Start DFS using recursion
             */
			// item = dfsVisitRec( n, item, m, num, &number);
        }
    }   
    /** 
     * TODO: introduce assert(all nodes have been visited) 
     */

    freeMarker( m);

    return item;
}

/**
 *  Init graph nodes
 */
void
Graph::readNodesFromXmlDoc( xmlNode * a_node)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("node")))
		{
			Node *node = newNode();
			node->readByXml (cur_node);
		}
	}
}



/**
 *  Init graph edges
 */
void
Graph::readEdgesFromXmlDoc( xmlNode * a_node, vector<Node *> nodes)
{
	xmlNode * cur_node;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if ( cur_node->type == XML_ELEMENT_NODE
			 && xmlStrEqual( cur_node->name, xmlCharStrdup("edge")))
		{
			Edge *edge;
			xmlAttr * props;
			int from = -1, to = -1;

			/** Find 'from' and 'to' */
			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("from")))
				{
					from = strtoul( ( const char *)( props->children->content), NULL, 0);
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("to")))
				{
					to = strtoul( ( const char *)( props->children->content), NULL, 0);
				}
			}
			if ( from == -1 || to == -1) continue;
			edge = newEdge( nodes[from], nodes[to]);
			/** Parse other properties */
			edge->readByXml (cur_node);
		}
	}
}

/**
 * Initializes graph form xmlDoc root node
 */
void
Graph::readFromXmlDoc( xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
	vector<Node *> nodes;
	bool is_found = false;

    for (cur_node = a_node; cur_node && !is_found; cur_node = cur_node->next)
	{
		if( cur_node->type == XML_ELEMENT_NODE
			&& xmlStrEqual( cur_node->name, xmlCharStrdup("graph")))
		{
			xmlAttr * props;
			Node * node;

			for( props = cur_node->properties; props; props = props->next)
			{
				if ( xmlStrEqual( props->name, xmlCharStrdup("default_node_size")))
				{
					setDefaultNodeSize( strtoul( ( const char *)( props->children->content), NULL, 0));
				} else if ( xmlStrEqual( props->name, xmlCharStrdup("name")))
				{
					setName( ( char *)( props->children->content));
				}
			}

			readNodesFromXmlDoc( cur_node->children);
			nodes.insert(nodes.end(), ( max_node_id + 1), (Node *)NULL);
			for ( node = firstNode(); isNotNullP( node); node = node->nextNode())
				    nodes[ node->userId()] = node;

			readEdgesFromXmlDoc( cur_node->children, nodes);

			readAttribsFromXml (cur_node->children);
			is_found = true;
		}
    }
	nodes.clear();
}

/**
 *  Build graph from xml file.
 */
void
Graph::readFromXml(const char *filename)
{
    xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

     /** This initializes the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* Parse the file and get the DOM */
    doc = xmlReadFile(filename, NULL, 0);

    if (doc == NULL)
        printf("error: could not parse file %s\n", filename);

    /* get the root element node */
    root_element = xmlDocGetRootElement(doc);

	readFromXmlDoc( root_element);

	this->xml_doc = doc;

	/* Cleanup function for the XML library */
    xmlCleanupParser();
}

/**
 *  Constructor by XML filename
 */
Graph::Graph( char * filename)
{
	init();
	readFromXml( filename);
}

/**
 *  Write nodes
 */
void
Graph::writeNodesByXmlWriter( xmlTextWriterPtr writer)
{
	Node * node;
	for ( node = firstNode(); isNotNullP( node); node = node->nextNode())
	{
		xmlTextWriterWriteString( writer, BAD_CAST "\t");
		xmlTextWriterStartElement( writer, BAD_CAST "node");

		node->writeByXmlWriter( writer);

		xmlTextWriterEndElement( writer);
	    xmlTextWriterWriteString( writer, BAD_CAST "\n");
	}
}

/**
 *  Write edges
 */
void
Graph::writeEdgesByXmlWriter( xmlTextWriterPtr writer)
{
	Edge * edge;
	for ( edge = firstEdge(); isNotNullP( edge); edge = edge->nextEdge())
	{
		xmlTextWriterWriteString( writer, BAD_CAST "\t");
		xmlTextWriterStartElement( writer, BAD_CAST "edge");

		edge->writeByXmlWriter( writer);

	    xmlTextWriterWriteString( writer, BAD_CAST "\n");
		xmlTextWriterWriteString( writer, BAD_CAST "\t");
		xmlTextWriterEndElement( writer);
		xmlTextWriterWriteString( writer, BAD_CAST "\n");
	}
}

/**
 *  Write graph to xml file
 */
void
Graph::writeToXml( const char *filename)
{
	xmlTextWriterPtr writer;


    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterFilename( filename, 0);
    assert( writer != NULL);
    
    xmlTextWriterStartDocument( writer, NULL, NULL, NULL);
	xmlTextWriterStartElement( writer, BAD_CAST "graph");
	xmlTextWriterWriteString( writer, BAD_CAST "\n");

	xmlTextWriterWriteAttribute( writer, BAD_CAST "name", BAD_CAST name());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "default_node_size",
		                               "%d", defaultNodeSize());

	writeNodesByXmlWriter( writer);
    writeEdgesByXmlWriter( writer);
	writeAttribsByXmlWriter (writer);

	xmlTextWriterEndElement( writer);

	xmlTextWriterEndDocument( writer);


	xmlFreeTextWriter( writer);
}
/**
 * Implementation for numerations cleanup
 */
void
Graph::clearNumerationsInObjects()
{
    Node *n;
    Edge *e;

    /** Clean markers in nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedNumerations( static_cast<Numbered *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedNumerations( static_cast<Numbered *>(e));
    }
}

/**
 * Implementation for markers cleanup
 */
void
Graph::clearMarkersInObjects()
{
    Node *n;
    Edge *e;

    /** Clean markers in nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(e));
    }
}

/**
 *  Destructor of graph class
 */
Graph::~Graph()
{
    /* Free the document */
    if ( xml_doc != NULL)xmlFreeDoc( xml_doc);

}