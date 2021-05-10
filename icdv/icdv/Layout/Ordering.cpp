/** 
 * @file: Ordering.cpp
 */

#include "Layout.h"

vector<int> Ordering::AdjInPositions(pLNode node) {
        vector<int> positions;
	int rank = node->Rank();
	if (rank > 0)
                for (unsigned int i = 0; i < order_vector[rank - 1].size(); i++) {
			if (order_vector[rank - 1][i]->IsAdjacentToNode(node))
                                positions.push_back(i);
		}
	return positions;
}

vector<int> Ordering::AdjOutPositions(pLNode node) {
        vector<int> positions;
        unsigned int rank = node->Rank();
	if (rank < order_vector.size() )
                for (unsigned int i = 0; i < order_vector[rank + 1].size(); i++) {
			if (node->IsAdjacentToNode(order_vector[rank + 1][i]))
                                positions.push_back(i);
		}
	return positions;
}


void Ordering::Dump(){
        for (unsigned int rank = 0; rank < order_vector.size(); rank++){
		printf("rank = %d\n",rank);
		for (unsigned int i = 0; i < order_vector[rank].size(); i++){
                        printf("%d(%d,%d) ",
                               order_vector[rank][i]->id(),
                               order_vector[rank][i]->getX(),
                               order_vector[rank][i]->getY());

		}
		printf("\n");
	}
}
