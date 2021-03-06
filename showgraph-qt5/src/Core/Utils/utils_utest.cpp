/**
 * @file: utils_utest.cpp 
 * Implementation of testing of utils
 */
/*
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "utils_iface.h"


#define ASSERT(v) /**/

/** Sample class */
class classA
{
private:
    /** private constructors, assignment and destructor */
    classA(){};
    classA( const classA&){};
    classA& operator =( const classA&){};
    ~classA(){};
    /** Needed for singleton creation */
    friend class Single< classA>;
};

/** Typedef for classA encapsulated in singleton */
typedef Single< classA> SingleA;

/**
 * Test simple singleton
 */
bool uTestSingle()
{
    SingleA::init();

    classA* a1 = SingleA::instance();
    classA* a2 = SingleA::instance();
/*    ASSERT( areEqP( a1, a2));*/
    SingleA::deinit();
    return true;
}

/**
 * Test Utils package
 */
bool uTestUtils()
{
    /** test singleton */
    if ( !uTestSingle())
        return false;

    /** Tets list classes */
    if ( !uTestList())
        return false;

    /** Test memory management */
    if ( !uTestMem())
		return false;

    /** Test configuration classes functionality */
    if ( !uTestConf())
    	return false;
	return true;
}
