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

#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>

#define SDL_ASSETS "assets/"

#endif /* _WT_SDL_INCLUDE_H_ */
