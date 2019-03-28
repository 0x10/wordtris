/*!*****************************************************************************
 * @file dea_wrapper.cpp
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

#include "dea_wrapper.h"

extern "C" {
#include "dea.h"
}

/**************************************
 *
 *************************************/
void DeaWrapper::process( dea_t* dea, const char symbol, unsigned char verbose )
{
    process_symbol( dea, symbol, verbose );
}

/**************************************
 *
 *************************************/
void DeaWrapper::init( dea_t* dea )
{
    init_dea( dea );
}

/**************************************
*
*************************************/
void DeaWrapper::free( dea_t* dea )
{
    free_dea( dea );
}

/**************************************
*
*************************************/
void DeaWrapper::print( dea_t* d )
{
    print_dea( d );
}


/**************************************
 *
 *************************************/
dea_t* DeaWrapper::construct_contains( const std::string argv )
{
    return new_contains( argv.c_str(), argv.length() );
}

