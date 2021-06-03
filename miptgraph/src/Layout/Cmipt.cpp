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
 *
 * SPDX-License-Identifier: GPL-3.0+
 * License-Filename: LICENSE
 */

/**
 * @file: Cmipt.cpp
 */

#include "../cmiptgraph.h"
#include "Layout.h"
#include "config.h"

/* return lib version */
extern "C" const char* cmipt_version(void)
{
    return (PACKAGE_VERSION);
}

/* create new graph */
extern "C" void* cmipt_newgraph(void)
{
    pLGraph pg = new LGraph;
    return ((void*)pg);
}

/* delete graph */
extern "C" void* cmipt_deletegraph(void* g)
{
    pLGraph pg;
    if (g) {
        pg = (pLGraph)g;
        delete pg;
    }
    return (NULL);
}

/* create new node */
extern "C" void* cmipt_newnode(void* g)
{
    pLNode p;
    pLGraph pg;
    pg = (pLGraph)g;
    p = pg->AddNode();
    return ((void*)p);
}

/* run layout */
extern "C" void cmipt_layout(void* g, int iter, int transpose, int iter2, int debug)
{
    bool dotr = false;
    bool dodb = false;
    int itera = 0;
    int iterb = 0;
    pLGraph pg;
    pg = (pLGraph)g;
    if (iter > 0) {
        itera = iter;
    } else {
        itera = 100;
    }
    if (iter2 > 0) {
        iterb = iter2;
    } else {
        iterb = 100;
    }
    if (transpose > 0) {
        dotr = true;
    } else {
        dotr = false;
    }
    if (debug > 0) {
        dodb = true;
    } else {
        dodb = false;
    }
    pg->Layout(itera, dotr, iterb, dodb);
    return;
}

/* end. */
