/** 
 * @file: LGraph.h
 */

#ifndef LAYOUT_GRAPH_H
#define LAYOUT_GRAPH_H

/// Graph with Layout methods
/**
 * Includes ranking, median and transposition heuristic
 */

class LGraph: public Graph {
private:
	unsigned int maxrank;

        bool layouted; // Are the graph already layouted.
        Ordering *order;

public:
	LGraph(){
		maxrank = 0;
                order = NULL;
	}
	/// Replacing revert eages from the ReverceEdges list.
	void ReverseReverseEdges(list<pEdge> &ReverseEdges);

	int getMaxrank() {return maxrank; }
	
	/**
	 * This main Layout function do whole procedure of LAYOUT:
	 * 1. Reversing "Reverse" edges
	 * 2. Initing Rank for all nodes
	 * 3. Find and breaks "long" edges
	 * 4. Makes ordering (horisontal placement)
	 * 5. ...
	 */
        void Layout(unsigned int number_of_iterations = 3,
                    bool do_transpose = true,
                    int transpose_range = -1);

	/**
	 * Init Rank value for each node.
	 * 
	 * rank = 0 for "head" nodes;
	 * rank = max(rank(adj_nodes)) for others.
	 */
	virtual void InitRank();
	
	/// Breaks all "long" edges.
	void AddDummyNodes(list<pEdge> &LongEdges);

	/// Finds all "long" edges in graph and puts it to the LondEdges list.
	void FindLongEdges(list<pEdge> &LongEdges);
	
        /// Init pos value for each node using order.
        void InitPos(Ordering *order);

        /// Init coordiates for each node.
        void InitCoordinates(Ordering *order,
                             int normalwide = 50,
                             int dummywide = 25,
                             int vertical_size = 80);

	/**
	 * The weighted median heuristic for reducing edge crossings.
	 * At each rank a vertex is assigned a median based on the adjacent 
	 * vertices on the previous rank. Then, the vertices in the rank are 
	 * sorted by their medians.
	 */
        void WeightedMedianHeuristic(int iter);

	/**
	 * The transposition heuristic for reducing edge crossings.
	 * Transpose repeatedly exchanges adjacent vertices on the 
	 * same rank if this decreases the number of crossings.
	 */
        void Transpose(int max);
	
	/// Calculate all edges crossings in the whole graph.
        int countCrossing(Ordering *order);

	
	/// Calculate all crossings between rank an rank+1.
        int countCrossingOnRank(Ordering *order,int rank);
	
	/**
	 * The initial (horisontal)ordering.
	 * Should be called after InitRank().
	 */	 
	vector< vector<pLNode> > InitOrder();
	virtual pLNode AddNode();
        virtual pLEdge AddEdge(pNode from, pNode to);
	virtual void FreeNode(pNode p);
	virtual void FreeEdge(pEdge p);

    friend class  LEdge;
    friend class  LNode;
    friend class  Ordering;
};

#endif
