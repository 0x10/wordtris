/*!*****************************************************************************
 * @file dea_wrapper.h
 * @brief cpp wrapper for dea.c
 *
 * This file was developed as part of dea regex
 *
 * @author Christian Kranz
 * 
 * @copyright Copyright 2019 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz
 *
 ******************************************************************************/
#ifndef _DEA_WRAPPER_H_
#define _DEA_WRAPPER_H_

extern "C" {
#include "dea_common.h"
}

#include <string>

/**************************************
 *
 *************************************/
class DeaWrapper
{
public:
    /**************************************
     *
     *************************************/
    static void process( dea_t* dea, const char symbol, unsigned char verbose = 0 );
    /**************************************
     *
     *************************************/
    static void init( dea_t* dea );
    /**************************************
    *
    *************************************/
    static dea_t* construct_contains( const std::string w );
    /**************************************
    *
    *************************************/
    static void free( dea_t* dea );
};

#endif /* _DEA_WRAPPER_H */
