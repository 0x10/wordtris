/*******************************************************************************
 * @file dea.h
 * @brief main library header for dea lib
 *
 * @author Christian Kranz
 *
 * @copyright Copyright 2018 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz.
 ******************************************************************************/
#ifndef __DEA_H_
#define __DEA_H_

#include <stdlib.h>
#include "dea_common.h"

typedef struct dea_state_tag dea_state_t;
typedef struct {
    dea_input_symbol_t  input_symbol;
    dea_state_t*        next_state;
} dea_transition_t;

typedef struct dea_state_tag {
    dea_decision_t    is_accepting;
    dea_transition_t* transitions;
    size_t            transition_count;
} dea_state_t;

typedef struct {
    dea_state_t* states;
    size_t       state_count;
    dea_state_t* current_state;
} dea_t;

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void free_dea( dea_t* d );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_t* new_empty_dea( void );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_t* new_dea( size_t state_count );

    /*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_state_t* new_state( dea_t* d, dea_decision_t is_accepting );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_transition_t* new_transition( dea_state_t*       s_src,
                                          dea_state_t*       s_trg, 
                                          dea_input_symbol_t input_symbol );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void init_dea( dea_t* d );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void print_dea( dea_t* d );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API void process_symbol( dea_t* d, char symbol, unsigned char verbose );

/*******************************************************************************
 *
 ******************************************************************************/
DEA_API dea_decision_t verify_input( dea_t* d, const char* input );


#endif /* __DEA_H_ */
