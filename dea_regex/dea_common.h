/*******************************************************************************
 * @file dea_common.h
 * @brief common types for all dea varieties
 *
 * @author Christian Kranz
 *
 * @copyright Copyright 2018 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz.
 ******************************************************************************/
#ifndef __DEA_COMMON_H_
#define __DEA_COMMON_H_

#define DEA_API         extern
#define MAX_STR_LEN     (65535)

typedef enum {
    CHAR,
    SPECIAL,
    INVALID
} dea_char_type_t;

typedef enum {
    NOT_ACCEPTING,
    ACCEPTING
} dea_decision_t;

/* these are reg ex specific chars not dea specific
 * dea specials group n transitions to a single
 */
typedef enum {
    ANY_SYMBOL        = '.',
    ANY_WHITESPACE    = 'w',
    ANY_DIGIT         = 'd'
} dea_special_types_t;


typedef struct {
    char            symbol;
    dea_char_type_t type;
} dea_input_symbol_t;

#endif /* __DEA_COMMON_H_ */
