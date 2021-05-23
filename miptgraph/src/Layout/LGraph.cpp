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

/**
 * @file: LGraph.cpp
 */

#include "Layout.h"

void LGraph::Layout(unsigned int number_of_iterations,
    bool do_transpose,
    int transpose_range)
{
    list<pEdge> ReverseEdges;

    /* avoid crash when graph has no nodes */
    if (m_total_nodes_num == 0) {
        return;
    }

    /* nothing to layout if there are no edges */
    if (m_total_edges_num == 0) {
        return;
    }

    FindReverseEdges(ReverseEdges);

    ReverseReverseEdges(ReverseEdges);

    InitRank();

    list<pEdge> LongEdges;
    FindLongEdges(LongEdges);

    AddDummyNodes(LongEdges);

    ReverseEdges.clear();

    order = new Ordering();
    order->order_vector = InitOrder();

    // Number of iterations.
    layouted = true;

    for (unsigned int i = 0; i < number_of_iterations; i++) {
        WeightedMedianHeuristic(i);
        /// swap nodes at this level, can be slow.
        if (do_transpose) {
            Transpose(i + transpose_range);
        }
    }

    InitPos(order);
    InitCoordinates(order);

    printf("Crossings:%d\n", countCrossing(order));

    return;
}

void LGraph::InitRank()
{
    // Calculating the rank for all Nodes
    for (list<pNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        unsigned int rank = ((LNode*)(*node_iter))->Rank();
        if (rank > maxrank)
            maxrank = rank;
    }
}

vector<vector<pLNode>>
LGraph::InitOrder()
{
    vector<vector<pLNode>> order(maxrank + 1);
    for (list<pNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        order[((pLNode)(*node_iter))->Rank()].push_back((pLNode)(*node_iter));
    }
    return order;
}

void LGraph::ReverseReverseEdges(list<pEdge>& ReverseEdges)
{
    for (list<pEdge>::iterator edge_iter = ReverseEdges.begin();
         edge_iter != ReverseEdges.end();
         edge_iter++)
        ((pLEdge)(*edge_iter))->Reverse();
}

void LGraph::FindLongEdges(list<pEdge>& LongEdges)
{
    for (list<pEdge>::iterator edge_iter = edges_list()->begin();
         edge_iter != edges_list()->end();
         edge_iter++) {
        if (((pLNode)(*edge_iter)->to())->Rank() - ((pLNode)(*edge_iter)->from())->Rank() > 1)
            LongEdges.push_back(*edge_iter);
    }
}

void LGraph::AddDummyNodes(list<pEdge>& LongEdges)
{
    for (list<pEdge>::iterator edge_iter = LongEdges.begin();
         edge_iter != LongEdges.end();
         edge_iter++) {
        ((pLEdge)(*edge_iter))->BreakLongEdge();
    }
}

/// Comparator to work with pointers
bool ComparePointer(pLNode node1, pLNode node2)
{
    return node1->getMedian() < node2->getMedian();
}

/**
 * scan graph and calculate barycenter values
 * then sort the new node order and check
 * if edge crossings reduced.
 */
void LGraph::WeightedMedianHeuristic(int iter)
{

    if (!layouted) {
        Layout(0, 0, 0);
    }

    Ordering temp_order;

    temp_order.order_vector = order->order_vector;

    // go from top to bottom or revesed
    if (iter % 2 == 0) {
        // scan from top to bottom of drawing
        for (unsigned int r = 1; r <= maxrank; r++) {
            for (unsigned int i = 0; i < temp_order.order_vector[r].size(); i++) {
                temp_order.order_vector[r][i]->median = temp_order.order_vector[r][i]->Median(*order, MEDIAN_IN);
            }
            /// Sort temp_order using ComparePointer comparator
            sort(temp_order.order_vector[r].begin(),
                temp_order.order_vector[r].end(),
                ComparePointer);
        }
    } else {
        // scan from bottom to top of drawing
        for (int r = maxrank - 1; r >= 0; r--) {
            for (unsigned int i = 0; i < temp_order.order_vector[r].size(); i++) {
                temp_order.order_vector[r][i]->median = temp_order.order_vector[r][i]->Median(*order, MEDIAN_OUT);
            }
            sort(temp_order.order_vector[r].begin(),
                temp_order.order_vector[r].end(),
                ComparePointer);
        }
    }

    // give nodes x pos based on pos in temp_order
    InitPos(&temp_order);

    // count edge crossings in whole graph
    int cross_temp_order = countCrossing(&temp_order);

    // give nodes x pos based on pos in order
    InitPos(order);

    // count edge crossings in whole graph
    int cross_order = countCrossing(order);

    // if edge crossings reduced, set order to temp_order
    // different order can have same amount of edge crossings
    // using <= is needed to keep the layout moving
    // this must be <= to get best results of the barycenter
    if (cross_temp_order <= cross_order) {
        order->order_vector = temp_order.order_vector;
        printf("edge crossings reduced from %d to %d\n", cross_order, cross_temp_order);
    } else {
        // did not improve, keep graph unchanged
        printf("edge crossings did not reduce from current %d to %d\n", cross_order, cross_temp_order);
    }

    return;
}

/**
 * Count edge crossings of the whole graph
 */
int LGraph::countCrossing(Ordering* order)
{
    int crossing = 0;
    // count crossings at every level
    for (unsigned int rank = 0; rank < maxrank - 1; rank++) {
        crossing += countCrossingOnRank(order, rank);
    }
    return crossing;
}

/**
 * Count edge crossings between rank level rank and rank+1
 */
int LGraph::countCrossingOnRank(Ordering* order, int rank)
{
    int crossing = 0;

    // Making list of all edges betwen rank and rank+1
    vector<pLEdge> edge_list;
    for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
        for (list<pEdge>::iterator edge_iter = order->order_vector[rank][i]->out_edges_list()->begin();
             edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
             edge_iter++) {
            edge_list.push_back((pLEdge)(*edge_iter));
        }
    }

    for (unsigned int i = 0; i < edge_list.size(); i++) {
        for (unsigned int j = i + 1; j < edge_list.size(); j++) {
            // Criterion of crossing edge_list[i] and edge_list[j]
            int cmp1 = ((pLNode)edge_list[i]->to())->pos - ((pLNode)edge_list[j]->to())->pos;

            int cmp2 = ((pLNode)edge_list[i]->from())->pos - ((pLNode)edge_list[j]->from())->pos;

            if ((cmp1 > 0 && cmp2 < 0) || (cmp1 < 0 && cmp2 > 0)) {
                crossing++;
            }
        }
    }
    return crossing;
}

/**
 * Give nodes initial x position based on index in order
 */
void LGraph::InitPos(Ordering* order)
{
    for (unsigned int rank = 0; rank <= maxrank; rank++) {
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            order->order_vector[rank][i]->pos = i;
        }
    }
    return;
}

void LGraph::InitCoordinates(Ordering* order,
    int normalwide,
    int dummywide,
    int vertical_size)
{
    vector<unsigned int> wides(maxrank + 1);

    // Getting Max Size;
    unsigned int maxwide = 0;

    // Calculating wide of each rank.
    for (unsigned int rank = 0; rank <= maxrank; rank++) {
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            if (!order->order_vector[rank][i]->dummy) {
                wides[rank] += normalwide;
            } else {
                wides[rank] += dummywide;
            }
        }
        if (wides[rank] > maxwide) {
            maxwide = wides[rank];
        }
    }

    // Centering graph
    for (unsigned int rank = 0; rank <= maxrank; rank++) {
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            if (i == 0) {
                order->order_vector[rank][i]->x = (maxwide / 2) - (wides[rank] / 2) + 20;
            } else if (!order->order_vector[rank][i]->dummy) {
                order->order_vector[rank][i]->x = order->order_vector[rank][i - 1]->x + normalwide;
            } else {
                order->order_vector[rank][i]->x = order->order_vector[rank][i - 1]->x + dummywide;
            }

            order->order_vector[rank][i]->y = rank * vertical_size;
        }
    }
}

void LGraph::Transpose(int max)
{
    if (!layouted) {
        Layout(0, 0, 0);
    }

    bool improved = true;

    for (int j = 1; j < max; j++) {
        improved = true;
        while (improved) {
            improved = false;
            for (unsigned int r = 1; r < order->order_vector.size(); r++) {
                for (unsigned int i = 0; (order->order_vector[r].size() > j) && (i < order->order_vector[r].size() - (j + 1));
                     i++) {
                    pLNode v = order->order_vector[r][i];
                    pLNode w = order->order_vector[r][i + j];
                    InitPos(order);
                    int cross0 = countCrossingOnRank(order, r) + countCrossingOnRank(order, r - 1);
                    v->pos += j;
                    w->pos -= j;
                    int cross1 = countCrossingOnRank(order, r) + countCrossingOnRank(order, r - 1);
                    if (cross1 < cross0) {
                        improved = true;
                        order->order_vector[r][i] = w;
                        order->order_vector[r][i + j] = v;
                    }
                }
            }
        }
    }
}

void LGraph::FreeNode(pNode p)
{
    assert(p != NULL);
    delete (pLNode)p;
}

void LGraph::FreeEdge(pEdge p)
{
    assert(p != NULL);
    delete (pLEdge)p;
}

pLNode
LGraph::AddNode()
{
    pLNode new_node = new LNode(this);
    return new_node;
}

pLEdge
LGraph::AddEdge(pNode from, pNode to)
{
    pLEdge new_edge = new LEdge((pLNode)from, (pLNode)to);
    return new_edge;
}
