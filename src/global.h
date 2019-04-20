/*!*****************************************************************************
 * @file global.h
 * @brief global header definitions for wordtris
 *
 * This file was developed as part of wordtris
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
#ifndef _WT_GLOBAL_H_
#define _WT_GLOBAL_H_

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

#include "wt_l10n.h"
#include "wt_basic_types.h"
#include "wt_backend_select.h"

#define ABS(N) ((N<0)?(-N):(N))

#define WT_BIND_EVENT_HANDLER( func )   std::bind ( &func, this )
#define WT_BIND_EVENT_HANDLER_1( func ) std::bind ( &func, this, std::placeholders::_1 )
#define WT_BIND_EVENT_HANDLER_2( func ) std::bind ( &func, this, std::placeholders::_1, std::placeholders::_2  )
#define WT_BIND_EVENT_HANDLER_3( func ) std::bind ( &func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 )


#endif /* _WT_GLOBAL_H_ */
