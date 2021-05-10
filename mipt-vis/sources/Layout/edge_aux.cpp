#include "layout_iface.h"

//-----------------------------------------------------------------------------
EdgeAux::EdgeAux( GraphAux *graph_p, int _id, NodeAux* _pred, NodeAux* _succ):
    Edge( static_cast< Graph*>( graph_p), _id,  static_cast< Node*>( _pred),  static_cast< Node*>( _succ)),
	type (tree)
{}

/**
* Checks is this edge follows from virtual root
*/
bool EdgeAux::isVirtualRootEdge()
{
	NodeAux* pre = addAux ( pred());
	while (!pre->real() && pre->firstPred() != 0) //follow back throug adge
		pre = pre->firstPred()->pred();
	if (pre->real()) return false;
	return true;
}
//-----------------------------------------------------------------------------
void EdgeAux::debugPrint()
{
	Edge::debugPrint();
	switch (type)
	{
	case tree:
		out (" tree");
		break;
	case mesh:
		out (" loop");
		break;
	case back:
		out (" back");
		break;
	case forward:
		out (" forward");
		break;
	default:
		out (" wrong type");
	};
}
//-----------------------------------------------------------------------------