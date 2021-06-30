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
 * \param verbose if true print messages
 * \parm usebary if true use barycenter values
 */
void LGraph::Layout(unsigned long int number_of_iterations,
    bool do_transpose,
    int transpose_range,
    bool verbose, bool usebary)
{
    bool changed = false;
    int nsame = 0; // same crossings count
    int curc = 0; // current crossings
    int bestc = 0; // best crossings
    bool status = false; // median status
    list<pLEdge> ReverseEdges; // edges which are reversed

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

    // clear db
    nodesplay = splay_tree_delete(nodesplay);
    edgesplay = splay_tree_delete(edgesplay);
    nodesplay = splay_tree_new(splay_tree_compare_ints, (splay_tree_delete_key_fn)0, (splay_tree_delete_value_fn)0);
    edgesplay = splay_tree_new(splay_tree_compare_ints, (splay_tree_delete_key_fn)0, (splay_tree_delete_value_fn)0);

    // nothing to layout if there are no edges but there can be self-edges at nodes
    if (m_total_edges_num == 0) {
        // special layout mode to set the single nodes
        // The graph is layouted
        layouted++;

        // init vertical levels
        InitRank();

        // create array for node ordering in the levels
        order = new Ordering();

        // create node lists of every rank level
        order->order_vector = InitOrder();

        // give nodes relative x position
        InitPos(order);

        // center graph and give nodes absolute node position
        InitCoordinates(order);

        FinalCoordinates(order);

        // copy nodes in db
        for (list<pLNode>::iterator node_iter = nodes_list()->begin();
             node_iter != nodes_list()->end();
             node_iter++) {
            splay_tree_insert(nodesplay, (splay_tree_key)((LNode*)(*node_iter))->id(), (splay_tree_value)((LNode*)(*node_iter)));
        }

        // there are zero crossings
        if (verbose == true) {
            printf("Graph has %d starter nodes and %d ranks\n", m_nstarter_num, (maxrank + 1));
            order->Dump();
            printf("Final Crossings: %d\n", countCrossing(order));
        }

        delete order;

        return;
    }

    // find cycles in the graph, reverse few edges if needed
    changed = FindReverseEdges(ReverseEdges);

    if (changed == true) {
        // number of reversed edges changed
        if (verbose == true) {
            std::printf("Reversed edges changed to %d\n", nreversededges());
        }
    }

    // reverse edges to get cyclic graph
    ReverseReverseEdges(ReverseEdges);

    // at this point the graph is acyclic with reversed edges
    // double space the graph then same edges are multiple edges
    // set node rank levels and get max rank y level in the graph
    InitRank();

    // list for longer edges to split
    list<pLEdge> LongEdges;

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

    order->m_crossings_num = InitOrder2();
    order->m_iicrossings_num = InitOrder2();
    order->m_ircrossings_num = InitOrder2();
    order->m_rrcrossings_num = InitOrder2();

    // Check nodes in order data
    CheckOrder(order);

    // The graph is layouted
    layouted++;

    nsame = 0;
    bestc = 0;

    // improve edge crossings
    for (unsigned int i = 0; i < number_of_iterations; i++) {
        // relative x position nodes on median value
        status = WeightedMedianHeuristic(i, verbose, usebary);
        curc = countCrossing(order);
        if (curc == 0) {
            break;
        }
        // true status if crossings did not change
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

    // get statistics too and count crossings
    curc = FinalcountCrossing(order);

    // center graph and give nodes absolute node position
    FinalCoordinates(order);

    // copy nodes in db
    for (list<pLNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        splay_tree_insert(nodesplay, (splay_tree_key)((LNode*)(*node_iter))->id(), (splay_tree_value)((LNode*)(*node_iter)));
    }

    // copy edges in db
    for (list<pLEdge>::iterator edge_iter = edges_list()->begin();
         edge_iter != edges_list()->end();
         edge_iter++) {
        splay_tree_insert(edgesplay, (splay_tree_key)((pLEdge)(*edge_iter))->id(), (splay_tree_value)((pLEdge)(*edge_iter)));
    }

    // this prints the node order in the levels
    if (verbose == true) {
        printf("Graph has %u starter nodes\n", m_nstarter_num);
        order->Dump();
        printf("Final Crossings: %d\n", curc);
    }

    // free to avoid memleak
    delete order;

    return;
}

/**
 * get how many rank y levels there are in the graph
 * at this point the graph is acyclic with reversed edges
 * double space the graph then same edges are multiple edges
 */
void LGraph::InitRank()
{
    m_nstarter_num = 0;
    maxrank = 0;
    unsigned int rank = 0;
    // Calculating the rank for all Nodes
    for (list<pLNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        // determine the rank of the node
        rank = ((LNode*)(*node_iter))->Rank();
        if (rank) {
            // silence unused warning
        }
    }
    // Calculating the rank for all Nodes
    for (list<pLNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        // get rank level of this node
        rank = ((LNode*)(*node_iter))->Rank();
        // double space the graph
        // this is needed to make sure same edges are in drawing multiple edges
        // instead of one bundled edge
        ((LNode*)(*node_iter))->setrank(2 * rank);
        rank = ((LNode*)(*node_iter))->Rank();
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
 * that seems to improve the barycenter algorithm
 */
vector<vector<pLNode>>
LGraph::InitOrder()
{
    pLNode curnode;
    // reserve space for maxrank levels + 1
    vector<vector<pLNode>> order(maxrank + 1);

    // scan all nodes
    // todo run dfs and in order of visited push
    for (list<pLNode>::iterator node_iter = nodes_list()->begin();
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
 * reserve space for maxrank levels + 1
 */
vector<int>
LGraph::InitOrder2()
{

    // reserve space for maxrank levels + 1
    vector<int> order(maxrank + 1);

    return order;
}

/**
 * reverse edges to get acyclic graph
 */
void LGraph::ReverseReverseEdges(list<pLEdge>& ReverseEdges)
{
    // all reversed edges as normal direction edge
    // avoids a looping in node rank()
    for (list<pLEdge>::iterator edge_iter = ReverseEdges.begin();
         edge_iter != ReverseEdges.end();
         edge_iter++) {
        ((pLEdge)(*edge_iter))->Reverse();
    }
    return;
}

/**
 * find edges which are longer then 1 vertical rank level
 */
void LGraph::FindLongEdges(list<pLEdge>& LongEdges)
{
    int nhedges = 0;
    int delta = 0;
    pLEdge curedge;
    pLNode fn;
    pLNode tn;

    // scan all edges
    for (list<pLEdge>::iterator edge_iter = edges_list()->begin();
         edge_iter != edges_list()->end();
         edge_iter++) {
        curedge = (*edge_iter);
        fn = curedge->from();
        tn = curedge->to();
        if (fn == NULL) {
            printf("nil fn\n");
        }
        if (tn == NULL) {
            printf("nil tn\n");
        }
        //        delta = ((*edge_iter)->to())->Rank() - ((*edge_iter)->from())->Rank();
        delta = tn->Rank() - fn->Rank();
        // check for wrong value
        if (delta < 0) {
            // todo
            printf("%s() delta is %d shouldnothappen\n", __func__, delta);
        }
        // check if edge is too long
        if (delta > 1) {
            // this is a edge spanning multiple levels
            LongEdges.push_back(*edge_iter);
        }
        // check horizontal edge
        if (delta == 0) {
            nhedges++;
            (*edge_iter)->SetHedge(true);
        } else {
            (*edge_iter)->SetHedge(false);
        }
    }

    // set number of hor. edges
    Set_Nhoredges(nhedges);

    return;
}

/**
 * split long edges with dummy nodes
 */
void LGraph::AddDummyNodes(list<pLEdge>& LongEdges)
{

    // scan all edges, skip self-edges
    for (list<pLEdge>::iterator edge_iter = LongEdges.begin();
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
 * \param if usebary is true use barycenter values
 * \return true if number of crossings did not change
 */
bool LGraph::WeightedMedianHeuristic(int iter, bool verbose, bool usebary)
{
    bool eq = false;
    Ordering temp_order;

    temp_order.order_vector = order->order_vector;

    // go from top to bottom or reversed
    if ((iter % 2) == 0) {
        // scan from top to bottom of drawing
        for (unsigned int r = 1; r <= maxrank; r++) {
            // get median barycenter value for every node at this level
            for (unsigned int i = 0; i < temp_order.order_vector[r].size(); i++) {
                temp_order.order_vector[r][i]->median = temp_order.order_vector[r][i]->Median(*order, MEDIAN_IN, usebary);
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
                temp_order.order_vector[r][i]->median = temp_order.order_vector[r][i]->Median(*order, MEDIAN_OUT, usebary);
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
    eq = false;
    if (cross_temp_order <= cross_order) {
        order->order_vector = temp_order.order_vector;
        if (verbose == true) {
            printf("%s(): edge crossings reduced from %d to %d\n", __func__, cross_order, cross_temp_order);
        }
        // return true if did not change
        if (cross_order == cross_temp_order) {
            eq = true;
        }
    } else {
        // did not improve, keep graph unchanged
        if (verbose == true) {
            if (0) {
                printf("%s(): edge crossings did not reduce from current %d to %d\n", __func__, cross_order, cross_temp_order);
            }
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
    if (maxrank < 2) {
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
 * Count edge crossings of the whole graph and get statistics
 */
int LGraph::FinalcountCrossing(Ordering* order)
{
    int crossing = 0; // total number of crossings returned
    int rankcrossing = 0; // crossings at this rank level
    int maxcrossing = 0; // max. crossings at certain level
    // int maxcrossingrank = 0; level with most crossings

    // if only single nodes
    if (maxrank < 2) {
        return 0;
    }

    // count crossings at every level
    for (unsigned int rank = 0; rank < maxrank - 1; rank++) {
        rankcrossing = FinalcountCrossingOnRank(order, rank);
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
    pLEdge curedge;

    // Making list of all outgoing edges between rank and rank+1 level
    vector<pLEdge> edge_list;

    // scan node at this rank level
    for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
        // select the outgoing edges at a node
        for (list<pLEdge>::iterator edge_iter = order->order_vector[rank][i]->out_edges_list()->begin();
             edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
             edge_iter++) {
            curedge = (*edge_iter);
            edge_list.push_back(curedge);
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
            } else {
                // edges did not cross
            }
        }
    }

    edge_list.clear();

    return crossing;
}

/**
 * Count edge crossings between rank level rank and rank+1
 */
int LGraph::FinalcountCrossingOnRank(Ordering* order, int rank)
{
    int crossing = 0;
    int iicrossings = 0;
    int ircrossings = 0;
    int rrcrossings = 0;
    pLEdge curedgei = NULL;
    pLEdge curedgej = NULL;

    // Making list of all outgoing edges between rank and rank+1 level
    vector<pLEdge> edge_list;

    // scan node at this rank level
    for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
        // select the outgoing edges at a node and reset edge crossing count data
        for (list<pLEdge>::iterator edge_iter = order->order_vector[rank][i]->out_edges_list()->begin();
             edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
             edge_iter++) {
            edge_list.push_back((pLEdge)(*edge_iter));
            // no crossings at this edge
            ((pLEdge)(*edge_iter))->iicross = 0;
            ((pLEdge)(*edge_iter))->ircross = 0;
            ((pLEdge)(*edge_iter))->rrcross = 0;
            // not a  conflict type 1 edge
            ((pLEdge)(*edge_iter))->conflict = false;
        }
    }

    // scan the out going edges in just created edge list and set type of edge crossing
    for (unsigned int i = 0; i < edge_list.size(); i++) {
        // scan the next edges after current edge
        curedgei = (pLEdge)edge_list[i];
        for (unsigned int j = i + 1; j < edge_list.size(); j++) {
            curedgej = (pLEdge)edge_list[j];
            // Criterion of crossing edge_list[i] and edge_list[j]
            // check relative position of to nodes in edge
            int cmp1 = ((pLNode)edge_list[i]->to())->pos - ((pLNode)edge_list[j]->to())->pos;
            // check relative position of from nodes in edge
            int cmp2 = ((pLNode)edge_list[i]->from())->pos - ((pLNode)edge_list[j]->from())->pos;

            // cmp1 is delto to pos, cmp2 is delta from pos
            if ((cmp1 > 0 && cmp2 < 0) || (cmp1 < 0 && cmp2 > 0)) {
                // edges did cross
                crossing++;
                // set type of edge crossing
                if ((curedgei->inner == true) || (curedgej->inner == true)) {

                    if ((curedgei->inner == true) && (curedgej->inner == true)) {
                        // these edges are inner crossing edges
                        curedgei->iicross++;
                        curedgej->iicross++;
                        iicrossings++;
                    } else {
                        /* i is a non-inner, j a inner edge */
                        curedgei->ircross++;
                        curedgej->ircross++;
                        // this are the edge type 1 conflicta
                        ircrossings++;
                    }

                } else {
                    // both are not inner edges
                    curedgei->rrcross++;
                    curedgej->rrcross++;
                    rrcrossings++;
                }
            } else {
                // edges did not cross
            }
        }
    }

    // crossing counts for this level
    order->m_crossings_num[rank] = crossing;
    order->m_iicrossings_num[rank] = iicrossings;
    order->m_ircrossings_num[rank] = ircrossings;
    order->m_rrcrossings_num[rank] = rrcrossings;

    edge_list.clear();

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
            printf("%s(): wrong number of nodes at level %d with %lu nodes\n", __func__, rank, order->order_vector[rank].size());
        }
        // scan all nodes at this level
        for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
            // give node relative x position based on position in order
            // order->order_vector[rank][i]->pos = i;
            curnode = order->order_vector[rank][i];
            // set relative x postion
            curnode->pos = i;
            // scan outgoing edges and check if to node is one level more
            for (list<pLEdge>::iterator edge_iter = curnode->out_edges_list()->begin();
                 edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
                 edge_iter++) {
                fromnode = (pLNode)(pLEdge)(*edge_iter)->from();
                tonode = (pLNode)(pLEdge)(*edge_iter)->to();
                if ((tonode->Rank() - fromnode->Rank()) != 1) {
                    printf("%s(): delta at outgoing edges should be 1\n", __func__);
                }
            }
            // same incoming edges
            for (list<pLEdge>::iterator edge_iter = curnode->in_edges_list()->begin();
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
    int delta = 0;

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
            for (list<pLEdge>::iterator edge_iter = curnode->out_edges_list()->begin();
                 edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
                 edge_iter++) {
                fromnode = (pLNode)(pLEdge)(*edge_iter)->from();
                tonode = (pLNode)(pLEdge)(*edge_iter)->to();
                delta = (tonode->Rank() - fromnode->Rank());
                if (delta != 1) {
                    printf("%s(): delta at outgoing edges should be 1 but it is %d\n", __func__, delta);
                }
            }
            // same incoming edges
            for (list<pLEdge>::iterator edge_iter = curnode->in_edges_list()->begin();
                 edge_iter != order->order_vector[rank][i]->in_edges_list()->end();
                 edge_iter++) {
                fromnode = (pLNode)(pLEdge)(*edge_iter)->from();
                tonode = (pLNode)(pLEdge)(*edge_iter)->to();
                delta = (tonode->Rank() - fromnode->Rank());
                if (delta != 1) {
                    printf("%s(): delta at incoming edges should be 1 but it is %d\n", __func__, delta);
                }
            }
        }
    }

    return;
}

/**
 *
 * \param orient is 0..3 orientation mode
 */
static void FinalCoordinates_horizontal_compaction(Ordering* order, int orient)
{
}

//static bool FinalCoordinates_vertical_align_is_type1_edge(node u, node v)
// scan edge list for this edge and return if it is type1 edge
// or scan outgoing edges of node for this edge and check

/**
 * for every node choose the node it will be vertical aligned to
 * \param orient is 0..3 orientation mode

initialize root[v] = v;
initialize align[v] = v;
for i=1 to lastlayer do
{
 r←0;
 for k=1 to last_node_in_layer do
 {
   if node v(i) k has_upper_neighbors u1 ≺ ... ≺ ud with d > 0 then
   {
 // slice
    for m median of node v(i) k do
    {
     if align[v(i)k] = v(i)k then
     {
      if (um,v(i)k) not marked and r < pos[um] then
      {
        align[um] ← v(i)k;
       root[v(i)k] ← root[um];
       align[v(i)k] ← root[v(i)k];
       r = pos[um];
     }
    }
   }
  }
 }
}


 */
static void FinalCoordinates_vertical_align(Ordering* order, int orient)
{
}

/**
 * sort in/out edges of a node
 */
static void FinalCoordinates_init_sortededges(pLNode node)
{
    int nconn = 0;
    int needed = 0;
    int prevfrom = 0;
    int prevto = 0;
    pLEdge curedge;
    pLNode fromnode;
    pLNode tonode;

    nconn = (int)node->in_edges_list()->size();

    if (nconn > 1) {
        needed = 0;
        prevfrom = -1;
        // check if sort is needed
        for (list<pLEdge>::iterator edge_iter = node->in_edges_list()->begin(); edge_iter != node->in_edges_list()->end(); edge_iter++) {
            curedge = (*edge_iter);
            fromnode = curedge->from();
            if (fromnode->getPos() < prevfrom) {
                needed = 1;
                break;
            }
            prevfrom = fromnode->getPos();
        }
        if (needed > 0) {
            // sort of in edges is needed
            node->NodeSortIn();
        }
    }

    nconn = (int)node->out_edges_list()->size();

    if (nconn > 1) {
        needed = 0;
        prevto = -1;
        // check if sort is needed
        for (list<pLEdge>::iterator edge_iter = node->out_edges_list()->begin(); edge_iter != node->out_edges_list()->end(); edge_iter++) {
            curedge = (*edge_iter);
            tonode = curedge->to();
            if (tonode->getPos() < prevto) {
                needed = 1;
                break;
            }
            prevto = tonode->getPos();
        }
        if (needed > 0) {
            // sort of out edge is needed
            node->NodeSortOut();
        }
    }

    return;
}

/**
 *
 */
void LGraph::FinalCoordinates_init_medians(Ordering* order)
{
    list<pLEdge>::iterator edge_iter;
    pLNode curnode;
    pLNode c1node;
    pLNode c2node;
    pLEdge curedge;
    int i = 0;
    int j = 0;
    int nconn = 0;

    // init the sizes for the node data
    // m_total_nodes_num is > 0 and is number of real + dummy nodes
    for (i = 0; i < 4; i++) {
        // root node of block the node belongs to
        root[i] = vector<pLNode>(m_total_nodes_num);
        // node to align with of the node
        align[i] = vector<pLNode>(m_total_nodes_num);
        // node
        sink[i] = vector<pLNode>(m_total_nodes_num);
        // node
        medians[i] = vector<pLNode>(m_total_nodes_num);
        // movement
        shift[i] = vector<double>(m_total_nodes_num);
    }

    // scan all nodes to set the median nodes
    for (list<pLNode>::iterator node_iter = nodes_list()->begin();
         node_iter != nodes_list()->end();
         node_iter++) {
        curnode = ((LNode*)(*node_iter));
        j = curnode->id();

        // in/out edges of node must be sorted for median
        FinalCoordinates_init_sortededges(curnode);

        // init the 4 connecting nodes possible at node
        // upper_left=0, lower_left=1, upper_right=2, lower_right=3
        for (i = 0; i < 4; i++) {
            root[i][j] = curnode;
            align[i][j] = curnode;
            sink[i][j] = curnode;
            medians[i][j] = curnode;
            shift[i][j] = 0.0;
        }

        // preset
        medians[orient::lower_left][j] = curnode; // left;
        medians[orient::lower_right][j] = curnode; // right;

        // check the out going edges of this node
        nconn = (int)curnode->out_edges_list()->size();
        if (nconn == 0) {
            // no out edges
            medians[orient::lower_left][j] = curnode; // left;
            medians[orient::lower_right][j] = curnode; // right;
        } else if (nconn == 1) {
            // find median of out edges
            // with 1 edge both are the to node.
            edge_iter = curnode->out_edges_list()->begin();
            c1node = (pLNode)(pLEdge)(*edge_iter)->to();
            medians[orient::lower_left][j] = c1node; // left;
            medians[orient::lower_right][j] = c1node; // right;
        } else if (nconn == 2) {
            // find median of out edges
            edge_iter = curnode->out_edges_list()->begin();
            c1node = (pLNode)(pLEdge)(*edge_iter)->to();
            edge_iter++;
            c2node = (pLNode)(pLEdge)(*edge_iter)->to();
            medians[orient::lower_left][j] = c1node; // left;
            medians[orient::lower_right][j] = c2node; // right;
        } else {
            // find median of out edges
            i = 0;
            for (list<pLEdge>::iterator edge_iter = curnode->out_edges_list()->begin();
                 edge_iter != curnode->out_edges_list()->end();
                 edge_iter++) {
                curedge = (*edge_iter);
                if (i == ((nconn / 2) - 1)) {
                    c1node = (*edge_iter)->to();
                    edge_iter++;
                    c2node = (*edge_iter)->to();
                    medians[orient::lower_left][j] = c1node; // left;
                    medians[orient::lower_right][j] = c2node; // right;
                    break;
                }
                i++;
            }
        }

        // check the in coming edges to this node
        nconn = (int)curnode->in_edges_list()->size();

        // preset default
        medians[orient::upper_left][j] = curnode; // left;
        medians[orient::upper_right][j] = curnode; // right;

        if (nconn == 0) {
            // no in edges
            medians[orient::upper_left][j] = curnode; // left;
            medians[orient::upper_right][j] = curnode; // right;
        } else if (nconn == 1) {
            // find median of in edges
            edge_iter = curnode->in_edges_list()->begin();
            c1node = (pLNode)(pLEdge)(*edge_iter)->from();
            medians[orient::lower_left][j] = c1node; // left;
            medians[orient::lower_right][j] = c1node; // right;
        } else if (nconn == 2) {
            // find median of in edges
            edge_iter = curnode->in_edges_list()->begin();
            c1node = (*edge_iter)->from();
            edge_iter++;
            c2node = (*edge_iter)->from();
            medians[orient::lower_left][j] = c1node; // left;
            medians[orient::lower_right][j] = c2node; // right;
        } else {
            // find median of in edges
            i = 0;
            for (list<pLEdge>::iterator edge_iter = curnode->in_edges_list()->begin();
                 edge_iter != curnode->in_edges_list()->end();
                 edge_iter++) {
                curedge = (*edge_iter);
                if (i == ((nconn / 2) - 1)) {
                    c1node = (*edge_iter)->from();
                    edge_iter++;
                    c2node = (*edge_iter)->from();
                    medians[orient::lower_left][j] = c1node; // left;
                    medians[orient::lower_right][j] = c2node; // right;
                    break;
                }
                i++;
            }
        }
    }

    return;
}

/**
 * set nodes at final (x,y) position in the graph
 * (x2,y2) have same values as (x,y) at start
 * this set new (x2,y2) coordinates for the nodes
 */
void LGraph::FinalCoordinates(Ordering* order)
{
    bool verbose = true;
    int ntype1 = 0;
    int rc = 0;
    vector<pLEdge> edge_list;

    // print the levels with the orders of the nodes
    if (verbose == true) {
        order->Dump();
    }

    // mark crossing edge conflicts
    // this is the crossing count routine but only checks where non-inner segment and a inner segment cross
    // there can only dummynodes when maxrank >1
    // with levels 0, 1 and 2 with at least dummy nodes at level 1
    // This phase of the node placer marks all type 1 and type 2 conflicts.
    // The conflict types base on the distinction of inner segments and non-inner segments of edges.
    // A inner segment is present if an edge is drawn between two dummy nodes and thus is part of
    // a long edge.
    // A non-inner segment is present if one of the connected nodes is not a dummy node.
    // Type 0 conflicts occur if two non-inner segments cross each other.
    // Type 1 conflicts happen when a non-inner segment and a inner segment cross.
    // Type 2 conflicts are present if two inner segments cross.
    // The markers are later used to solve conflicts in favor of long edges.
    // In case of type 2 conflicts, the marker favors the earlier node in layout order.
    // In case of type 1 conflict the non-inner edge is marked as conflicting edge.
    if (maxrank > 1) {
        // here if maxrank is 2 or more with levels 0, 1, 2 ...
        // there can only be inner edges below level 2 because level 0 has start nodes
        // and level 1 has dummy nodes with only non-inner edges
        // the last level only has end nodes with (last-level-1) dummy nodes
        // at that level are only non-inner edges
        for (unsigned int rank = 2; rank < maxrank - 2; rank++) {
            rc = 0;
            // only if there are crossings at this level with a dummy node involved
            // edge type 1 conflicts when inner edge crosses non-inner edge
            if (verbose == true) {
                std::printf("between level %d and level %d are %d crossings", rank, rank + 1, order->m_crossings_num[rank]);
                if (order->m_crossings_num[rank] > 0) {
                    std::printf(", %d local-nonlocal Type 1 conflict, %d local-local Type 2 conflict, %d nonlocal-nonlocal crossings type 0 conflict",
                        order->m_ircrossings_num[rank],
                        order->m_iicrossings_num[rank],
                        order->m_rrcrossings_num[rank]);
                }
                std::printf("\n");
            }
            if (order->m_ircrossings_num[rank] > 0) {
                if (verbose == true) {
                    std::printf("between level %d and level %d are %d Type 1 edge conflicts\n", (int)rank, (int)(rank + 1), (int)order->m_ircrossings_num[rank]);
                }
                // check nodes at level (rank) and the out edges to level rank+1
                // find the Type 1 conflict edges and mark them
                // if curedge->ircross > 0 then it is a Type 1 conflict edge
                // check if edge is inner or non-inner segment
                // mark non-inner segment as conflict edge

                // Making list of all outgoing edges between rank and rank+1 level
                edge_list.clear();

                // scan node at this rank level
                for (unsigned int i = 0; i < order->order_vector[rank].size(); i++) {
                    // select the outgoing edges at a node and reset edge crossing count data
                    for (list<pLEdge>::iterator edge_iter = order->order_vector[rank][i]->out_edges_list()->begin();
                         edge_iter != order->order_vector[rank][i]->out_edges_list()->end();
                         edge_iter++) {
                        edge_list.push_back((pLEdge)(*edge_iter));
                        // not a  conflict type 1 edge
                        ((pLEdge)(*edge_iter))->conflict = false;
                    }
                }

                // scan the edges in just created edge list
                for (unsigned int i = 0; i < edge_list.size(); i++) {
                    if (edge_list[i]->n_ircross() > 0) {
                        // scan the next edges after current edge
                        for (unsigned int j = i + 1; j < edge_list.size(); j++) {
                            if (edge_list[j]->n_ircross() > 0) {
                                // Criterion of crossing edge_list[i] and edge_list[j]
                                // check relative position of to nodes in edge
                                int cmp1 = ((pLNode)edge_list[i]->to())->pos - ((pLNode)edge_list[j]->to())->pos;
                                // check relative position of from nodes in edge
                                int cmp2 = ((pLNode)edge_list[i]->from())->pos - ((pLNode)edge_list[j]->from())->pos;

                                // cmp1 is delto to pos, cmp2 is delta from pos
                                if ((cmp1 > 0 && cmp2 < 0) || (cmp1 < 0 && cmp2 > 0)) {
                                    rc++;
                                    // edges did cross
                                    if (verbose == true) {
                                        std::printf("edge crossing inner status %d and %d\n", edge_list[i]->inner, edge_list[j]->inner);
                                    }
                                    if (edge_list[i]->inner == true && edge_list[j]->inner == true) {
                                        // inner inner edge crossings
                                    } else if (edge_list[i]->inner == false && edge_list[j]->inner == false) {
                                        // should not happen
                                    } else if (edge_list[i]->inner == false && edge_list[j]->inner == true) {
                                        // noninner inner crossing
                                        if (edge_list[i]->conflict == false) {
                                            // set the noninner edge as conflict edge
                                            edge_list[i]->setconflict(true);
                                            ntype1++;
                                        }
                                    } else {
                                        // inner noninner crossing
                                        if (edge_list[j]->conflict == false) {
                                            // set the noninner edge as conflict edge
                                            edge_list[j]->setconflict(true);
                                            ntype1++;
                                        }
                                    }
                                } else {
                                    // edges did not cross
                                }
                            } // end of if (edge_list[j]->n_ircross() > 0)
                        } // end of for (unsigned int j = i + 1; j < edge_list.size(); j++)
                    } // end of if (edge_list[i]->n_ircross() > 0)
                } // end of for (unsigned int i = 0; i < edge_list.size(); i++)
            } // end of if (order->m_ircrossings_num[rank] > 0)
            // todo here
            std::printf("at rank %d found %d crossings and expected %d crossings\n", rank, order->m_ircrossings_num[rank], rc);
        } // end of for (unsigned int rank = 0; rank < maxrank - 1; rank++)
    } // end of if (maxrank > 1)

    edge_list.clear();

    if (verbose == true) {
        if (ntype1 > 0) {
            std::printf("total %d type 1 conflict edges in the graph\n", ntype1);
            // output it as dot graph
            std::printf("// conflict type 1 edge is noninner edge colored red and crossing between inner and noninner edge\n");
            std::printf("// inner edge is blue\n");
            std::printf("// noninner edge is green\n");
            std::printf("digraph conflict1edges {\n");
            // scan all edges
            for (list<pLEdge>::iterator edge_iter = edges_list()->begin();
                 edge_iter != edges_list()->end();
                 edge_iter++) {
                std::printf(" %d -> %d [color=\"", (*edge_iter)->from()->id(), (*edge_iter)->to()->id());
                if (((pLEdge)(*edge_iter))->conflict == true) {
                    // this is a conflict 1 edge and a noninner edge
                    std::printf("red");
                } else {
                    // colorize inner/noninner edges
                    if (((pLEdge)(*edge_iter))->inner == true) {
                        std::printf("blue");
                    } else {
                        std::printf("green");
                    }
                }
                std::printf("\"];\n");
            }
        }
        std::printf("}\n");
    }

    // init arrays with the node data
    FinalCoordinates_init_medians(order);

    for (int i = 0; i < 4; ++i) {
        // for every node choose the node it will be vertica; aligned to
        FinalCoordinates_vertical_align(order, i);
        FinalCoordinates_horizontal_compaction(order, i);
    }

    return;
}

/**
 * set nodes at final (x,y) position in this level
 * \param normalwide is x size for a node
 * \param dummy wide is x sie for a dummy node
 * \param vertical_size is y distance between y rank levels
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
            if (order->order_vector[rank][i]->dummy == false) {
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
            } else if (order->order_vector[rank][i]->dummy == false) {
                // regular node
                order->order_vector[rank][i]->x = order->order_vector[rank][i - 1]->x + normalwide;
            } else {
                // dummy node
                order->order_vector[rank][i]->x = order->order_vector[rank][i - 1]->x + dummywide;
            }
            // y spacing
            order->order_vector[rank][i]->y = rank * vertical_size;

            // set a value for the second (x,y) pos
            order->order_vector[rank][i]->x2 = order->order_vector[rank][i]->x;
            order->order_vector[rank][i]->y2 = order->order_vector[rank][i]->y;
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
                            if (0) {
                                printf("%s(): edge crossings did not reduce from current %d to %d at try %lu rank %lu\n", __func__, cross0, cross1, j, i);
                            }
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
void LGraph::FreeNode(pLNode p)
{
    assert(p != NULL);
    delete p; // todo warning here
}

/**
 * free edge
 */
void LGraph::FreeEdge(pLEdge p)
{
    assert(p != NULL);
    delete (pLEdge)p; // todo warning here
}

/**
 * add edge between from and to nodes
 */
pLEdge
LGraph::AddEdge(pLNode from, pLNode to, void* usrdata)
{
    pLEdge new_edge = new LEdge(from, to);
    new_edge->usrdata = usrdata;
    return new_edge;
}

/**
 * find node with given id number
 */
pLNode LGraph::FindNode(int num)
{
    pLNode curnode = NULL;
    splay_tree_node spn = NULL;

    spn = splay_tree_lookup(nodesplay, (splay_tree_key)num);

    if (spn) {
        curnode = (pLNode)spn->value;
    }

    return (curnode);
}

/**
 * find edge with given id number
 */
pLEdge LGraph::FindEdge(int num)
{
    pLEdge curedge = NULL;
    splay_tree_node spn = NULL;

    spn = splay_tree_lookup(edgesplay, (splay_tree_key)num);

    if (spn) {
        curedge = (pLEdge)spn->value;
    }

    return (curedge);
}

/**
     * create graph
     */
LGraph::LGraph()
{
    maxrank = 0;
    m_nstarter_num = 0;
    layouted = false;
    order = (Ordering*)0;
    nodesplay = (splay_tree)0;
    edgesplay = (splay_tree)0;
    m_total_nodes_num = 0;
    m_total_dummynodes_num = 0;
    m_total_edges_num = 0;
    m_total_horedges_num = 0;
    m_total_selfedges_num = 0;
    m_total_reversededges_num = 0;
    next_node_id = 0;
    next_edge_id = 0;
    m_id = 0;
    m_xspacing_num = 20;
    m_yspacing_num = 40;
}

/**
     * delete graph
     */
LGraph::~LGraph()
{
    nodesplay = splay_tree_delete(nodesplay);
    edgesplay = splay_tree_delete(edgesplay);
    Destroy();
}

/**
 * create node
 */
pLNode LGraph::AddNode()
{
    pLNode new_node = new LNode(this);
    return new_node;
}

/**
 * clear graph
 * \todo check
 */
void LGraph::Destroy()
{
    //    list<pNode>::iterator node_iter;
    //    list<pEdge>::iterator edge_iter;

    while (m_nodes_list.empty() == false) {
        pLNode pn = m_nodes_list.front();
        // delete node and it in/out edges
        DeleteNode(pn);
    }

    assert(m_nodes_list.empty());
    assert(m_edges_list.empty());
    assert(m_total_edges_num == 0);
    assert(m_total_nodes_num == 0);

    nodesplay = splay_tree_delete(nodesplay);
    edgesplay = splay_tree_delete(edgesplay);

    return;
}

/**
 * return false if some integrity error of graph data
 */
bool LGraph::Verify(void)
{
    // error if no nodes, but there are edges.
    if (m_edges_list.size() > 0 && m_nodes_list.size() == 0) {
        return false;
    }

    map<pLEdge, bool> edge_map;

    // scan all nodes
    for (list<pLNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {

        // error if node does not belong to this graph
        if ((*node_iter)->m_graph != this) {
            return false;
        }

        // scan incoming edges at this node
        for (list<pLEdge>::iterator edge_iter = (*node_iter)->m_in_edges_list.begin();
             edge_iter != (*node_iter)->m_in_edges_list.end();
             edge_iter++) {

            // error if edge does not belong to this graph
            if ((*node_iter)->m_graph != (*edge_iter)->m_graph) {
                return false;
            }
        }

        // scan outgoing edges at this node
        for (list<pLEdge>::iterator edge_iter = (*node_iter)->m_out_edges_list.begin();
             edge_iter != (*node_iter)->m_out_edges_list.end();
             edge_iter++) {

            // error if edge does not belong to this graph
            if ((*node_iter)->m_graph != (*edge_iter)->m_graph) {
                return false;
            }
        }
    }

    // scan all edges
    for (list<pLEdge>::iterator edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {

        edge_map[(*edge_iter)] = true;
    }

    // scan all edges
    for (list<pLEdge>::iterator edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {

        // error if edge has no from node or no end node
        if ((*edge_iter)->m_from == NULL || (*edge_iter)->m_to == NULL) {
            return false;
        }

        // error if edge does not belong to a graph
        if ((*edge_iter)->m_graph == NULL) {
            return false;
        }

        // error if one of nodes does not belong to a graph
        if ((*edge_iter)->m_from->m_graph == NULL || (*edge_iter)->m_to->m_graph == NULL) {
            return false;
        }

        // error if edge does not belong to this graph
        if ((*edge_iter)->m_graph != this) {
            return false;
        }

        // error if iter does not belong to this graph
        if (!((*edge_iter)->m_graph == (*edge_iter)->m_to->m_graph && (*edge_iter)->m_graph == (*edge_iter)->m_from->m_graph)) {
            return false;
        }

        bool flag = false;

        // Check for list of incoming edges.
        for (list<pLEdge>::iterator edge_local = (*edge_iter)->m_to->m_in_edges_list.begin();
             edge_local != (*edge_iter)->m_to->m_in_edges_list.end();
             edge_local++) {

            // Sets if this is exist incoming edge
            if ((*edge_iter) == (*edge_local)) {
                flag = true; // named edge_iter.
            }

            // If there are some edges without declaration
            if (edge_map[(*edge_iter)] == false) {
                return false; // in m_edges_list, graph isn't correct.
            }
        }

        /* stop at error */
        if (flag == false) {
            return false;
        }

        flag = false;

        // The same check for list of outgoing edges.
        for (list<pLEdge>::iterator edge_local = (*edge_iter)->m_from->m_out_edges_list.begin();
             edge_local != (*edge_iter)->m_from->m_out_edges_list.end();
             edge_local++) {

            // Sets if this is exist outgoing edge
            if ((*edge_iter) == (*edge_local)) {
                flag = true;
            }

            // If there are some edges without declaration
            if (edge_map[(*edge_iter)] == false) {
                return false;
            }
        }

        // stop at error
        if (flag == false) {
            return false;
        }
    }

    // graph data is correct
    return true;
}

/**
 * run dfs to assing node dfs number
 */
void LGraph::DFS(pLNode node,
    map<pLNode, bool>* isused,
    map<pLNode, int>* dfs,
    int* num)
{
    pLEdge curedge;
    pLNode ton;

    if ((*isused)[node] == true) {
        // return if node has already set
        return;
    }

    (*isused)[node] = true;

    // set dfs count
    (*dfs)[node] = ++(*num);

    // follow outgoing edges of this node
    for (list<pLEdge>::iterator edge_iter = node->m_out_edges_list.begin();
         edge_iter != node->m_out_edges_list.end();
         edge_iter++) {
        curedge = (*edge_iter);
        ton = curedge->to();
        if (ton) {
            // if to edge is not done, recurse
            //        if (((*isused)[(*edge_iter)->m_to]) == false) {
            //            DFS((*edge_iter)->m_to, isused, dfs, num);
            //        }
            if (((*isused)[ton]) == false) {
                DFS(ton, isused, dfs, num);
            }
        }
    }

    return;
}

/**
 * return true if revesed edges count changed
 * find cycles in the graph and reverse edge
 */
bool LGraph::FindReverseEdges(list<pLEdge>& ReverseEdges)
{
    map<pLNode, int>* dfs = new map<pLNode, int>; // dfs count numbers
    map<pLNode, bool>* isused = new map<pLNode, bool>; // dfs flags
    int num = 0; // Current dfs counter is 0
    size_t count_rev_edges = ReverseEdges.size();
    int rank = 0;

    // clear dfs
    for (list<pLNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {
        (*dfs)[*node_iter] = 0;
    }

    // clear used
    for (list<pLNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {
        (*isused)[(*node_iter)] = false;
    }

    for (list<pLNode>::iterator node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {

        if ((*isused)[*node_iter] == false) {
            // Starts from every new node.
            DFS((*node_iter), isused, dfs, &num);
        }
    }

    delete isused;

    // detect cycles
    for (list<pLEdge>::iterator edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {

        if ((*dfs)[(*edge_iter)->m_to] < (*dfs)[(*edge_iter)->m_from]) {
            // this is a cycle
            ReverseEdges.push_back((*edge_iter));
        }
    }

    delete dfs;

    // total number of reversed edges without self-edges
    m_total_reversededges_num = ReverseEdges.size();

    if (ReverseEdges.size() == count_rev_edges) {
        return false; // No changes
    }

    /* reversed edges changed  */
    return true;
}

/**
 * set uniq graph id as int number
 */
void LGraph::Setid(int id)
{

    m_id = id;

    return;
}

/**
 * increase count of dummy nodes
 */
void LGraph::AddDummyNode()
{
    m_total_dummynodes_num++;
    return;
}

/**
 * delete node and it in/out edges
 */
void LGraph::DeleteNode(pLNode p)
{
    bool found = false;

    // todo if dummy node, update graph dummy node count

    // clear incoming edges
    while (p->m_in_edges_list.empty() == false) {
        found = DeleteEdge(p->m_in_edges_list.front()->m_from, p);
        if (found == false) { // edge not found
        }
    }

    // clear outgoing edges
    while (p->m_out_edges_list.empty() == false) {
        found = DeleteEdge(p, p->m_out_edges_list.front()->m_to);
        if (found == false) { // edge not found
        }
    }

    // remove node from nodelist
    m_nodes_list.remove(p);

    // decr. number of total nodes
    if (m_total_nodes_num > 0) {
        m_total_nodes_num--;
    }

    // clear node
    FreeNode(p);

    return;
}

/**
 * remove edge between node from and node to
 * return tru if edge deleted, false when edge not found
 */
bool LGraph::DeleteEdge(pLNode from, pLNode to)
{
    assert(from && from->m_graph == this);
    assert(to && to->m_graph == this);

    // delete self edge if specified
    if (from->id() == to->id()) {
        // this is a self edge
        if (from->nselfedges() <= 0) {
            // shouldnothappen
            from->m_selfedges = 0;
            return false;
        }
        from->m_selfedges--;
        // update in graph data
        if (from->m_graph->m_total_selfedges_num > 0) {
            from->m_graph->m_total_selfedges_num--;
        }
        return true;
    }

    list<pLEdge>::iterator edge_iter;

    // scan edge list
    for (edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {
        pLEdge pe = *edge_iter;
        // if this is edge with from/to number
        if (pe->m_from == from && pe->m_to == to) {
            m_edges_list.erase(edge_iter);
            // remove as outgoing edge at from node
            pe->m_from->m_out_edges_list.remove(pe);
            // remove as incoming edge at to node
            pe->m_to->m_in_edges_list.remove(pe);
            pe->m_from = NULL;
            pe->m_to = NULL;
            m_total_edges_num--;
            FreeEdge(pe);
            // edge is found and deleted
            return true;
        }
    }

    // return false if edge not found.
    return false;
}

/**
 * print graph data
 */
void LGraph::Dump()
{
    list<pLNode>::iterator node_iter;
    list<pLEdge>::iterator edge_iter;

    printf("Dumping graph id:%d (%d nodes, %d real nodes, %d dummy nodes, %d edges, %d horizontal edges, %d selfedges %d reversed edges)\n", id(), nnodes(), nrealnodes(), ndummynodes(), nedges(), nhoredges(), nselfedges(), nreversededges());
    printf("Nodes:\n");
    for (node_iter = m_nodes_list.begin();
         node_iter != m_nodes_list.end();
         node_iter++) {
        pLNode pn = *node_iter;
        if (pn) {
            /* print single node */
            pn->Dump();
        }
    }

    // edge list does not have self edges
    printf("Edges:\n");
    for (edge_iter = m_edges_list.begin();
         edge_iter != m_edges_list.end();
         edge_iter++) {
        pLEdge pe = *edge_iter;
        if (pe) {
            /* print single edge */
            pe->Dump();
        }
    }

    return;
}

/* end. */
