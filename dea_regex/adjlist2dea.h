/*******************************************************************************
 * @file adjlist2dea.h
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
#ifndef __ADJLIST2DEA_H_
#define __ADJLIST2DEA_H_

#include "dea.h"

#define ESCAPE_CHAR         0x5C
#define SEPERATOR_CHAR      '#'
#define ACCEPTING_CODE      'a'
#define NOT_ACCEPTING_CODE  'r'

/*******************************************************************************
 * example input:
 *  rra## 0#a#1## 0#b#2## 1#a#2##
 *
 ******************************************************************************/
dea_t* dea_from_adj_list( const char* adj_list );

/*******************************************************************************
 * example input:
 *  rra## 0#a#1## 0#b#2## 1#a#2##
 *
 ******************************************************************************/
dea_decision_t is_adj_list_valid( const char* adj_list );

/*******************************************************************************
 * example input:
 *  rra## 0#a#1## 0#b#2## 1#a#2##
 *
 * prints to stdout
 ******************************************************************************/
void dea_to_adj_list( dea_t* d );

#endif /* __ADJLIST2DEA_H_ */
