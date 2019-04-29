/*!*****************************************************************************
 * @file wt_sdl_utils.h
 * @brief sdl helper class
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
#ifndef _WT_SDL_UTILS_H_
#define _WT_SDL_UTILS_H_


#include "SDL.h"

#include "wt_sdl_config.h"

class WtSdlUtils
{
public:
    /**************************
     *
     *************************/
    static SDL_Texture* loadAssetToTexture( SDL_Renderer* renderer,
                                            std::string   fname,
                                            std::string   theme="default" )
    {
        SDL_Texture* result_texture = NULL;
        std::string path = std::string(SDL_ASSETS).append(theme).append("/").append(fname);
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if (surface != 0)
        {
            result_texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        else
        {
            std::cout<<"could not load "<<path<<std::endl;
        }
        return result_texture;
    }
};

#endif /* _WT_SDL_UTILS_H_ */
