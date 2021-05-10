/** 
 * @file: Node.cpp
 */
#include "predecls.h"
#include "Graph.h"
#include "Node.h"
#include "Edge.h"

void Node::Dump()
{
	list<pEdge>::iterator edge_iter;
	printf("Node %d:\n", m_id);
	printf("  In  edges:");
	for(edge_iter = m_in_edges_list.begin();
		edge_iter !=m_in_edges_list.end();
		edge_iter++) {
			pEdge pe = *edge_iter;
			printf(" ");
			pe->Print();
	}
	printf("\n  Out edges:");
	for(edge_iter = m_out_edges_list.begin();
		edge_iter !=m_out_edges_list.end();
		edge_iter++) {
			pEdge pe = *edge_iter;
			printf(" ");
			pe->Print();
	}
	printf("\n");
}

Node::Node(pGraph graph)
{
	assert(graph);
	m_graph = graph;
	m_id = graph->next_node_id;
	graph->m_total_nodes_num++;
	graph->next_node_id++;
	graph->m_nodes_list.push_back(this);
}

