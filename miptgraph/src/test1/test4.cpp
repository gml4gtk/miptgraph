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
 * @file: Layout.cpp
 */

#include "../Layout/Layout.h"

/**
* Create a LGraph object and call some of its methods.
* Run two graph layout at same time.
*/

int main(int argc, char* argv[])
{
    if (argc) {
    }
    if (argv) {
    }
    printf("\nLayout test started..\n");

    LGraph g2;

    g2.Setid(2);

    int len2 = 5;
    pLNode* p2 = new pLNode[len2];

    // Creating a new graph
    for (int i = 0; i < len2; i++) {
        p2[i] = (pLNode)g2.AddNode();
        printf("added node in g2 p2[%d] with id %d\n", i, p2[i]->id());
    }

    pLEdge ee;

    /* nodes in graph 2 are 0..4 */

    /* 0->0 self-edge is not in edge list */
    ee = (pLEdge)g2.AddEdge(p2[0], p2[0]);
    printf("added edge in g2 with id %d between node %d and node %d with id %d\n", g2.id(), ee->from()->id(), ee->to()->id(), ee->id());

    /* graph has no edges */

    /*
    * This main Layout function do whole procedure of LAYOUT:
    * 1. Reversing "Reverse" edges
    * 2. Initing Rank for all nodes
    * 3. Find and breaks "long" edges
    * 4. Makes ordering (horizontal placement)
    * 5. ...
    *
    * void Layout(unsigned int number_of_iterations = 3,
    * bool do_transpose = true,
    * int transpose_range = -1);
    * and transpose_range > 1 to improve crossings
    */

    printf("before layout\n");
    g2.Dump();

    /* layout graph 2 */
    g2.Layout(5, true, 2);

    printf("after layout of graph 2 with dummy nodes and changed edges\n");
    g2.Dump();

    for (list<pNode>::iterator node_iter = g2.nodes_list()->begin();
         node_iter != g2.nodes_list()->end();
         node_iter++) {
        double x = ((LNode*)(*node_iter))->getX();
        double y = ((LNode*)(*node_iter))->getY();
        int id = ((LNode*)(*node_iter))->id();
        printf("node id %d is at position (%d,%d)\n", id, (int)x, (int)y);
    }

    g2.Destroy();
    delete[] p2;

    return (0);
}

/* end. */
