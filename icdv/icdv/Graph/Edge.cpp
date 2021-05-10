/** 
 * @file: Edge.cpp
 */
#include "predecls.h"

Edge::Edge(pNode from, pNode to):
	m_from(from),
	m_to(to)
{
	assert(from);
	assert(to);
	assert(from->m_graph == to->m_graph);

        reverse = false;
	pGraph pg = from->m_graph;
	assert(pg);

	m_id = pg->next_edge_id;
	pg->m_total_edges_num++;
	pg->next_edge_id++;
	pg->m_edges_list.push_back(this);

	m_graph = pg;

	from->m_out_edges_list.push_back(this);
	to->m_in_edges_list.push_back(this);
};

void Edge::Reverse(){
	pNode temp_node;

	// Swaping from<->to nodes for the edges;
	temp_node = m_to; 
	m_to = m_from;
	m_from = temp_node;

	// Swaping in<->out edges for the nodes;
	m_to->m_in_edges_list.push_back(this);
	m_from->m_out_edges_list.push_back(this);
	m_to->m_out_edges_list.remove(this);
	m_from->m_in_edges_list.remove(this);

        reverse = true;
}

void Edge::Dump() {
	printf("Edge %d: %d-->%d rev= %d\n", m_id, m_from->m_id, m_to->m_id, reverse);
};

void Edge::Print() {
	printf("%d->%d", m_from->m_id, m_to->m_id);
};

