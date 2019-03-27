/*******************************************************************************
 * @file stack_dea.h
 * @brief main library header for stack dea lib
 *
 * @author Christian Kranz
 *
 * @copyright Copyright 2018 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz.
 ******************************************************************************/
#ifndef __STACK_DEA_H_
#define __STACK_DEA_H_

#include "dea_common.h"

#define MAX_STACK_DEPTH     (1024)

#define STACK_SYMBOL_EMPTY  (0x00)
#define STACK_START_SYMBOL  (0x01)

typedef char sdea_stack_symbols_t;
typedef struct {
    sdea_stack_symbols_t* bottom;
    size_t                count;
} sdea_stack_t;

typedef struct sdea_state_tag sdea_state_t;
typedef struct {
    dea_input_symbol_t   input_symbol;
    sdea_stack_symbols_t expected_top;

    sdea_state_t*        next_state;
    sdea_stack_symbols_t new_top;
} sdea_transition_t;

typedef struct sdea_state_tag {
    dea_decision_t     is_accepting;
    sdea_transition_t* transitions;
    size_t             transition_count;
} sdea_state_t;

typedef struct {
    sdea_state_t* states;
    size_t        state_count;
    sdea_state_t* current_state;
    sdea_stack_t  stack;
} sdea_t;

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void free_sdea( sdea_t* d );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API sdea_t* new_empty_sdea( void );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API sdea_t* new_sdea( size_t state_count );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API sdea_state_t* new_state( sdea_t* d, sdea_decision_t is_accepting );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API sdea_transition_t* new_transition( sdea_state_t*        s_src,
                                           sdea_state_t*        s_trg, 
                                           dea_input_symbol_t   input_symbol,
                                           sdea_stack_symbols_t expected_top_symbol,
                                           sdea_stack_symbols_t new_top_symbol );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void init_sdea( sdea_t* d );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void print_sdea( sdea_t* d );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void process_symbol( sdea_t* d, char symbol, unsigned char verbose );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API sdea_decision_t verify_input( sdea_t* d, const char* input );


#endif /* __STACK_DEA_H_ */
