/*******************************************************************************
 * @file adjlist2dea.c
 * @brief conversion functions to export or import a dea to or from
 *        an adjacent list
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

#include "adjlist2dea.h"

#define verifier_state_count    11
static dea_state_t verifier_states[verifier_state_count];

#define verifier_state0_transition_count    3
static dea_transition_t state0_transitions[verifier_state0_transition_count] = {
    [0] = { .input_symbol = { .symbol = NOT_ACCEPTING_CODE, .type = CHAR    }, .next_state = &verifier_states[2] },
    [1] = { .input_symbol = { .symbol = ACCEPTING_CODE,     .type = CHAR    }, .next_state = &verifier_states[2] },
    [2] = { .input_symbol = { .symbol = ANY_SYMBOL,         .type = SPECIAL }, .next_state = &verifier_states[1] }
};

#define verifier_state2_transition_count    4
static dea_transition_t state2_transitions[verifier_state2_transition_count] = {
    [0] = { .input_symbol = { .symbol = NOT_ACCEPTING_CODE, .type = CHAR    }, .next_state = &verifier_states[2] },
    [1] = { .input_symbol = { .symbol = ACCEPTING_CODE,     .type = CHAR    }, .next_state = &verifier_states[2] },
    [2] = { .input_symbol = { .symbol = SEPERATOR_CHAR,     .type = CHAR    }, .next_state = &verifier_states[3] },
    [3] = { .input_symbol = { .symbol = ANY_SYMBOL,         .type = SPECIAL }, .next_state = &verifier_states[1] }
};

#define verifier_state3_transition_count    2
static dea_transition_t state3_transitions[verifier_state3_transition_count] = {
    [0] = { .input_symbol = { .symbol = SEPERATOR_CHAR, .type = CHAR    }, .next_state = &verifier_states[4] },
    [1] = { .input_symbol = { .symbol = ANY_SYMBOL,     .type = SPECIAL }, .next_state = &verifier_states[1] }
};

#define verifier_state4_transition_count   2
static dea_transition_t state4_transitions[verifier_state4_transition_count] = {
    [0] = { .input_symbol = { .symbol = ANY_DIGIT,  .type = SPECIAL }, .next_state = &verifier_states[5] },
    [1] = { .input_symbol = { .symbol = ANY_SYMBOL, .type = SPECIAL }, .next_state = &verifier_states[1] }
};

#define verifier_state5_transition_count   3
static dea_transition_t state5_transitions[verifier_state5_transition_count] = {
    [0] = { .input_symbol = { .symbol = ANY_DIGIT,      .type = SPECIAL }, .next_state = &verifier_states[5] },
    [1] = { .input_symbol = { .symbol = SEPERATOR_CHAR, .type = CHAR    }, .next_state = &verifier_states[6] },
    [2] = { .input_symbol = { .symbol = ANY_SYMBOL,     .type = SPECIAL }, .next_state = &verifier_states[1] }
};

#define verifier_state6_transition_count    2
static dea_transition_t state6_transitions[verifier_state6_transition_count] = {
    [0] = { .input_symbol = { .symbol = ESCAPE_CHAR, .type = CHAR    }, .next_state = &verifier_states[7] },
    [1] = { .input_symbol = { .symbol = ANY_SYMBOL,  .type = SPECIAL }, .next_state = &verifier_states[8] }
};

#define verifier_state7_transition_count    6
static dea_transition_t state7_transitions[verifier_state7_transition_count] = {
    [0] = { .input_symbol = { .symbol = SEPERATOR_CHAR, .type = CHAR    }, .next_state = &verifier_states[8] },
    [1] = { .input_symbol = { .symbol = ESCAPE_CHAR,    .type = CHAR    }, .next_state = &verifier_states[8] },
    [2] = { .input_symbol = { .symbol = ANY_SYMBOL,     .type = CHAR    }, .next_state = &verifier_states[8] },
    [3] = { .input_symbol = { .symbol = ANY_WHITESPACE, .type = CHAR    }, .next_state = &verifier_states[8] },
    [4] = { .input_symbol = { .symbol = ANY_DIGIT,      .type = CHAR    }, .next_state = &verifier_states[8] },
    [5] = { .input_symbol = { .symbol = ANY_SYMBOL,     .type = SPECIAL }, .next_state = &verifier_states[1] }
};

#define verifier_state8_transition_count    2
static dea_transition_t state8_transitions[verifier_state8_transition_count] = {
    [0] = { .input_symbol = { .symbol = SEPERATOR_CHAR, .type = CHAR    }, .next_state = &verifier_states[9] },
    [1] = { .input_symbol = { .symbol = ANY_SYMBOL,     .type = SPECIAL }, .next_state = &verifier_states[1] }
};

#define verifier_state9_transition_count   2 
static dea_transition_t state9_transitions[verifier_state9_transition_count] = {
    [0] = { .input_symbol = { .symbol = ANY_DIGIT,  .type = SPECIAL }, .next_state = &verifier_states[10] },
    [1] = { .input_symbol = { .symbol = ANY_SYMBOL, .type = SPECIAL }, .next_state = &verifier_states[1]  }
};

#define verifier_state10_transition_count   3
static dea_transition_t state10_transitions[verifier_state10_transition_count] = {
    [0] = { .input_symbol = { .symbol = ANY_DIGIT,      .type = SPECIAL }, .next_state = &verifier_states[10] },
    [1] = { .input_symbol = { .symbol = SEPERATOR_CHAR, .type = CHAR    }, .next_state = &verifier_states[3]  },
    [2] = { .input_symbol = { .symbol = ANY_SYMBOL,     .type = SPECIAL }, .next_state = &verifier_states[1]  }
};

static dea_state_t verifier_states[verifier_state_count] = {
    [0] = {
        .is_accepting       = 0,
        .transitions        = state0_transitions,
        .transition_count   = verifier_state0_transition_count
    },
    [1] = {
        .is_accepting       = 0,
        .transitions        = NULL,
        .transition_count   = 0
    },
    [2] = {
        .is_accepting       = 0,
        .transitions        = state2_transitions,
        .transition_count   = verifier_state2_transition_count
    },
    [3] = {
        .is_accepting       = 0,
        .transitions        = state3_transitions,
        .transition_count   = verifier_state3_transition_count
    },
    [4] = {
        .is_accepting       = 1,
        .transitions        = state4_transitions,
        .transition_count   = verifier_state4_transition_count
    },
    [5] = {
        .is_accepting       = 0,
        .transitions        = state5_transitions,
        .transition_count   = verifier_state5_transition_count
    },
    [6] = {
        .is_accepting       = 0,
        .transitions        = state6_transitions,
        .transition_count   = verifier_state6_transition_count
    },
    [7] = {
        .is_accepting       = 0,
        .transitions        = state7_transitions,
        .transition_count   = verifier_state7_transition_count
    },
    [8] = {
        .is_accepting       = 0,
        .transitions        = state8_transitions,
        .transition_count   = verifier_state8_transition_count
    },
    [9] = {
        .is_accepting       = 0,
        .transitions        = state9_transitions,
        .transition_count   = verifier_state9_transition_count
    },
    [10] = {
        .is_accepting       = 0,
        .transitions        = state10_transitions,
        .transition_count   = verifier_state10_transition_count
    }
};


static dea_t dea_verifier = {
    .states         = verifier_states,
    .state_count    = verifier_state_count,
    .current_state  = &verifier_states[0]
};

/*******************************************************************************
 *
 ******************************************************************************/
static size_t get_next_seperator_pos( const char* adj_list, size_t offset )
{
    size_t al_idx;
    size_t al_len = strnlen( adj_list, MAX_STR_LEN );
    size_t result = 0;

    for( al_idx = offset; al_idx < (al_len-1) ; al_idx++ )
    {
        if ( ( SEPERATOR_CHAR == adj_list[al_idx] ) && ( SEPERATOR_CHAR == adj_list[al_idx+1] ) )
        {
            if ( al_idx > 0 )
            {
                if ( ESCAPE_CHAR != adj_list[al_idx-1] )
                {
                    result = al_idx;
                    break;
                }
            }
            else
            {
                result = al_idx;
                break;
            }
        }
    }

    return result;
}

/*******************************************************************************
 * example input:
 *  rra## 0#a#1## 0#b#2## 1#a#2##
 *
 ******************************************************************************/
dea_t* dea_from_adj_list( const char* adj_list )
{
    dea_t* new_dea = NULL;

    if ( NULL != adj_list ) 
    {
        const size_t GROUP_SEPERATOR_SIZE = 2;
        size_t al_idx;
        size_t al_len = strnlen( adj_list, MAX_STR_LEN );
        printf("al_len = %lu\n", al_len);
        size_t first_sep_pos = get_next_seperator_pos( adj_list, 0 );

        new_dea = new_empty_dea();

//        printf("%d\n", first_sep_pos);
        for( al_idx = 0; al_idx < first_sep_pos ; al_idx++ )
        {
            dea_decision_t acc = NOT_ACCEPTING;
            if ( ACCEPTING_CODE == adj_list[al_idx] )
            {
                acc = ACCEPTING;
            }

            (void)new_state( new_dea, acc );
//            printf("%c, %d, %lu\n", adj_list[al_idx], acc, new_dea->state_count );
        }
        init_dea( new_dea );


        {
            size_t old_pos = first_sep_pos+GROUP_SEPERATOR_SIZE;
            size_t next_sep_pos = get_next_seperator_pos( adj_list, old_pos );
            while ( 0 != next_sep_pos )
            {
                const size_t TRANS_SEPERATOR_SIZE = 1;
                const char* source_state  = &adj_list[old_pos];
                char* next_trans_sep;
                char symbol;
                size_t src_idx;
                size_t trg_idx;
                dea_char_type_t symbol_type = CHAR;

                src_idx         = strtol( source_state, &next_trans_sep, 10 );
                next_trans_sep  = &next_trans_sep[TRANS_SEPERATOR_SIZE];
                symbol          = *next_trans_sep;
                if ( symbol == ESCAPE_CHAR ) {
                    next_trans_sep  = &next_trans_sep[sizeof(symbol)];
                    symbol          = *next_trans_sep;
                    if ( ( ANY_WHITESPACE == symbol ) || ( ANY_DIGIT == symbol ) )
                    {
                        symbol_type     = SPECIAL;
                    }
                }
                else
                {
                    if ( ANY_SYMBOL == symbol )
                    {
                        symbol_type     = SPECIAL;
                    }
                }
                next_trans_sep  = &next_trans_sep[sizeof(symbol)+TRANS_SEPERATOR_SIZE];
                trg_idx         = strtol( next_trans_sep, &next_trans_sep, 10 );

//                printf("%lu#%c#%lu\n", src_idx, symbol, trg_idx);

                if ( ( ( src_idx >= 0 ) && ( trg_idx >= 0 ) ) &&
                     ( ( src_idx < new_dea->state_count ) && ( trg_idx < new_dea->state_count ) ) )
                {
                    dea_input_symbol_t input_symbol = { .symbol = symbol, .type = symbol_type };
                    new_transition( &new_dea->states[ src_idx ],
                                    &new_dea->states[ trg_idx ],
                                    input_symbol );
                }

                old_pos = next_sep_pos+GROUP_SEPERATOR_SIZE;
                next_sep_pos = get_next_seperator_pos( adj_list, old_pos );
            }
        }

    }

    return new_dea;
}

/*******************************************************************************
 * example input:
 *  rra## 0#a#1## 0#b#2## 1#a#2##
 *
 ******************************************************************************/
dea_decision_t is_adj_list_valid( const char* adj_list )
{
    /* check that the given dea is valid adj list syntax */
    return verify_input( &dea_verifier, adj_list );
}

/*******************************************************************************
 *
 ******************************************************************************/
static size_t get_state_id( dea_t* d, dea_state_t* s )
{
    size_t s_idx = 0;

    if ( ( NULL != d ) && ( NULL != s ) )
    {
        for ( s_idx = 0; s_idx < d->state_count; s_idx++ )
        {
            if ( s == &d->states[s_idx] )
            {
                break;
            }   
        }
    }

    return s_idx;
}

/*******************************************************************************
 *
 ******************************************************************************/
void dea_to_adj_list( dea_t* d )
{
    if ( NULL != d )
    {
        size_t s_idx;
        for ( s_idx=0; s_idx < d->state_count ; s_idx++ )
        {
            if ( d->states[s_idx].is_accepting == NOT_ACCEPTING )
            {
                printf("%c", NOT_ACCEPTING_CODE);
            }
            else
            {
                printf("%c", ACCEPTING_CODE);
            }
        }
        printf("%c%c", SEPERATOR_CHAR, 
                       SEPERATOR_CHAR );

        for ( s_idx=0; s_idx < d->state_count ; s_idx++ )
        {
            size_t t_idx;
            for ( t_idx=0; t_idx < d->states[s_idx].transition_count; t_idx++ )
            {
                dea_transition_t* t = &d->states[s_idx].transitions[t_idx];

                printf( "%lu%c", s_idx,
                                 SEPERATOR_CHAR );

                if (( t->input_symbol.symbol == SEPERATOR_CHAR ) || ( t->input_symbol.symbol == ESCAPE_CHAR ))
                {
                    printf( "%c", ESCAPE_CHAR );
                }

                printf( "%c%c", t->input_symbol.symbol,
                                SEPERATOR_CHAR );

                printf("%lu%c%c", get_state_id( d, t->next_state ),
                                  SEPERATOR_CHAR,
                                  SEPERATOR_CHAR );
            }
        }

        printf("\n");
    }
}
