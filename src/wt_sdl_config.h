/*!*****************************************************************************
 * @file wt_sdl_config.h
 * @brief config for sdl driver, defines virtual screensize
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
#ifndef _WT_SDL_CONFIG_H_
#define _WT_SDL_CONFIG_H_

#define SDL_WIDTH  (540)
#define SDL_HEIGHT (960)

#ifndef SDL_ASSETS
#define SDL_ASSETS "" //used for android assets vs pc assets
#endif /* SDL_ASSETS */

#endif /* _WT_SDL_CONFIG_H */
