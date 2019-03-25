/*!*****************************************************************************
 * @file wt_backend_select.h
 * @brief header file for backend config
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
#ifndef _WT_BACKEND_SELECT_H_
#define _WT_BACKEND_SELECT_H_

#ifdef RENDER_NCURSES
#include "wt_ncurses.h"
#define ACTIVE_WINDOW        NCURSES_WINDOW
#define ACTIVE_INPUT         NCURSES_INPUT
#define ACTIVE_WINDOW_WIDTH  NCURSES_WIDTH
#define ACTIVE_WINDOW_HEIGHT NCURSES_HEIGHT
#else
#include "wt_sdl.h"
#define ACTIVE_WINDOW        SDL_WINDOW
#define ACTIVE_INPUT         SDL_INPUT
#define ACTIVE_WINDOW_WIDTH  SDL_WIDTH
#define ACTIVE_WINDOW_HEIGHT SDL_HEIGHT
#endif

#endif /* _WT_BACKEND_SELECT_H_ */
