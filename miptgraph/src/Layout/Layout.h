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

#endif

/* end. */
