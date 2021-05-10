/** 
 * @file: LEdge.cpp
 */

#include "Layout.h"

void LEdge::BreakLongEdge(){

    ///Difference between rank of the "in" and "out"  nodes
    int n = ((pLNode) to())->Rank() - ((pLNode) from())->Rank();

    list<pLEdge> *edges_list = new list<pLEdge>;

    pLNode prevnode = ((pLNode) from());
    pLNode newnode;

    bool is_reverse = reverse;
    pEdge new_edge;
    ///Adding n-1 nodes to do all rank differences equal 1
    for (int i = 1; i < n; i++) {
        newnode = (pLNode) graph()->AddNode();
        newnode->dummy = true;
        newnode->rank = (((pLNode) from())->Rank()) + i;
        new_edge = graph()->AddEdge(prevnode, newnode);
        ((pLEdge)(new_edge))->SetComposite(true);
        new_edge->SetReverse(is_reverse);
        edges_list->push_back((pLEdge) new_edge);
        prevnode = newnode;
    }

    new_edge = graph()->AddEdge(prevnode, to());
    new_edge->SetReverse(is_reverse);
    ((pLEdge)new_edge)->SetComposite(true);
    edges_list->push_back((pLEdge) new_edge);
    graph()->DeleteEdge(from(), to());

    for(list<pLEdge>::iterator edge_iter = edges_list->begin();
            edge_iter != edges_list->end();
            edge_iter++)
        ((pLEdge) *edge_iter)->m_composite_edges = edges_list;
    composite = true;
}

void LEdge::CompositeEdgesDump() {
    printf("Edge: %d. Composite Edges:\n",id());
    if (m_composite_edges != NULL) {
        for(list<pLEdge>::iterator edge_iter = m_composite_edges->begin();
            edge_iter != m_composite_edges->end();
            edge_iter++)
            printf("%d   ", (*edge_iter)->id());
    }
    printf("\n");
}

LEdge::~LEdge() {
    if (m_composite_edges != NULL){
        list<pLEdge> *temp_list = m_composite_edges;
        for(list<pLEdge>::iterator edge_iter = temp_list->begin();
            edge_iter != temp_list->end();
            edge_iter++){
                    ((pLEdge) *edge_iter)->m_composite_edges = NULL;
            }
   delete temp_list;
   }
}
