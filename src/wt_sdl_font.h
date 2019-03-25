/*!*****************************************************************************
 * @file wt_sdl_font.h
 * @brief sdl font helper class
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
#ifndef _WT_SDL_FONT_H_
#define _WT_SDL_FONT_H_


#include "SDL.h"

#include "wt_sdl_utils.h"

class WtSdlFont
{
public:
    WtSdlFont() :
        m_name(""),
        m_font_w(0),
        m_font_h(0),
        m_font_data()
    {
    }
    WtSdlFont( std::string name, 
               size_t w,
               size_t h,
               std::string filename, 
               SDL_Renderer* renderer ) :
        m_name( name ),
        m_font_w( w ),
        m_font_h( h )
    {
        load_font_data( filename, renderer );
    }
    ~WtSdlFont()
    {
        for(size_t i = 0; i < m_font_data.size(); i++)
            SDL_DestroyTexture(m_font_data[i]);
    }

    /**************************
     *
     *************************/
    void load_font_data( std::string filename, SDL_Renderer* renderer )
    {
        if ( m_font_data.size() > 0 )
        {
            for(size_t i = 0; i < m_font_data.size(); i++)
                SDL_DestroyTexture( m_font_data[i] );
            m_font_data.clear();
        }

        SDL_Texture* font_complete = WtSdlUtils::loadAssetToTexture( filename, renderer );
        // fonts are organized by ascii code...
        for(char i = ' '; i < '~'; i++ )
        {
            m_font_data.push_back( get_letter_texture( font_complete, i, renderer ) );
        }
        SDL_DestroyTexture(font_complete);
    }

    /**************************
     *
     *************************/
    size_t width()
    {
        return m_font_w;
    }

    /**************************
     *
     *************************/
    size_t height()
    {
        return m_font_w;
    }

    /**************************
     *
     *************************/
    void write( WtCoord pos, char ch, SDL_Renderer* renderer )
    {
        SDL_Rect small;
        small.x = pos.x;
        small.y = pos.y;
        small.w = m_font_w;
        small.h = m_font_h;

        SDL_RenderCopy(renderer, m_font_data[(ch - 0x20)], NULL, &small );        
    }
private:
    /**************************
     *
     *************************/
    SDL_Texture* GetAreaTexture( SDL_Rect rect, SDL_Renderer* renderer, SDL_Texture* source )
    {
        SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
        SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, result);
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 0);
//TODO font color        SDL_SetTextureColorMod(result, 0, 255, 255 );
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, source, &rect, NULL);
        // the folowing line should reset the target to default(the screen)
        SDL_SetRenderTarget(renderer, NULL);

        return result;
    }

    /**************************
     *
     *************************/
    SDL_Texture *get_letter_texture( SDL_Texture* t_letters, char letter, SDL_Renderer* renderer )
    {
        SDL_Rect letter_rect;
        letter_rect.x = (letter - 0x20) * m_font_w;
        letter_rect.y = 0;
        letter_rect.w = m_font_w;
        letter_rect.h = m_font_h;
        return GetAreaTexture( letter_rect, renderer, t_letters );
    }

private:
    std::string                 m_name;
    size_t                      m_font_w;
    size_t                      m_font_h;
    std::vector<SDL_Texture*>   m_font_data;
};

#endif /* _WT_SDL_FONT_H_ */
