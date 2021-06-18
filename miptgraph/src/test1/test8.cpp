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
 * @file: test8.cpp
 */

#include "../Layout/Layout.h"

/**
* Create a LGraph object and call some of its methods.
*/

int main(int argc, char* argv[])
{
    if (argc) {
    }
    if (argv) {
    }
    printf("\nLayout test started..\n");

    LGraph g;

    /* 3 type 1 edge conflicts, 19 nodes
digraph G {
n2 -> n13
n2 -> n19
n1 -> n2
n1 -> n5
n1 -> n6
n1 -> n16
n5 -> n3
n5 -> n17
n6 -> n4
n6 -> n8
n6 -> n16
n16 -> n1
n13 -> n2
n3 -> n19
n4 -> n17
n4 -> n1
n8 -> n0
n8 -> n11
n18 -> n17
n18 -> n6
n7 -> n18
n11 -> n10
n11 -> n18
n11 -> n8
n9 -> n7
n9 -> n17
n10 -> n0
n10 -> n16
n10 -> n15
n12 -> n5
n12 -> n17
n14 -> n16
n15 -> n10
n15 -> n16
}

graph [
node [ id 0 ]
node [ id 1 ]
node [ id 2 ]
node [ id 3 ]
node [ id 4 ]
node [ id 5 ]
node [ id 6 ]
node [ id 7 ]
node [ id 8 ]
node [ id 9 ]
node [ id 10 ]
node [ id 11 ]
node [ id 12 ]
node [ id 13 ]
node [ id 14 ]
node [ id 15 ]
node [ id 16 ]
node [ id 17 ]
node [ id 18 ]
node [ id 19 ]
edge [ source 2 target 13 ]
edge [ source 2 target 19 ]
edge [ source 1 target 2 ]
edge [ source 1 target 5 ]
edge [ source 1 target 6 ]
edge [ source 1 target 16 ]
edge [ source 5 target 3 ]
edge [ source 5 target 17 ]
edge [ source 6 target 4 ]
edge [ source 6 target 8 ]
edge [ source 6 target 16 ]
edge [ source 16 target 1 ]
edge [ source 13 target 2 ]
edge [ source 3 target 19 ]
edge [ source 4 target 17 ]
edge [ source 4 target 1 ]
edge [ source 8 target 0 ]
edge [ source 8 target 11 ]
edge [ source 18 target 17 ]
edge [ source 18 target 6 ]
edge [ source 7 target 18 ]
edge [ source 11 target 10 ]
edge [ source 11 target 18 ]
edge [ source 11 target 8 ]
edge [ source 9 target 7 ]
edge [ source 9 target 17 ]
edge [ source 10 target 0 ]
edge [ source 10 target 16 ]
edge [ source 10 target 15 ]
edge [ source 12 target 5 ]
edge [ source 12 target 17 ]
edge [ source 14 target 16 ]
edge [ source 15 target 10 ]
edge [ source 15 target 16 ]
]

*/

    /* 0..19 are 20 nodes */
    int len = 20;
    pLNode* p = new pLNode[len];

    // Creating a new graph
    for (int i = 0; i < len; i++) {
        p[i] = (pLNode)g.AddNode();
    }

    // Performing some transformations
    g.AddEdge(p[2], p[13], NULL);
    g.AddEdge(p[2], p[19], NULL);
    g.AddEdge(p[1], p[2], NULL);
    g.AddEdge(p[1], p[5], NULL);
    g.AddEdge(p[1], p[6], NULL);
    g.AddEdge(p[1], p[16], NULL);
    g.AddEdge(p[5], p[3], NULL);
    g.AddEdge(p[5], p[17], NULL);
    g.AddEdge(p[6], p[4], NULL);
    g.AddEdge(p[6], p[8], NULL);
    g.AddEdge(p[6], p[16], NULL);
    g.AddEdge(p[16], p[1], NULL);
    g.AddEdge(p[13], p[2], NULL);
    g.AddEdge(p[3], p[19], NULL);
    g.AddEdge(p[4], p[17], NULL);
    g.AddEdge(p[4], p[1], NULL);
    g.AddEdge(p[8], p[0], NULL);
    g.AddEdge(p[8], p[11], NULL);
    g.AddEdge(p[18], p[17], NULL);
    g.AddEdge(p[18], p[6], NULL);
    g.AddEdge(p[7], p[18], NULL);
    g.AddEdge(p[11], p[10], NULL);
    g.AddEdge(p[11], p[18], NULL);
    g.AddEdge(p[11], p[8], NULL);
    g.AddEdge(p[9], p[7], NULL);
    g.AddEdge(p[9], p[17], NULL);
    g.AddEdge(p[10], p[0], NULL);
    g.AddEdge(p[10], p[16], NULL);
    g.AddEdge(p[10], p[15], NULL);
    g.AddEdge(p[12], p[5], NULL);
    g.AddEdge(p[12], p[17], NULL);
    g.AddEdge(p[14], p[16], NULL);
    g.AddEdge(p[15], p[10], NULL);
    g.AddEdge(p[15], p[16], NULL);

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

    printf("before layout input graph data\n");
    g.Dump();

    g.Layout(500, true, 100, /* debug */ false);

    printf("after layout with dummy nodes and changed edges\n");
    g.Dump();

    printf("maxrank = %i\n", g.getMaxrank());

    /* border around drawing */
    int xborder = 10;
    int yborder = 10;

    /* size of drawing */
    int maxx = 0;
    int maxy = 0;

    /* print the node positioning */
    for (list<pNode>::iterator node_iter = g.nodes_list()->begin();
         node_iter != g.nodes_list()->end();
         node_iter++) {
        unsigned int rank = ((LNode*)(*node_iter))->Rank();
        unsigned int pos = ((LNode*)(*node_iter))->getPos();
        double x = ((LNode*)(*node_iter))->getX();
        double y = ((LNode*)(*node_iter))->getY();
        bool dum = ((LNode*)(*node_iter))->IsDummy();
        if ((int)x > maxx) {
            maxx = (int)x;
        }
        if ((int)y > maxy) {
            maxy = (int)y;
        }
        printf("node id %d: relative pos (%d,%d) absolute pos (%d,%d) dummy=%d\n", ((LNode*)(*node_iter))->id(), pos, rank, (int)x, (int)y, dum);
    }

    printf("Layout test passed!\nThis is the image svg data:\n\n");

    printf("<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:svg=\"http://www.w3.org/2000/svg\">\n",
        maxx + 2 * xborder, maxy + 2 * yborder);
    printf("%s\n", " <g>");
    printf("%s\n", "  <title>miptgraph</title>");
    printf("%s\n", "  <style>");
    printf("%s\n", "    .small { font: italic 15px sans-serif; }");
    printf("%s\n", "  </style>");

    list<pEdge>::iterator edge_iter;

    double xf = 0;
    double yf = 0;
    double xt = 0;
    double yt = 0;

    for (edge_iter = g.edges_list()->begin();
         edge_iter != g.edges_list()->end();
         edge_iter++) {
        pEdge pe = *edge_iter;
        if (pe) {
            /* get from, to coords of nodes in this edge */
            xf = ((pLNode)(*edge_iter)->from())->getX();
            yf = ((pLNode)(*edge_iter)->from())->getY();

            xt = ((pLNode)(*edge_iter)->to())->getX();
            yt = ((pLNode)(*edge_iter)->to())->getY();
            printf("  <!-- edge id %d edge from node %d->%d reversed=%d -->\n", pe->id(), ((pLNode)(*edge_iter)->from())->id(), ((pLNode)(*edge_iter)->to())->id(), pe->IsReverse());

            printf("  <line id=\"edge_%d\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke-width=\"2\" ",
                pe->id(), (int)xf + xborder, (int)yf + yborder, (int)xt + xborder, (int)yt + yborder);

            if (pe->IsReverse()) {
                /* draw arrow at from-node */
                printf("stroke=\"%s\" fill=\"none\"/>\n", "#0f0fff");
            } else {
                /* draw arrow at to-node */
                printf("stroke=\"%s\" fill=\"none\"/>\n", "#696969");
            }
        }
    }

    for (list<pNode>::iterator node_iter = g.nodes_list()->begin();
         node_iter != g.nodes_list()->end();
         node_iter++) {
        double x = ((LNode*)(*node_iter))->getX();
        double y = ((LNode*)(*node_iter))->getY();
        bool dum = ((LNode*)(*node_iter))->IsDummy();
        int id = ((LNode*)(*node_iter))->id();
        int xsize = 20;
        int ysize = 20;
        int ty = 10;
        if (dum) {
            /* this is a dummy node */
            printf("  <rect id=\"node_%d\" width=\"%d\" height=\"%d\" x=\"%d\"  y=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#000000\" fill=\"#ff0000\"/>\n",
                id, xsize / 4, ysize / 4, (int)x + xborder, (int)y + yborder);
        } else {
            /* this is a regular node from input */
            printf(
                "  <rect id=\"node_%d\" width=\"%d\" height=\"%d\" x=\"%d\"  y=\"%d\" stroke-linecap=\"null\" stroke-linejoin=\"null\" stroke-dasharray=\"null\" stroke=\"#000000\" fill=\"#7fff00\"/>\n",
                id, xsize, ysize, (int)x - xborder / 2, (int)y - yborder / 2);
            printf("  <text x=\"%d\" y=\"%d\" class=\"small\">%d</text>\n", (int)x - xborder / 2 + 5, (int)y - yborder / 2 + 5 + ty, id);
        }
    }

    printf("%s\n", " </g>");
    printf("%s\n", "</svg>");

    // Clean up
    g.Destroy();
    delete[] p;

    return (0);
}

/* end. */
