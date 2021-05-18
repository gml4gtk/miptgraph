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

/// Extention of class Node for layout
/**
 * This node class contains new variable:
 *		rank   - vertical relative position of node in the graph
 *		pos    - horisontal relative position
 *		median - temp value to calculate pos
 * Also there is some methods:
 *      Rank() to calculate rank
 *      Median() to calculate median
 *      IsAdjacentToNode is test for adjesent of 2 nodes
 */

//#include <QString>

class LNode : public Node {
private:
    int rank;
    int pos;
    double median;

    /// Flag for the LNode::Rank()
    bool we_were_here;

    bool dummy;

    // Coordinates for drawing
    double x;
    double y;

public:
    // True, if it's an entry node.
    bool entry;

    // Number
    unsigned int uBlkId;
    list<unsigned int> m_preds_lst;
    list<unsigned int> m_succs_lst;
    list<unsigned int> m_exec_lst;
    list<unsigned int> m_prob_lst;

    const std::string* Content() { return &content; }

    void SetContent(const std::string& source)
    {
        //        content.clear();
        content += source;
    }

    bool IsDummy() { return dummy; }

    double getX() { return x; }
    double getY() { return y; }

    /// Calculate node rank
    int Rank();

    double getMedian() { return median; }
    int getPos() { return pos; }

    /** Calculate weighed median for the node.
   * The median value of a vertex is defined as the median position of the
   *adjacent vertices if that is uniquely defined. Otherwise, it is interpolated
   *between the two median positions using a measure of tightness. Generally,
   *the weighted median is biased toward the side where vertices are more
   *closely packed.
   *
   * If direction == true then funcrion calculate IN MEDIAN
   *	if false then OUT MEDIAN
   */
    double Median(Ordering order, bool direction);

    /// Return true if node is adjacent to this node
    bool IsAdjacentToNode(pLNode node);

    LNode(pLGraph pg)
        : Node((pGraph)pg)
    {
        x = y = -1;
        rank = -1;
        median = 0;
        we_were_here = false;
        dummy = false;
    }
    friend class LEdge;
    friend class LGraph;

protected:
    // Content of basic block.
    std::string content;
};

#endif
