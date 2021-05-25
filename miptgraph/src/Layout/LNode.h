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
 *      Rank() to calculate rank
 *      Median() to calculate median
 *      IsAdjacentToNode is test for adjacent of 2 nodes
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
     * final Coordinates for drawing or -1 if undefined
     */
    double x;
    double y;

    /**
     * node x size for label text
     */
    int nxsize;

    /**
     * node y size for label text
     */
    int nysize;

    /**
     * if a dummy node this is the from node of the edge the dummy node belongs to
     */
    pNode origfrom;

    /**
     * if a dummy node this is the to node of the edge the dummy node belongs to
     */
    pNode origto;

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
     * return final x coord
     */
    double getX() { return x; }

    /**
     * return final y coord
     */
    double getY() { return y; }

    /**
     * Calculate node rank
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
    double Median(Ordering order, bool direction);

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
	nxsize = 50; // default node x size
	nysize = 50; // default node y size
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
