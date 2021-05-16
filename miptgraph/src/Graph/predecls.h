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
 * @file: predecls.h
 */
#ifndef GRAPH_PREDECLS_H
#define GRAPH_PREDECLS_H

#include <algorithm>
#include <assert.h>
#include <list>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

/// Macros for disable copy constructor.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)

class Graph;
class Node;
class Edge;

typedef Graph* pGraph;
typedef Node* pNode;
typedef Edge* pEdge;

#include "Edge.h"
#include "Graph.h"
#include "Node.h"

#endif