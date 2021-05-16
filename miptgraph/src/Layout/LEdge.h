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

/// Edge with Layout methods
/** TODO(Lega) : improve docs.
 *
 * This class contains one new variable: reverse
 * And method BreakLongEdge() for better drawing long edges
 */
class LEdge : public Edge {
private:
    list<pLEdge>* m_composite_edges;
    bool composite;

public:
    const list<pLEdge>* composite_edges() { return m_composite_edges; }

    LEdge(pLNode from, pLNode to)
        : Edge((pNode)from, (pNode)to)
    {
        composite = false;
        reverse = false;
        m_composite_edges = NULL;
    }

    // TODO(Lega): this function produces a lot of memory leaks
    // and segfault at the end of execution. Try to fix it.
    ~LEdge();

    friend class LGraph;
    friend class LNode;

    /**
   *  Lond edge is one with length(L) more than one rank.
   *	This method makes L-1 dummy nodes and L edges between them
   *  to do this edge shorter
   */
    void BreakLongEdge();
    void CompositeEdgesDump();

    bool Composite() { return composite; }

    void SetComposite(bool is_composite) { composite = is_composite; }
};

#endif
