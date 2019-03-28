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

#define ESCAPE_CHAR 0x5C

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
dea_t* DeaWrapper::construct_contains( const std::string w )
{
    dea_t* new_d = NULL;
    size_t l = w.length();
    const char* argv = w.c_str();

    if ( l > 0 )
    {
        int was_escape =0;
        size_t i;

        new_d = new_empty_dea();
        for ( i=0; i<l; i++ )
        {
            if ( ( argv[i] == ESCAPE_CHAR ) && ( was_escape == 0))
            {
                was_escape = 1;
            }
            else
            {
                dea_state_t* new_s = new_state( new_d, NOT_ACCEPTING );
            
                dea_char_type_t symbol_type = CHAR;
                if ( was_escape == 1 )
                {
                    was_escape = 0;
                    if ( ( argv[i] == ANY_WHITESPACE ) || ( argv[i] == ANY_DIGIT ) )
                    {
                        symbol_type = SPECIAL;
                    }

                }
                else 
                {
                    if ( argv[i] == ANY_SYMBOL )
                    {
                        symbol_type = SPECIAL;
                    }
                }

                dea_input_symbol_t new_symbol = { .symbol = argv[i], .type = symbol_type };
                new_transition( new_s,
                                NULL,
                                new_symbol );

                if ( new_d->state_count > 1 )
                {
                    dea_input_symbol_t input_symbol;

                    new_d->states[new_d->state_count-2].transitions[0].next_state = &new_d->states[new_d->state_count-1];

                    input_symbol.symbol = ANY_SYMBOL;
                    input_symbol.type   = SPECIAL;

                    (void)new_transition( new_s, 
                                          &new_d->states[1],
                                          new_d->states[0].transitions[0].input_symbol );
                    (void)new_transition( new_s, 
                                          &new_d->states[0],
                                          input_symbol );
                }
            }
        }

        new_state( new_d, ACCEPTING );
        new_d->states[new_d->state_count-2].transitions[0].next_state = &new_d->states[new_d->state_count-1];

        init_dea( new_d );
    }
    return new_d;
}

