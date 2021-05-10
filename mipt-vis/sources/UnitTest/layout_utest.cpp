/**
 * File: layout_utest.cpp - Implementation of testing of layout library, 
 * internal graph placement component in MiptVis tool.
 * Copyright (C) 2009 MiptVis
 */

#include "utest_impl.h"

bool uTestLayout()
{
	GraphAux ga;
	
    char * file = _getcwd( NULL, 1024);
    strcat_s( file, 1024, "/test_graph.xml");

	ga.readFromXml (file);
	out ("\n============= testing Layout =============\nbefore ranking:\n");
	ga.debugPrint();
	
	if (ga.ranking())
	{
		out ("\nafter ranking:\n");
		ga.debugPrint();
	}
	else
		out ("\n Error: Ranking filed");

	if ( ga.ordering())
	{
		out ( "\nafter ordering:\n");
		ga.debugPrint();
	}
	else
		out ( "\nError while ordering");

	out ("\n=========== end of layout test ===========\n");
	return true;
}