/**
 * File: enode.cpp - ENode class implementation, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#include "analysis_impl.h"

ENode::ENode( EGraph *graph, int _id):
    Node( static_cast<Graph *>( graph), _id)
{
    idom = NULL;
    pdom = NULL;
	parent = NULL;
	ancestor = NULL;
	semi = -1;
}
