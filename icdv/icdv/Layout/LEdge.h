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
class LEdge: public Edge {
  private:
    list<pLEdge> * m_composite_edges;
    bool composite;

  public:

        const list<pLEdge> * composite_edges() {
            return m_composite_edges;
        }

        LEdge(pLNode from, pLNode to): Edge((pNode)from, (pNode)to) {
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

        bool Composite() {
            return composite;
        }

        void SetComposite(bool is_composite) {
            composite = is_composite;
        }
};

#endif
