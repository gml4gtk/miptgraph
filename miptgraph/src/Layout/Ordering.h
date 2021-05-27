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
 * @file: Ordering.h
 */

#ifndef LAYOUT_ORDERING_H
#define LAYOUT_ORDERING_H

/**
 * Class for representation horisontal order of nodes in each rank
 * Ordering contains vector of ranks.
 * Each rank contain ordered list of the nodes.
 */
class Ordering {
private:
    vector<vector<pLNode>> order_vector;

public:
    Ordering() {}
    ~Ordering() {}

    void Dump();

    // The adj_position function returns an ordered array
    // of the present positions of the nodes adjacent to node.
    vector<int> AdjInPositions(pLNode node);
    vector<int> AdjOutPositions(pLNode node);

    friend class LGraph;
};

#endif

/* end. */
