/**
 * File: uTest_impl.h - Header for unit testing of libraries and tools of MiptVis
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef UTEST_IMPL_H
#define UTEST_IMPL_H

#include <stdio.h>
#include "../Graph/graph_iface.h"
#include "../Utils/utils_iface.h"
#include "../GUI/gui_iface.h"
#include "../Layout/layout_iface.h"
#include "../Analysis/analysis_iface.h"

/**
 * Graph library unit testing
 */
bool uTestGraph();
bool uTestGui( int argc, char * argv[]);
bool uTestforBFS();
bool uTestEGraph();
bool uTestDomSearch();
/**
* Layout library unit testing
*/
bool uTestLayout();
/**
 * Utils library unit testing
 */
bool uTestList();

#endif
