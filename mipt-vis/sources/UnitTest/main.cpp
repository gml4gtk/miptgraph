/**
 * File: UnitTest/main.cpp - Implementation and entry point for unit testing of MiptVis
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utest_impl.h"

int main(int argc, char **argv)
{
	/** Test graph package */
    if ( !uTestGraph())
        return -1;

	if ( !uTestforBFS())
		return -1;

	/** Test List */
    if ( !uTestList())
		return -1;
	/** Test EGraph */
    if ( !uTestEGraph())
        return -1;
	/** Test searching dominators */
	if( !uTestDomSearch())
		return -1;	
	/* Test layout (imho idiotic comment)*/
	if (!uTestLayout())
		return -1;
	/** Test GUI */
    if ( !uTestGui( argc, argv))
		return -1;
    return 0;	
}
