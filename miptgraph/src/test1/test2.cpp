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

    LGraph g;
    LGraph g2;

    int len = 10;
    pLNode* p = new pLNode[len];

    int len2 = 5;
    pLNode* p2 = new pLNode[len2];

    // Creating a new graph
    for (int i = 0; i < len; i++) {
        p[i] = (pLNode)g.AddNode();
    }

    // Creating a new graph
    for (int i = 0; i < len2; i++) {
        p2[i] = (pLNode)g2.AddNode();
        printf("added node in g2 p2[%d] with id %d\n", i, p2[i]->id());
    }

    // Performing some transformations
    /* nodes in graph 1 are 0..9 */
    g.AddEdge(p[0], p[1]);
    g.AddEdge(p[0], p[2]);
    g.AddEdge(p[0], p[3]);
    g.AddEdge(p[1], p[5]);
    g.AddEdge(p[2], p[6]);
    g.AddEdge(p[2], p[5]);
    g.AddEdge(p[3], p[4]);
    g.AddEdge(p[4], p[7]);
    g.AddEdge(p[4], p[9]);
    g.AddEdge(p[5], p[8]);
    g.AddEdge(p[6], p[9]);
    g.AddEdge(p[1], p[8]);
    g.AddEdge(p[1], p[7]);
    g.AddEdge(p[1], p[6]);
    g.AddEdge(p[1], p[5]);
    g.AddEdge(p[1], p[9]);

    g2.Setid(2);

    pLEdge ee;

    /* nodes in graph 2 are 0..4 */
    ee = (pLEdge)g2.AddEdge(p2[0], p2[1]);
    printf("added edge in g2 with id %d between node %d and node %d with id %d\n", g2.id(), ee->from()->id(), ee->to()->id(), ee->id());

    ee = (pLEdge)g2.AddEdge(p2[0], p2[2]);
    printf("added edge in g2 between node %d and node %d with id %d\n", ee->from()->id(), ee->to()->id(), ee->id());

    ee = (pLEdge)g2.AddEdge(p2[0], p2[3]);
    printf("added edge in g2 between node %d and node %d with id %d\n", ee->from()->id(), ee->to()->id(), ee->id());

    ee = (pLEdge)g2.AddEdge(p2[3], p2[4]);
    printf("added edge in g2 between node %d and node %d with id %d\n", ee->from()->id(), ee->to()->id(), ee->id());

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

    /* layout graph 1 */
    g.Layout(5, true, 2, true);

    printf("after layout of graph 1 with dummy nodes and changed edges\n");
    g.Dump();

    printf("graph 1 maxrank = %i\n", g.getMaxrank());

    /* layout graph 2 */
    g2.Layout(5, true, 2);

    printf("after layout of graph 2 with dummy nodes and changed edges\n");
    g2.Dump();

    printf("graph 2 maxrank = %i\n", g2.getMaxrank());

    /// graph with no nodes should not crash
    LGraph g3;

    /* layout graph 3 */
    g3.Layout(5, true, 2, true);

    printf("after layout of graph 3 with dummy nodes and changed edges\n");
    g3.Dump();

    // Clean up
    g.Destroy();
    delete[] p;

    g2.Destroy();
    delete[] p2;

    return (0);
}

/* end. */
