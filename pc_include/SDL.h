/*!*****************************************************************************
 * @file SDL.h
 * @brief SDL header wrapper sdl builds pc vs android
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
#ifndef _WT_SDL_INCLUDE_H_
#define _WT_SDL_INCLUDE_H_

/* this file is needed for PC build
 * to include PC SDL instead of builded
 */

#ifdef USE_SYSTEM_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL2-2.0.9/include/SDL.h"
#include "SDL2_ttf-2.0.15/SDL_ttf.h"
#endif /* USE_SYSTEM_SDL */

#define SDL_ASSETS "assets/"

#endif /* _WT_SDL_INCLUDE_H_ */
