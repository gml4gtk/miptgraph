/**
 * File: misc.h - assertion related routines of Utils library for MiptVis
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef ASRT_H
#define ASRT_H

/**
 * Type of error
 */
enum ErrorT
{
    ERROR_GENERIC
};

/**
 * Error's severity
 */
enum Severity
{
    CRITICAL
};

/**
 * Error description
 */
class Error
{
    ErrorT type;
    Severity sev;
public:
    Error()
    {
        type = ERROR_GENERIC;
        sev = CRITICAL;
    }
};

/**
 * Generic assertion template
 *
template<class Excpt> inline void assert( bool assertion)
{
    if ( !assertion)
    {
        throw Excpt();
    }
}
*/
/**
 * assertion template parameterized with thrown exception type
 wrong this does not work
 *
template<class Excpt> inline void assert( bool asrt, Excpt e)
{
    if ( !asrt)
    {
        throw e;
    }
}
*/

#if 0
/**
 * Generic assert
 */
inline void assert( bool asrt)
{
/*    return assert< int>( asrt);*/
}
#endif
#undef assert
#define assert(var) /**/

#endif
