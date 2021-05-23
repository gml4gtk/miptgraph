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
 * @file: Ordering.cpp
 */

#include "Layout.h"

vector<int>
Ordering::AdjInPositions(pLNode node)
{
    vector<int> positions;
    int rank = node->Rank();
    if (rank > 0) {
        for (unsigned int i = 0; i < order_vector[rank - 1].size(); i++) {
            if (order_vector[rank - 1][i]->IsAdjacentToNode(node)) {
                positions.push_back(i);
            }
        }
    }
    return positions;
}

vector<int>
Ordering::AdjOutPositions(pLNode node)
{
    vector<int> positions;
    unsigned int rank = node->Rank();
    if (rank < order_vector.size()) {
        for (unsigned int i = 0; i < order_vector[rank + 1].size(); i++) {
            if (node->IsAdjacentToNode(order_vector[rank + 1][i])) {
                positions.push_back(i);
            }
        }
    }
    return positions;
}

void Ordering::Dump()
{
    for (unsigned int rank = 0; rank < order_vector.size(); rank++) {
        printf("rank = %d\n", rank);
        for (unsigned int i = 0; i < order_vector[rank].size(); i++) {
            printf("%d(%d,%d) ",
                order_vector[rank][i]->id(),
                order_vector[rank][i]->getX(),
                order_vector[rank][i]->getY());
        }
        printf("\n");
    }

    return;
}
