/*******************************************************************************
 * @file dea.c
 * @brief main source file for the dea lib; 
 *        implements a deterministic finite automata (dea)
 *
 * @author Christian Kranz
 *
 * @copyright Copyright 2018 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz.
 ******************************************************************************/
extern "C" {

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dea.h"

#define _TAB    0x09
#define _LF     0x0A
#define _CR     0x0D
#define _SP     0x20

/*******************************************************************************
 * Declarations
 ******************************************************************************/
/*******************************************************************************
 *
 ******************************************************************************/
static void print_transition( dea_transition_t* t );

/*******************************************************************************
 *
 ******************************************************************************/
static void print_state( size_t idx, dea_state_t* s, unsigned char is_current );

/*******************************************************************************
 *
 ******************************************************************************/
static dea_decision_t process_special( dea_t* d, dea_transition_t* t, char s );

/*******************************************************************************
 * Static implementation
 ******************************************************************************/
/*******************************************************************************
 *
 ******************************************************************************/
static void print_transition( dea_transition_t* t )
{
    if ( NULL != t )
    {
        if ( CHAR == t->input_symbol.type )
            printf("              |--> \"%c\" ==> %p\n", t->input_symbol.symbol, (void*)t->next_state );
        else
            printf("              |--> [%c] ==> %p\n", t->input_symbol.symbol, (void*)t->next_state );
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
static void print_state( size_t idx, dea_state_t* s, unsigned char is_current )
{
    if ( NULL != s )
    {
        printf("     |--> [%zd @ %p] is_accepting == %d", idx, (void*)s, s->is_accepting );
        if ( 0 != is_current ) printf(" {*}\n");
        else printf("\n");

        size_t t_idx;
        for( t_idx=0; t_idx < s->transition_count; t_idx++ )
        {
            if ( NULL != s->transitions )
            {
                print_transition( &s->transitions[t_idx] );
            }
        }
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
static dea_decision_t process_special( dea_t* d, dea_transition_t* t, char s )
{
    dea_decision_t res = NOT_ACCEPTING;

    switch( t->input_symbol.symbol )
    {
        case ANY_SYMBOL:        
            d->current_state = t->next_state;
            res = ACCEPTING;
            break;
        case ANY_WHITESPACE:
            switch ( s )
            {
                case _SP:
                case _TAB:
                case _LF:
                case _CR:
                    d->current_state = t->next_state;
                    res = ACCEPTING;
                    break;
                default: break;
            }
        case ANY_DIGIT:
            if ( ( s >= '0' ) && ( s <= '9' ) )
            {
                d->current_state = t->next_state;
                res = ACCEPTING;
            }
            break;
        default: break;
    }

    return res;
}

/*******************************************************************************
 * Extern implementation
 ******************************************************************************/
/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void free_dea( dea_t* d )
{
    if ( NULL != d )
    {
        size_t s_idx;
        for ( s_idx=0; s_idx < d->state_count ; s_idx++ )
        {
            size_t t_idx;
            for( t_idx=0; t_idx < d->states[s_idx].transition_count; t_idx++ )
            {
                d->states[s_idx].transitions[t_idx].next_state = NULL;
            }
            free( d->states[s_idx].transitions );
            d->states[s_idx].transitions = NULL;
        }
        d->current_state = NULL;

        if ( NULL != d->states )
        {
            free( d->states );
            d->states = NULL;
        }

        free( d );
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_t* new_empty_dea( void )
{
    dea_t* new_dea = (dea_t*)malloc( sizeof(dea_t) );
    if ( NULL != new_dea )
    {
        new_dea->state_count = 0;
        new_dea->states = NULL;
        new_dea->current_state = NULL;
    }
    return new_dea;
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_t* new_dea( size_t state_count )
{
    dea_t* new_dea = new_empty_dea();
    if ( NULL != new_dea )
    {
        size_t state_size = sizeof(dea_state_t) * (state_count + 1);
        new_dea->states = (dea_state_t*)malloc( state_size );
        new_dea->state_count = state_count;
        new_dea->current_state = &new_dea->states[0];
    }
    return new_dea;
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_state_t* new_state( dea_t* d, dea_decision_t is_accepting )
{
    dea_state_t* new_state = NULL;
    if ( NULL != d )
    {
        size_t new_size = sizeof(dea_state_t) * (d->state_count + 1);

#warning REALLOC moves memory so pointer values change; replace with cpp code or adjust pointer
        d->states = (dea_state_t*)realloc( d->states, 
                                           new_size );
        if ( NULL != d->states )
        {            
            new_state = &d->states[ d->state_count ];
            d->state_count++;

            new_state->is_accepting     = is_accepting;
            new_state->transitions      = NULL;
            new_state->transition_count = 0;
        }
    }
    return new_state;
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_transition_t* new_transition( dea_state_t*       s_src,
                                          dea_state_t*       s_trg, 
                                          dea_input_symbol_t input_symbol )
{
    dea_transition_t* new_trans = NULL;
    if ( NULL != s_src )
    {
        size_t new_size = sizeof(dea_transition_t) * (s_src->transition_count + 1);

        s_src->transitions = (dea_transition_t*)realloc( s_src->transitions, 
                                                         new_size );
        if ( NULL != s_src->transitions )
        {
            new_trans = &s_src->transitions[s_src->transition_count];
            s_src->transition_count++;

            new_trans->input_symbol = input_symbol;
            new_trans->next_state   = s_trg;
        }
    }
    return new_trans;
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void init_dea( dea_t* d )
{
    if ( NULL != d )
    {
        d->current_state = d->states;
    }
}


/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void print_dea( dea_t* d )
{
    if ( NULL != d )
    {
        printf("dea %p\n", (void*)d);
        printf(" |--> states => %zd @ %p\n", d->state_count, (void*)d->states );
        size_t s_idx;
        for ( s_idx=0; s_idx < d->state_count ; s_idx++ )
        {
            unsigned char is_current = 0;
            if ( &d->states[s_idx] == d->current_state )
                is_current = 1;

            print_state( s_idx, &d->states[s_idx], is_current );
        }
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void process_symbol( dea_t* d, char symbol, unsigned char verbose )
{
    if ( NULL != d )
    {
        if ( NULL != d->current_state )
        {
            size_t t_idx;
            for ( t_idx = 0; t_idx < d->current_state->transition_count; t_idx++ )
            {
                dea_decision_t found = NOT_ACCEPTING;

                dea_transition_t* t = &d->current_state->transitions[t_idx];
                if ( 0 != verbose ) 
                    printf("%c ?= %c ", t->input_symbol.symbol, symbol );

                switch ( t->input_symbol.type )
                {
                    case SPECIAL:
                        if ( 0 != verbose ) 
                            printf(" (%p -> %p)",(void*)d->current_state, (void*)t->next_state);

                        found = process_special( d, t, symbol );
                        if ( NOT_ACCEPTING == found )
                        {
                            if ( 0 != verbose ) 
                                printf("\t FAIL");
                        }
                        if ( 0 != verbose ) 
                            printf("\n");
                        break;
                    case CHAR:
                        if ( symbol == t->input_symbol.symbol )
                        {
                            if ( 0 != verbose ) 
                                printf(" (%p -> %p)\n",(void*)d->current_state, (void*)t->next_state);
                            d->current_state = t->next_state;
                            found = ACCEPTING;
                        }
                        break;
                    default: 
                        break;
                }

                if ( ACCEPTING == found )
                    break;

                if ( 0 != verbose ) 
                    printf("\n");

            }
        }
    }
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_decision_t verify_input( dea_t* d, const char* input )
{
    dea_decision_t result = NOT_ACCEPTING;

    if ( ( NULL != input ) && ( NULL != d ) )
    {
        if ( NULL != d->current_state )
        {
            size_t input_len = strnlen( input, MAX_STR_LEN );
            size_t input_idx;
            printf("input(%zd) = %s\n", input_len, input );
            for (input_idx = 0; input_idx < input_len; input_idx++ )
            {
                process_symbol( d, input[input_idx], 1 );
            }

            result = d->current_state->is_accepting;
        }
    }
    return result;
}

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_t* new_contains( const char* argv, size_t l )
{
#define ESCAPE_CHAR 0x5C
    dea_t* new_d = NULL;

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

                dea_input_symbol_t new_symbol;
                new_symbol.symbol = argv[i];
                new_symbol.type = symbol_type;
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



}
