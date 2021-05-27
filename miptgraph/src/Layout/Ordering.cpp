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
 * @file: Ordering.cpp
 */

#include "Layout.h"

/**
 * get adjust position to previous level
 */
vector<int>
Ordering::AdjInPositions(pLNode node)
{
    vector<int> positions;
    int rank = node->Rank();
    // incoming edges are not at level 0 nodes
    if (rank > 0) {
        // scan nodes at prev. level
        for (unsigned int i = 0; i < order_vector[rank - 1].size(); i++) {
            // check if node is outgoing of node at order_vector[rank - 1][i]
            if (order_vector[rank - 1][i]->IsAdjacentToNode(node) == true) {
                positions.push_back(i);
            }
        }
    }
    return positions;
}

/**
 * get adjust position to next level
 */
vector<int>
Ordering::AdjOutPositions(pLNode node)
{
    vector<int> positions;
    unsigned int rank = node->Rank();
    // rank must be lower the the max. rank in order_vector.size()
    if (rank < order_vector.size()) {
        // scan nodes at next level
        for (unsigned int i = 0; i < order_vector[rank + 1].size(); i++) {
            // check if node at order_vector[rank + 1][i] is outgoing of node
            if (node->IsAdjacentToNode(order_vector[rank + 1][i])) {
                positions.push_back(i);
            }
        }
    }
    return positions;
}

/**
 * print the node order in all rank levels
 */
void Ordering::Dump()
{
    for (unsigned int rank = 0; rank < order_vector.size(); rank++) {
        printf("Level rank %d has these %lu nodes:\n ", rank, order_vector[rank].size());
        for (unsigned int i = 0; i < order_vector[rank].size(); i++) {
            printf(" id:%d(%d,%d)",
                order_vector[rank][i]->id(),
                (int)order_vector[rank][i]->getX(),
                (int)order_vector[rank][i]->getY());
        }
        printf("\n");
    }
    return;
}

/* end. */
