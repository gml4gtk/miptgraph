/** \file
 * \brief routines
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the root directory for details.
 *
 * \par
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "predecls.h"

Edge::Edge(pNode from, pNode to)
    : m_from(from)
    , m_to(to)
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

void Edge::Reverse()
{
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

void Edge::Dump()
{
    printf("Edge %d: %d-->%d rev= %d\n", m_id, m_from->m_id, m_to->m_id, reverse);
};

void Edge::Print()
{
    printf("%d->%d", m_from->m_id, m_to->m_id);
};
