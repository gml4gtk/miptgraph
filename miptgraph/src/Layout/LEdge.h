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
 * @file: LEdge.h
 */

#ifndef LAYOUT_EDGE_H
#define LAYOUT_EDGE_H

/**
 * Edge with Layout methods
 *
 * This class contains one new variable: reverse
 * And method BreakLongEdge() for better drawing long edges
 */
class LEdge : public Edge {


private:

    /**
     * set to true if this is a split edge
     */
    bool composite;

    /**
     * set to true if this is a inner edge between two dummy nodes
     */
    bool inner;

    /**
     * set to true if this is a type 1 conflicting edge
     */
    bool conflict;

    /**
     * set true is this is head edge of a split edge
     */
    bool splitedgehead;

    /**
     * set true is this is tail edge of a split edge
     */
    bool splitedgetail;

    /**
     * counter if inner edge crosses with another inner edge
     */
    int iicross;

    /**
     * counter if inner edge crosses with another not-inner edge
     */
    int ircross;

    /**
     * counter if not-inner edge crosses with another not-inner edge
     */
    int rrcross;

public:


    /**
     * create edge with defaults
     */
    LEdge(pLNode from, pLNode to)
        : Edge((pNode)from, (pNode)to)
    {
	if (from->id() == to->id()) {
	    // this is a self-edge handled in edge creation
	}
	splitedgehead = false;
	splitedgetail = false;
	conflict = false;
	composite = false;
	reverse = false;
	hedge = false;
	iicross = 0;
	ircross = 0;
	rrcross = 0;
	inner = false;
    }

    virtual ~LEdge();

    friend class LGraph;
    friend class LNode;

    /**
     * Long edge is one with length(L) more than one rank.
     * This method makes L-1 dummy nodes and L edges between them
     */
    void BreakLongEdge();

    /**
     * return bool true if there are composite edges
     * this is a edge which is split from long edge
     */
    bool Composite() { return composite; }

    /**
     * set edge as composite edge
     * this is a edge which is split from long edge
     */
    void SetComposite(bool is_composite) { composite = is_composite; }

    /**
     * return true if this is head of a split edge
     */
    bool IsHeadSplitEdge() { return splitedgehead; }

    /**
     * return true if this is tail of a split edge
     */
    bool IsTailSplitEdge() { return splitedgetail; }

    /**
     * return number of inner with non-inner crossings at this edge
     */
    int n_ircross() { return ircross; }

    /**
     * set edge conflict status
     */
    void setconflict (bool status) { conflict = status; }
};

#endif

/* end. */
