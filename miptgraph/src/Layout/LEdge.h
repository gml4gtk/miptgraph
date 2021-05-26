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
     * set to tru if this is a split edge
     */
    bool composite;

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
        composite = false;
        reverse = false;
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

};

#endif

/* end. */
