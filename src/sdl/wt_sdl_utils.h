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
    static WtDim get_image_size( std::string fname,
                                 std::string theme="default" )
    {
        WtDim image_size(0,0);
        std::string path = std::string(SDL_ASSETS);
        if ( fname[0] != '@' )
            path.append(theme).append("/").append(fname);
        else
            path.append(fname.substr(1));
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());

        if (surface != 0)
        {
            image_size.w = surface->w;
            image_size.h = surface->h;
            SDL_FreeSurface(surface);
        }
        else
        {
            std::cout<<"could not load "<<path<<std::endl;
        }

        return image_size;
    }


    /**************************
     *
     *************************/
    static SDL_Color get_color_from_string( std::string color )
    {
        SDL_Color result={0x4a, 0x90, 0xe2,255};
        unsigned int r,g,b;
        if ( sscanf(color.c_str(), "#%02x%02x%02x", &r, &g, &b) != 3 )
        {
            std::cout << "color invalid...\n";
        }
        result.r = r;
        result.g = g;
        result.b = b;
        return result;
    }

    /**************************
     *
     *************************/
    static SDL_Texture* load_asset_to_texture( SDL_Renderer* renderer,
                                               std::string   fname,
                                               std::string   theme="default" )
    {
        SDL_Texture* result_texture = NULL;
        std::string path = std::string(SDL_ASSETS);
        if ( fname[0] != '@' )
            path.append(theme).append("/").append(fname);
        else
            path.append(fname.substr(1));
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
