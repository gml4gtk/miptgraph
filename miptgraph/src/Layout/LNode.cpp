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
 * @file: LNode.cpp
 */

#include "Layout.h"

/* Ranking */
int LNode::Rank()
{
    // If rank is determined before..
    if (rank >= 0) {
        return rank;
    }

    assert(!we_were_here);
    // If node has no incoming edges rank defines as 0
    if (in_edges_list()->empty()) {
        rank = 0;
    } else {
        // Rank defines as max Rank of adjacent inoming node.
        int res = 0;
        int temp = 0;
        for (list<pEdge>::iterator edge_iter = in_edges_list()->begin();
             edge_iter != in_edges_list()->end();
             edge_iter++) {
            temp = ((pLNode)(*edge_iter)->from())->Rank() + 1;
            if (temp > res) {
                res = temp;
            }
        }
        rank = res;
    }
    we_were_here = true;
    return rank;
}

double
LNode::Median(Ordering order, bool direction)
{
    vector<int> list;
    if (direction == MEDIAN_IN) {
        list = order.AdjInPositions(this);
    } else {
        list = order.AdjOutPositions(this);
    }

    int size = list.size();
    int m = size / 2;

    if (size == 0) {
        return -1;
    }
    if (size % 2 == 1) {
        median = (double)list[m];
        return median;
    }
    if (size == 2) {
        median = ((double)list[0] + (double)list[1]) / 2.;
        return median;
    }

    double left = (double)list[m - 1] - (double)list[0];
    double right = (double)list[size - 1] - (double)list[m];

    median = (double)((double)list[m - 1] * right + (double)list[m] * left) / (left + right);

    return median; // 0.0;
}

bool LNode::IsAdjacentToNode(pLNode node)
{
    for (list<pEdge>::iterator edge_iter = out_edges_list()->begin();
         edge_iter != out_edges_list()->end();
         edge_iter++) {
        if ((pLNode)((*edge_iter)->to()) == node) {
            return true;
        }
    }
    return false;
}
