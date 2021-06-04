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
 * @file: LNode.cpp
 */

#include "Layout.h"

/**
 * Ranking of nodes to set at relative vertical level
 * nodes without incoming edges are placed at top of drawing
 * rank value is initialized as -1 means undefined
 * this scans incoming edges, when dfs scans outgoing edges
 */
int LNode::Rank()
{
    // If rank is determined before.
    if (rank >= 0) {
        // do not change
        return rank;
    }

    assert(!we_were_here);

    // If node has no incoming edges rank defines as 0
    if (in_edges_list()->empty()) {
        // at top of drawing, this is a single or starter node.
        rank = 0;
        entry = true;
    } else {
        entry = false;
        // Rank defines as max Rank of adjacent incoming nodes
        int res = 0;
        int temp = 0;
        // scan incomig edges
        for (list<pEdge>::iterator edge_iter = in_edges_list()->begin();
             edge_iter != in_edges_list()->end();
             edge_iter++) {
            if ((pLNode)(*edge_iter)) {
                if (((pLNode)(*edge_iter)->from())) {
                    temp = ((pLNode)(*edge_iter)->from())->Rank() + 1;
                    // set at max
                    if (temp > res) {
                        res = temp;
                    }
                } else {
                    printf("todo\n");
                }
            } else {
                printf("todo0\n");
            }
        }
        // the resulting y level
        rank = res;
    }
    we_were_here = true;
    return rank;
}

/**
 * return median barycenter value of node
 * \param direction 1 or 0 MEDIAN_IN or MEDIAN_OUT checking in/out edges at node
 * if no edges at node returns 0 or -1
 * if edge count is not a multiple of 2, return pos of middle edge point
 * if edge count is exactly 2 use average of pos 0 and 1
 * if edge count is exactly 1 use pos 0
 * if multiple of 2 use average
 * to change these calculations does change the layout
 * other ways are to get average of in+out edges
 * updated with how ogdf does it
 */
double
LNode::Median(Ordering order, bool direction)
{
    vector<int> list;

    // get list with in or out going edges at node
    if (direction == MEDIAN_IN) {
        list = order.AdjInPositions(this);
    } else {
        list = order.AdjOutPositions(this);
    }

    // for bary value use this:
    // iterate list
    //   sum+=list[n];
    // bary=sum/(list.size())

    // how many edges at node
    int size = list.size();

    // mid point
    int m = size / 2;

    // if no edges, return -1
    if (size == 0) {
	    /* orig return -1 */
        // ogdf return 0 should be better
        return 0; // -1;
    }

    // at exactly 1 use pos 0
    if (size == 1) {
        median = (double)list[0];
        return median;
    }

    // at exactly 2 use average of pos 0 and 1
    if (size == 2) {
	if (0) {
	    /* orig */
    	    median = ((double)list[0] + (double)list[1]) / 2.;
        } else {
	    /* this does ogdf */
	    median = ((double)list[0] + (double)list[1]);
	}
	return median;
    }

    // 1 mod 2 is 1
    // 2 mod 2 is 0
    // 3 mod 2 is 1
    // 4 mod 2 is 0

    // if multiple of 2, return pos of middle edge point
    if ((size % 2) == 1) {
	if (0) {
	    /* orig */
	    median = (double)list[m];
        } else {
	    /* this is used in ogdf */
	    median = 2*(double)list[m];
	}
        return median;
    }

    // if no multiple of 2 and this assumes list is sorted on position
    if (0) {
	/* orig */
    double left = (double)list[m - 1] - (double)list[0];
    double right = (double)list[size - 1] - (double)list[m];

    median = (double)((double)list[m - 1] * right + (double)list[m] * left) / (left + right);
    } else {
	/* this is used in ogdf */
	median = (double)list[m] + (double)list[m+1];
    }
    return median;
}

/**
 * return true if node is in out_edges_list()
 */
bool LNode::IsAdjacentToNode(pLNode node)
{
    for (list<pEdge>::iterator edge_iter = out_edges_list()->begin();
         edge_iter != out_edges_list()->end();
         edge_iter++) {
        // skip horizontal edges
        if ((*edge_iter)->IsHedge() == false) {
            if ((pLNode)((*edge_iter)->to()) == node) {
                return true;
            }
        }
    }
    return false;
}

/**
 * delete node
 */
LNode::~LNode()
{
    return;
}

/* end. */
