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
 * @file: LNode.h
 */

#ifndef LAYOUT_NODE_H
#define LAYOUT_NODE_H

/**
 * Extention of class Node for layout
 * This node class contains new variable:
 * rank   - vertical relative position of node in the graph
 * pos    - horisontal relative position
 * median - temp value to calculate pos
 * Also there is some methods:
 * Rank() to calculate rank or return already calculated one
 * Median() to calculate median
 * IsAdjacentToNode is test for adjacent of 2 nodes
 */

class LNode : public Node {
private:

    /**
     * vertical relative y rank level in the graph or -1 if undefined
     */
    int rank;

    /**
     * horizontal relative x position in a rank level or -1 if undefined
     */
    int pos;

    /**
     * median average value of node depending on linkage with nodes upper and lower
     */
    double median;

    /**
     * Flag for the LNode::Rank()
     */
    bool we_were_here;

    /**
     * if true this is a dummy node
     */
    bool dummy;

    /**
     * x initial Coordinate using simple placement for drawing or -1 if undefined
     */
    double x;

    /**
     * y initial Coordinate using simple placement for drawing or -1 if undefined
     */
    double y;

    /**
     * x2 final Coordinate for drawing or -1 if undefined
     */
    double x2;

    /**
     * y2 final Coordinate for drawing or -1 if undefined
     */
    double y2;

    /**
     * node x size for label text
     */
    int nxsize;

    /**
     * node y size for label text
     */
    int nysize;

    /**
     * node x center
     */
    int ncenterx;

    /**
     * node y center
     */
    int ncentery;

    /**
     * if a dummy node this is the from node of the edge the dummy node belongs to
     */
    pNode origfrom;

    /**
     * if a dummy node this is the to node of the edge the dummy node belongs to
     */
    pNode origto;

    /**
     * node left from this node in order at rank
     */
    pLNode lnode;

    /**
     * node right from this node in order at rank
     */
    pLNode rnode;

public:

    /**
     * if True, if it's an entry node without incoming edges
     */
    bool entry;

    /**
     * return true if this is a dummy node
     */
    bool IsDummy() { return dummy; }

    /**
     * return initial x coord
     */
    double getX() { return x; }

    /**
     * return initial y coord
     */
    double getY() { return y; }

    /**
     * return final x coord
     */
    double getXF() { return x2; }

    /**
     * return final y coord
     */
    double getYF() { return y2; }

    /**
     * Calculate node rank relative y position or return calculated one
     */
    int Rank();

    /**
     * get barycenter value
     */
    double getMedian() { return median; }

    /**
     * get relative x postion
     */
    int getPos() { return pos; }

    /**
     * set node size
     */
    void setNodeSize (int xsize, int ysize) { nxsize = xsize; ncenterx = xsize / 2; nysize = ysize; ncentery = ysize / 2; }

    /**
     * get node x size
     */
    int getxsize () { return nxsize; }

    /**
     * get node y size
     */
    int getysize () { return nysize; }

    /**
     * get node x center
     */
    int getxcenter () { return ncenterx; }

    /**
     * get node y center
     */
    int getycenter () { return ncentery; }

    /** Calculate weighed median for the node.
     * The median value of a vertex is defined as the median position of the
     * adjacent vertices if that is uniquely defined. Otherwise, it is interpolated
     * between the two median positions using a measure of tightness. Generally,
     * the weighted median is biased toward the side where vertices are more
     * closely packed.
     *
     * If direction == true then funcrion calculate IN MEDIAN
     * if false then OUT MEDIAN
     */
    double Median(Ordering order, bool direction, bool bary);

    /**
     * Return true if node is adjacent to this node
     */
    bool IsAdjacentToNode(pLNode node);

    /**
     * create node with defaults
     */
    LNode(pLGraph pg)
        : Node((pGraph)pg)
    {
	x = -1; // no x coord
	y = -1; // no y coord
	x2 = -1; // no x coord
	y2 = -1; // no y coord
	nxsize = 10; // default node x size
	nysize = 10; // default node y size
	rank = -1; // no rank
	pos = -1; // no x pos
	median = 0; // no barycenter value
	we_were_here = false;
	dummy = false; // this is a real node
	entry = false; // not startnode
	origfrom = NULL; // edge point
	origto = NULL; // edge point
    }

    virtual ~LNode();

    friend class LEdge;
    friend class LGraph;

protected:

};

#endif

/* end. */
