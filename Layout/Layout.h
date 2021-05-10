/** 
 * @file: Layout.h
 */

#ifndef LAYOUT_H
#define LAYOUT_H

#include "../Graph/predecls.h"

class LGraph;
class LNode;
class LEdge;
class Ordering;

typedef LGraph* pLGraph;
typedef LNode*  pLNode;
typedef LEdge*  pLEdge;

#include "LNode.h"
#include "LEdge.h"
#include "LGraph.h"
#include "Ordering.h"

#define MEDIAN_IN  1
#define MEDIAN_OUT 0

void LayoutTest();
#endif
