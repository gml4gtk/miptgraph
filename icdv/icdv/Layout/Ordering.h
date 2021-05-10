/** 
 * @file: Ordering.h
 */

#ifndef LAYOUT_ORDERING_H
#define LAYOUT_ORDERING_H

/// Class for representation horisontal order of nodes in each rank
/**
 *	Ordering contains vector of ranks.
 *  Each rank contain ordered list of the nodes.
 */

class Ordering {
  private:
        vector< vector<pLNode> > order_vector;
  public:
	Ordering() {}
	~Ordering() {}
	
	void Dump();

	// The adj_position function returns an ordered array 
	// of the present positions of the nodes adjacent to node.
        vector<int> AdjInPositions(pLNode node);
        vector<int> AdjOutPositions(pLNode node);

        friend class LGraph;
};

#endif // LAYOUT_ORDERING_H
