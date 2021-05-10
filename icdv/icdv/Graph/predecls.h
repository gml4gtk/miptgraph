/** 
 * @file: predecls.h
 */
#ifndef GRAPH_PREDECLS_H
#define GRAPH_PREDECLS_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

/// Macros for disable copy constructor.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class Graph;
class Node;
class Edge;

typedef Graph* pGraph;
typedef Node*  pNode;
typedef Edge*  pEdge;

#include "Graph.h"
#include "Node.h"
#include "Edge.h"

#endif