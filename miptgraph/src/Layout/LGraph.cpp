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
 *
 * SPDX-License-Identifier: GPL-3.0+
 * License-Filename: LICENSE
 */

/**
 * @file: LGraph.cpp
 */

#include "Layout.h"

/**
 * determine layout
 * \param number_of_iterations how many times
 * \param do_transpose if true do phase-II
 * \param transpose_range how many nodes to swap
 */
void LGraph::Layout(unsigned long int number_of_iterations,
    bool do_transpose,
    int transpose_range,
    bool verbose)
{
    bool changed = false;
    int nsame = 0; // same crossings count
    int curc = 0; // current crossings
    int bestc = 0; // best crossings
    bool status = false; // median status
    list<pEdge> ReverseEdges; // edges which are reversed

    if (layouted > 0) {
        // graph is already layouted
        // option here and todo
        return;
    }

    // avoid crash when graph has no nodes
    if (m_total_nodes_num == 0) {
        // The graph is layouted
        layouted++;
        return;
    }

    // nothing to layout if there are no edges but there can be self-edges at nodes
    if (m_total_edges_num == 0) {
        // special layout mode to set the single nodes
        // The graph is layouted
        layouted++;
        InitRank();
        // create array for node ordering in the levels
        order = new Ordering();
        // create node lists of every rank level
        order->order_vector = InitOrder();
        // give nodes relative x position
        InitPos(order);
        // center graph and give nodes absolute node position
        InitCoordinates(order);
        // there are zero crossings
        if (verbose == true) {
            printf("Graph has %u starter nodes and %d ranks\n", m_nstarter_num, (maxrank + 1));
            order->Dump();
            printf("Final Crossings: %d\n", countCrossing(order));
        }
        return;
    }

    // find cycles in the graph, reverse few edges if needed, set vertical rank level of nodes
    changed = FindReverseEdges(ReverseEdges);

    if (changed == true) {
        // number of reversed edges changed
        if (verbose == true) {
            printf("Reversed edges changed to %d\n", nreversededges());
        }
    }

    // change all reversed edges back into normal direction
    // todo
    ReverseReverseEdges(ReverseEdges);

    // get max rank y level in the graph
    InitRank();

    // list for longer edges to split
    list<pEdge> LongEdges;

    // add long edges to list
    FindLongEdges(LongEdges);

    // split long edges in short edges connected with dummy nodes
    AddDummyNodes(LongEdges);

    // free list of reversed edges
    ReverseEdges.clear();

    // create array for node ordering in the levels
    order = new Ordering();

    // create node lists of every rank level
    order->order_vector = InitOrder();

    // Check nodes in order data
    CheckOrder(order);

    // The graph is layouted
    layouted++;

    nsame = 0;
    bestc = 0;

    // improve edge crossings
    for (unsigned int i = 0; i < number_of_iterations; i++) {
        // relative x position nodes on median value
        status = WeightedMedianHeuristic(i, verbose);
        curc = countCrossing(order);
        if (curc == 0) {
            break;
        }
        if (status == true) {
            if (curc < bestc) {
                bestc = curc;
                nsame = 0;
            } else {
                nsame++;
            }
        } else {
            nsame = 0;
        }
        // stop when layout does not change and n times same crossings
        if (nsame > 5) {
            if (verbose == true) {
                printf("%s(): graph does not change anymore at %d crossings\n", __func__, curc);
            }
            break;
        }
        // swap nodes at all levels optional
        // Ususal is to swap nodes with same barycenter value
        if (do_transpose) {
            Transpose(i + transpose_range, verbose);
            curc = countCrossing(order);
            if (curc == 0) {
                break;
            }
        }
    }

    // give nodes relative x position
    InitPos(order);

    // center graph and give nodes absolute node position
    InitCoordinates(order);

    // this prints the node order in the levels
    if (verbose == true) {
        printf("Graph has %u starter nodes\n", m_nstarter_num);
        order->Dump();
        printf("Final Crossings: %d\n", countCrossing(order));
    }

    nodesplay = splay_tree_new(splay_tree_compare_ints, NULL, NULL);

    return;
}

/**
 * get how many rank y levels there are in the graph
 */
void LGraph::InitRank()
{
    m_nstarter_num = 0;
    maxrank = 0;
    // Calculating the rank for all Nodes
    for (list<pNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        // get rank level of this node
        unsigned int rank = ((LNode*)(*node_iter))->Rank();
        if (rank > maxrank) {
            maxrank = rank;
        }
        // check if this node starts a graph
        if (((LNode*)(*node_iter))->entry) {
            m_nstarter_num++;
        }
    }
    return;
}

/**
 * create list of nodes in every rank level
 * \todo add nodes initial order using dfs
 */
vector<vector<pLNode>>
LGraph::InitOrder()
{
    pLNode curnode;
    // reserve space for maxrank levels + 1
    vector<vector<pLNode>> order(maxrank + 1);
    // scan all nodes
    // todo run dfs and in order of visited push
    for (list<pNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        // get current node
        curnode = ((pLNode)(*node_iter));
        if (curnode->in_edges_list()->size() == 0) {
            // this is a starter node
            // todo run dfs at this starter node to fill order[]
        }
        // add node at rank level to list of nodes at that rank level
        order[((pLNode)(*node_iter))->Rank()].push_back((pLNode)(*node_iter));
    }
    return order;
}

/**
 * change all reversed edges into normal direction edge
 */
void LGraph::ReverseReverseEdges(list<pEdge>& ReverseEdges)
{
    // all reversed edges as normal direction edge
    // avoids a looping in node rank()
    for (list<pEdge>::iterator edge_iter = ReverseEdges.begin();
         edge_iter != ReverseEdges.end();
         edge_iter++) {
        ((pLEdge)(*edge_iter))->Reverse();
    }
    return;
}

/**
 * find edges which are longer then 1 vertical rank level
 */
void LGraph::FindLongEdges(list<pEdge>& LongEdges)
{
    int nhedges = 0;
    int delta = 0;
    // scan all edges
    for (list<pEdge>::iterator edge_iter = edges_list()->begin();
         edge_iter != edges_list()->end();
         edge_iter++) {
        delta = ((pLNode)(*edge_iter)->to())->Rank() - ((pLNode)(*edge_iter)->from())->Rank();
        // check for wrong value
        if (delta < 0) {
            // todo
            printf("%s() shouldnothappen\n", __func__);
        }
        // check if edge is too long
        if (delta > 1) {
            // this is a edge spanning multiple levels
            LongEdges.push_back(*edge_iter);
        }
        // check horizontal edge
        if (delta == 0) {
            nhedges++;
            // this is a horizontal edge
            (*edge_iter)->SetHedge(true);
        } else {
            (*edge_iter)->SetHedge(false);
        }
    }

    // set count of horizontal edges
    Set_Nhoredges(nhedges);

    return;
}

/**
 * split long edges with dummy nodes
 */
void LGraph::AddDummyNodes(list<pEdge>& LongEdges)
{
    // scan all edges, skip self-edges
    for (list<pEdge>::iterator edge_iter = LongEdges.begin();
         edge_iter != LongEdges.end();
         edge_iter++) {
        ((pLEdge)(*edge_iter))->BreakLongEdge();
    }
    return;
}

/**
 * Comparator to work with pointers to pLNode
 * comparing the median values and called from stable_sort()
 */
bool ComparePointer(pLNode node1, pLNode node2)
{
    double median1 = node1->getMedian();
    double median2 = node2->getMedian();
    double diff = median1 - median2;
    // numbers close to 0 are same as 0
    if (diff > -0.1) {
        return true;
    }
    if (diff >= 0) {
        return true;
    }
    if (diff < 0) {
        return false;
    }
    // old and not used
    return node1->getMedian() < node2->getMedian();
}

/**
 * scan graph and calculate barycenter values
 * then sort the new node order and check
 * if edge crossings reduced.
 * using stable_sort() keeps the elements with same value unchanged
 */
bool LGraph::WeightedMedianHeuristic(int iter, bool verbose)
{
    bool eq = false;
    Ordering temp_order;

    temp_order.order_vector = order->order_vector;

    // go from top to bottom or reversed
    if (iter % 2 == 0) {
        // scan from top to bottom of drawing
        for (unsigned int r = 1; r <= maxrank; r++) {
            // get median barycenter value for every node at this level
            for (unsigned int i = 0; i < temp_order.order_vector[r].size(); i++) {
                temp_order.order_vector[r][i]->median = temp_order.order_vector[r][i]->Median(*order, MEDIAN_IN);
            }
            // Sort temp_order using ComparePointer comparator on the barycenter value of the nodes
            stable_sort(temp_order.order_vector[r].begin(),
                temp_order.order_vector[r].end(),
                ComparePointer);
        }
    } else {
        // scan from bottom to top of drawing
        for (int r = maxrank - 1; r >= 0; r--) {
            // get median barycenter value for every node at this level
            for (unsigned int i = 0; i < temp_order.order_vector[r].size(); i++) {
                temp_order.order_vector[r][i]->median = temp_order.order_vector[r][i]->Median(*order, MEDIAN_OUT);
            }
            // Sort temp_order using ComparePointer comparator on the barycenter value of the nodes
            // use stable sort to keep equal values at same position
            stable_sort(temp_order.order_vector[r].begin(),
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
        if (verbose == true) {
            printf("%s(): edge crossings reduced from %d to %d\n", __func__, cross_order, cross_temp_order);
        }
        if (cross_order == cross_temp_order) {
            eq = true;
        }
    } else {
        // did not improve, keep graph unchanged
        if (verbose == true) {
            printf("%s(): edge crossings did not reduce from current %d to %d\n", __func__, cross_order, cross_temp_order);
        }
    }

    return (eq);
}

/**
 * Count edge crossings of the whole graph
 */
int LGraph::countCrossing(Ordering* order)
{
    int crossing = 0; // total number of crossings returned
    int rankcrossing = 0; // crossings at this rank level
    int maxcrossing = 0; // max. crossings at certain level
    // int maxcrossingrank = 0; level with most crossings

    // if only single nodes
    if (maxrank == 0) {
        return 0;
    }

    // count crossings at every level
    for (unsigned int rank = 0; rank < maxrank - 1; rank++) {
        rankcrossing = countCrossingOnRank(order, rank);
        if (rankcrossing > maxcrossing) {
            maxcrossing = rankcrossing;
            // maxcrossingrank = rank;
        }
        crossing += rankcrossing;
    }
    return crossing;
}

/**
 * Count edge crossings between rank level rank and rank+1
 */
int LGraph::countCrossingOnRank(Ordering* order, int rank)
{
    int crossing = 0;

    // Making list of all outgoing edges between rank and rank+1 level
    vector<pLEdge> edge_list;

    // scan node at this rank level
    for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
        // select the outgoing edges at a node
        for (list<pEdge>::iterator edge_iter = order->order_vector[rank][i]->out_edges_list()->begin();
             edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
             edge_iter++) {
            edge_list.push_back((pLEdge)(*edge_iter));
        }
    }

    // scan the edges in just created edge list
    for (unsigned int i = 0; i < edge_list.size(); i++) {
        // scan the next edges after current edge
        for (unsigned int j = i + 1; j < edge_list.size(); j++) {
            // Criterion of crossing edge_list[i] and edge_list[j]
            // check relative position of to nodes in edge
            int cmp1 = ((pLNode)edge_list[i]->to())->pos - ((pLNode)edge_list[j]->to())->pos;
            // check relative position of from nodes in edge
            int cmp2 = ((pLNode)edge_list[i]->from())->pos - ((pLNode)edge_list[j]->from())->pos;

            // cmp1 is delto to pos, cmp2 is delta from pos
            if ((cmp1 > 0 && cmp2 < 0) || (cmp1 < 0 && cmp2 > 0)) {
                // edges did cross
                crossing++;
            }
        }
    }

    return crossing;
}

/**
 * Give nodes initial x position based on index in order
 * does extra check on the in/out edges of the nodes in the order
 */
void LGraph::InitPos(Ordering* order)
{
    pLNode curnode;
    pLNode fromnode;
    pLNode tonode;
    pLNode leftnode;
    pLNode rightnode;
    // scan all vertical rank levels
    for (unsigned int rank = 0; rank <= maxrank; rank++) {
        // extra check
        if (order->order_vector[rank].size() <= 0) {
            printf("%s(): wrong number of nodes at level %d\n", __func__, rank);
        }
        // scan all nodes at this level
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            // give node relative x position based on position in order
            // order->order_vector[rank][i]->pos = i;
            curnode = order->order_vector[rank][i];
            // set relative x postion
            curnode->pos = i;
            // scan outgoing edges and check if to node is one level more
            for (list<pEdge>::iterator edge_iter = curnode->out_edges_list()->begin();
                 edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
                 edge_iter++) {
                fromnode = (pLNode)(pLEdge)(*edge_iter)->from();
                tonode = (pLNode)(pLEdge)(*edge_iter)->to();
                if ((tonode->Rank() - fromnode->Rank()) != 1) {
                    printf("%s(): delta at outgoing edges should be 1\n", __func__);
                }
            }
            // same incoming edges
            for (list<pEdge>::iterator edge_iter = curnode->in_edges_list()->begin();
                 edge_iter != order->order_vector[rank][i]->in_edges_list()->end();
                 edge_iter++) {
                fromnode = (pLNode)(pLEdge)(*edge_iter)->from();
                tonode = (pLNode)(pLEdge)(*edge_iter)->to();
                if ((tonode->Rank() - fromnode->Rank()) != 1) {
                    printf("%s(): delta at incoming edges should be 1\n", __func__);
                }
            }
        }
    }
    // set left/right nodes at every node
    // scan all vertical rank levels
    for (unsigned int rank = 0; rank <= maxrank; rank++) {
        // scan all nodes at this level
        leftnode = NULL;
        rightnode = NULL;
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            curnode = order->order_vector[rank][i];
            // set node at left of node or NULL at first node
            curnode->lnode = leftnode;
            if ((i + 1) < order->order_vector[rank].size()) {
                rightnode = order->order_vector[rank][i + 1];
            } else {
                rightnode = NULL;
            }
            // set node at right of node or NULL at last node
            curnode->rnode = rightnode;
            leftnode = curnode;
        }
    }

    return;
}

/**
 * Check node in order data
 */
void LGraph::CheckOrder(Ordering* order)
{
    pLNode curnode;
    pLNode fromnode;
    pLNode tonode;
    // scan all vertical rank levels
    for (unsigned int rank = 0; rank <= maxrank; rank++) {
        // extra check
        if (order->order_vector[rank].size() <= 0) {
            printf("%s(): wrong number of nodes at level %d\n", __func__, rank);
        }
        // scan all nodes at this level
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            // current node to check
            curnode = order->order_vector[rank][i];
            // scan outgoing edges and check if to node is one level more
            for (list<pEdge>::iterator edge_iter = curnode->out_edges_list()->begin();
                 edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
                 edge_iter++) {
                fromnode = (pLNode)(pLEdge)(*edge_iter)->from();
                tonode = (pLNode)(pLEdge)(*edge_iter)->to();
                if ((tonode->Rank() - fromnode->Rank()) != 1) {
                    printf("%s(): delta at outgoing edges should be 1\n", __func__);
                }
            }
            // same incoming edges
            for (list<pEdge>::iterator edge_iter = curnode->in_edges_list()->begin();
                 edge_iter != order->order_vector[rank][i]->in_edges_list()->end();
                 edge_iter++) {
                fromnode = (pLNode)(pLEdge)(*edge_iter)->from();
                tonode = (pLNode)(pLEdge)(*edge_iter)->to();
                if ((tonode->Rank() - fromnode->Rank()) != 1) {
                    printf("%s(): delta at incoming edges should be 1\n", __func__);
                }
            }
        }
    }

    return;
}

/**
 * set nodes at final (x,y) position in this level
 * \param normalwide is x size for a node
 * \param dummy wide is x sie for a dummy node
 * \param vertical_size is y distance between y rank levels
 * \todo this must be more flexible done
 */
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
        // scan nodes at this level
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            // adjust xsize depending on node type
            if (order->order_vector[rank][i]->IsDummy() == false) {
                // regular node
                wides[rank] += normalwide;
            } else {
                // dummy node
                wides[rank] += dummywide;
            }
        }
        // check if this level is wider
        if (wides[rank] > maxwide) {
            maxwide = wides[rank];
        }
    }

    // Centering graph
    for (unsigned int rank = 0; rank <= maxrank; rank++) {
        // check all nodes in this level rank
        // wides[rank] is the x size of this rank
        // maxwide is largest x size of the drawing
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            if (i == 0) {
                // put first node
                order->order_vector[rank][i]->x = (maxwide / 2) - (wides[rank] / 2) + 20;
            } else if (order->order_vector[rank][i]->IsDummy() == false) {
                // regular node
                order->order_vector[rank][i]->x = order->order_vector[rank][i - 1]->x + normalwide;
            } else {
                // dummy node
                order->order_vector[rank][i]->x = order->order_vector[rank][i - 1]->x + dummywide;
            }
            // y spacing
            order->order_vector[rank][i]->y = rank * vertical_size;
        }
    }

    // now priority layout setting (x2,y2)

    return;
}

/**
 * try different graph lyouts to find the one with lowest edge crossings
 * \param maxtry try limited number of times
 */
void LGraph::Transpose(unsigned long int maxtry, bool verbose)
{

    bool improved = true;

    for (unsigned long int j = 1; j < maxtry; j++) {
        improved = true;
        // try another layout
        while (improved == true) {
            improved = false;
            // scan all rank levels
            for (unsigned int r = 1; r < order->order_vector.size(); r++) {
                // scan nodes at levels
                for (unsigned long int i = 0; (order->order_vector[r].size() > j) && (i < order->order_vector[r].size() - (j + 1));
                     i++) {
                    // get nodes
                    pLNode v = order->order_vector[r][i];
                    pLNode w = order->order_vector[r][i + j];
                    // Give nodes initial x position based on index in order
                    InitPos(order);
                    // count crossings between level r and r+1, and level r-1 and r
                    int cross0 = countCrossingOnRank(order, r) + countCrossingOnRank(order, r - 1);
                    // change node positions
                    v->pos += j;
                    w->pos -= j;
                    // after node swapping count crossings between level r and r+1, and level r-1 and r
                    int cross1 = countCrossingOnRank(order, r) + countCrossingOnRank(order, r - 1);
                    // check if reduced crossings, use < and not <= here
                    if (cross1 < cross0) {
                        // swapping nodes improved edge crossings
                        improved = true;
                        // put nodes at new locations
                        order->order_vector[r][i] = w;
                        order->order_vector[r][i + j] = v;
                        if (verbose == true) {
                            printf("%s(): edge crossings reduced from %d to %d at try %lu rank %lu\n", __func__, cross0, cross1, j, i);
                        }

                    } else {
                        if (verbose == true) {
                            printf("%s(): edge crossings did not reduce from current %d to %d at try %lu rank %lu\n", __func__, cross0, cross1, j, i);
                        }
                    }
                    // end of nodes at level
                }
            }
            // end of rank levels
        }
        // end of maxtry
    }

    return;
}

/**
 * free node
 */
void LGraph::FreeNode(pNode p)
{
    assert(p != NULL);
    delete (pLNode)p; // todo warning here
}

/**
 * free edge
 */
void LGraph::FreeEdge(pEdge p)
{
    assert(p != NULL);
    delete (pLEdge)p; // todo warning here
}

/**
 * create new real node
 */
pLNode
LGraph::AddNode()
{
    pLNode new_node = new LNode(this);
    return new_node;
}

/**
 * add edge between from and to nodes
 */
pLEdge
LGraph::AddEdge(pNode from, pNode to, void* usrdata)
{
    pLEdge new_edge = new LEdge((pLNode)from, (pLNode)to);
    new_edge->usrdata = usrdata;
    return new_edge;
}

/**
 * find node with given id number
 */
pLNode LGraph::FindNode(int num)
{
    pLNode curnode;
    for (list<pNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        // get current node
        curnode = ((pLNode)(*node_iter));
        if (curnode->id() == num) {
            return (curnode);
        }
    }
    return (NULL);
}

/**
 * find edge with given id number
 */
pLEdge LGraph::FindEdge(int num)
{
    pLEdge curedge;
    // scan all edges
    for (list<pEdge>::iterator edge_iter = edges_list()->begin();
         edge_iter != edges_list()->end();
         edge_iter++) {
        curedge = (pLEdge)(*edge_iter);
        if (curedge->id() == num) {
            return (curedge);
        }
    }
    return (NULL);
}

/* end. */
