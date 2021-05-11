#ifndef EDGE_AUX_H
#define EDGE_AUX_H

/**
 * Auxiliary edge representation class. 
 */

class EdgeAux : public Edge
{
    friend class GraphAux;
    friend class NodeAux;

	enum
	{
		tree,
		forward,
		back,
		mesh //the such case: A->A
	} type;

protected:
    EdgeAux( GraphAux *graph_p, int _id, NodeAux* _pred, NodeAux* _succ);
	virtual void debugPrint();
public:
    inline EdgeAux* nextEdge()
	{
		return static_cast< EdgeAux*> ( Edge::nextEdge());
	}
	inline EdgeAux* nextPred()
	{
		return static_cast< EdgeAux*> ( Edge::nextPred());
	}
	inline EdgeAux* nextSucc()
	{
		return static_cast< EdgeAux*> ( Edge::nextSucc());
	}
	inline NodeAux* getNode( GraphDir dir);
	inline NodeAux* succ();
	inline NodeAux* pred();

	bool isVirtualRootEdge();

	
	/*
	 * These functions are not take into a mesh edges
	 */
	inline bool ahead() const
	{
		return type == EdgeAux::tree || type == EdgeAux::forward;
	}
	inline bool backward() const
	{
		return type == EdgeAux::back;
	}
	inline bool cycle() const
	{
		return type == EdgeAux::mesh;
	}

};

#endif
