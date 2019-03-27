/*******************************************************************************
 * @file contains.c
 * @brief application of dea unit which implements a contains search logic
 *        using the dea lib
 *
 * @author Christian Kranz
 *
 * @copyright Copyright 2018 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dea.h"
#include "adjlist2dea.h"

/*******************************************************************************
 *
 ******************************************************************************/
dea_t* construct_contains( const char* argv )
{
    dea_t* new_d = NULL;
    if ( NULL != argv )
    {
        size_t l = strnlen( argv, MAX_STR_LEN );

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
    }
    return new_d;
}

/*******************************************************************************
 *
 ******************************************************************************/
void print_contains_adj_list( const char* word )
{
    int i;
    size_t l = strnlen( word, MAX_STR_LEN );
    int current_state = 0;
    int was_escape =0;
    int first_was_escape =0;

    printf("r");
    for( i=0;i<l-1;i++)
    {
        if ( word[i] != ESCAPE_CHAR )
        {
            printf("r");
        }
        else
        {
            if ( ( i > 0 ) && ( word[i-1] == ESCAPE_CHAR ) )
            {
                printf("r");
            }
        }
    }
    printf("a##");

    for( i=0;i<l;i++)
    {
        if ( ( word[i] == ESCAPE_CHAR ) && ( was_escape == 0))
        {
            was_escape = 1;
            if (current_state == 0)
                first_was_escape = 1;
        }
        else
        {
            printf("%d#", current_state);
            if ( was_escape == 1 )
            {
                was_escape = 0;
                printf("%c", ESCAPE_CHAR );
            }
            else 
            {
                if ( word[i] == SEPERATOR_CHAR ) printf("%c", ESCAPE_CHAR );
            }
            printf("%c#", word[i] );
            printf("%d##", current_state+1);

            if ( current_state > 0 )
            {
                printf("%d#", current_state);
                if ( first_was_escape == 1 )
                {
                    printf("%c", ESCAPE_CHAR );
                    printf("%c#", word[1]);
                }
                else 
                {
                    if ( word[0] == SEPERATOR_CHAR ) printf("%c", ESCAPE_CHAR );
                    printf("%c#", word[0]);
                }
                printf("%d##", 1);

                printf("%d#.#%d##" , current_state, 0);
            }

            current_state++;
        }
    }
       // printf("\n");
}

/*******************************************************************************
 *
 ******************************************************************************/
size_t* contains( const char* search_string, const char* input, size_t* pos_list_count )
{
    size_t* pos_list = NULL;
    if ( ( NULL != input ) && ( NULL != search_string ) && ( NULL != pos_list_count ) )
    {
        dea_t* d_contains = construct_contains( search_string );
        if ( NULL != d_contains )
        {
            if ( NULL != d_contains->current_state )
            {
                size_t input_len = strnlen( input, MAX_STR_LEN );
                size_t input_idx;
                for (input_idx = 0; input_idx < input_len; input_idx++ )
                {
                    process_symbol( d_contains, input[input_idx], 0 );
                    if ( ACCEPTING == d_contains->current_state->is_accepting )
                    {
                        size_t pos = (input_idx+1) - (d_contains->state_count -1) ;
                        (*pos_list_count)++;
                        pos_list = (size_t*)realloc( pos_list, sizeof(size_t) * (*pos_list_count) );
                        pos_list[(*pos_list_count) - 1] = pos;

                        init_dea( d_contains );
                    }
                }
            }
            free_dea( d_contains );
        }
    }
    return pos_list;
}

int main( int argc, const char* argv[] )
{
//    printf("%d\n", argc);
    if ( argc > 2 )
    {
        size_t list_count = 0;
        size_t input_len = strnlen( argv[2], MAX_STR_LEN );
        printf("input(%lu) = %s\n", input_len, argv[2] );
        size_t* pos_list = contains( argv[1], argv[2], &list_count );
        size_t pos_idx;
        for( pos_idx=0; pos_idx<list_count; pos_idx++ )
        {
            printf("found string at %lu\n", pos_list[pos_idx] );
        }

        if ( NULL != pos_list )
            free( pos_list );

    }
    else
    if ( argc > 1 )
    {
        print_contains_adj_list( argv[1] );
    }


    return 0;
}
