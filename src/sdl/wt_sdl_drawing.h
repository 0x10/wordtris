/*!*****************************************************************************
 * @file wt_sdl_drawing.h
 * @brief main sdl drawing class
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
#ifndef _WT_SDL_DRAWING_H_
#define _WT_SDL_DRAWING_H_

#include "SDL.h"

#include <cassert>
#include <iostream>
#include <cmath>

#include "wt_sdl_utils.h"
#include "wt_sdl_font.h"
#include "wt_sdl_config.h"

#define SDL_WINDOW WINDOW( WtDrawingPolicySdl )

SDL_Window*   sdl_cleanup_window=NULL;
SDL_Renderer* sdl_cleanup_renderer=NULL;

void cleanup_sdl();
void cleanup_sdl()
{
    std::cout << "cleanup sdl..\n";
    if ( sdl_cleanup_renderer != NULL )
        SDL_DestroyRenderer(sdl_cleanup_renderer);
    if ( sdl_cleanup_window != NULL )
        SDL_DestroyWindow(sdl_cleanup_window);
    SDL_Quit();
}


class WtDrawingPolicySdl
{
private:
    static const uint8_t TEXT_FONT_SIZE = 12;
    static const uint8_t GRID_FONT_SIZE = 37;
    static const uint8_t GRID_OFFSET_X = 78;
    static const uint8_t GRID_OFFSET_Y = 127-GRID_FONT_SIZE;

    typedef std::map<std::string, SDL_Texture*> SDL_TextureCache;
// policy use only
protected:
    WtDrawingPolicySdl() :
        m_window( 0 ),
        m_renderer( 0 ),
        m_bg_img_path( ""),
        m_theme("default"),
        m_grid_font( "grid", GRID_FONT_SIZE, GRID_FONT_SIZE, "grid_font.bmp" ),
        m_grid_font_inverse( "grid_inverse", GRID_FONT_SIZE, GRID_FONT_SIZE, "grid_font_inverse.bmp" ),
        m_text_font( "text", TEXT_FONT_SIZE, TEXT_FONT_SIZE*2, "text_font.bmp" ),
        m_texture_cache()
    {
        if (SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            exit(-1);
        }
        atexit(cleanup_sdl);


        SDL_DisplayMode dm;

        if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
        {
             SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
             exit(-1);
        }

        std::cout << "const w/h = (" << SDL_WIDTH << "," << SDL_HEIGHT << ")" << std::endl;
        std::cout << "dm w/h = (" << dm.w << "," << dm.h << ")" << std::endl;

        if (SDL_CreateWindowAndRenderer(SDL_WIDTH, SDL_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &m_window, &m_renderer)) {
            std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
            exit(-1);
        }

        sdl_cleanup_window = m_window;
        sdl_cleanup_renderer = m_renderer;

        SDL_RenderSetLogicalSize(m_renderer, SDL_WIDTH, SDL_HEIGHT);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        /*
        TTF_Init();
        m_font = TTF_OpenFont("assets/DejaVuSansMono.ttf", 30);
        */
        set_bg("bg.bmp");

        m_grid_font.load_font_data( m_theme, m_renderer );
        m_grid_font_inverse.load_font_data( m_theme, m_renderer );
        m_text_font.load_font_data( m_theme, m_renderer );
    }

    ~WtDrawingPolicySdl()
    {
        std::cout << "destroy sdl..\n";

        clear_texture_cache();

        sdl_cleanup_window = NULL;
        sdl_cleanup_renderer = NULL;

        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

public:
    /**************************
     *
     *************************/
    void clr()
    {
        // clear canvas and init with background image
        SDL_RenderClear(m_renderer);

        // draw bg
        draw_image( WtCoord( 0, 0 ),
                    WtDim( SDL_WIDTH, SDL_HEIGHT ),
                    m_bg_img_path );
    }

    /**************************
     *
     *************************/
    virtual void set_theme( std::string name )
    {
        // todo base class themeable
        m_grid_font.set_theme( name, m_renderer );
        m_grid_font_inverse.set_theme( name, m_renderer );
        m_text_font.set_theme( name, m_renderer );
        m_theme = name;
        // TODO check if available and if not keep old
        
        clear_texture_cache();
    }

    /**************************
     *
     *************************/
    void draw_custom_cell_bg( uint8_t     row,
                              uint8_t     col,
                              std::string image )
    {
        draw_image( grid_pos_to_screen_pos( row, col, &m_grid_font ),
                    m_grid_font.size(),
                    image );
    }

    /**************************
     *
     *************************/
    void draw_at_grid( uint8_t row,
                       uint8_t col,
                       char    value,
                       const std::string font="grid" )
    {
        WtSdlFont* selected_font = &m_grid_font;
        if ( font == "grid_inverse" )
            selected_font = &m_grid_font_inverse;


        if ( value >= selected_font->start_symbol() )
        {
            selected_font->write( grid_pos_to_screen_pos( row, col, selected_font ), 
                                  value, 
                                  m_renderer );
        }
    }


    /**************************
     *
     *************************/
    void update()
    {
        SDL_RenderPresent(m_renderer);
    }

    /**************************
     *
     *************************/   
    void set_bg( const std::string bg_img )
    {
        m_bg_img_path = bg_img;
    }

    /**************************
      *
      *************************/   
    void draw_image( const WtCoord     pos,
                     const WtDim       size,
                     const std::string fname )
    {
        SDL_Texture*  button_img = get_texture( fname );
        SDL_Rect rect;
        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = size.w;
        rect.h = size.h;
        SDL_RenderCopy(m_renderer, button_img, NULL, &rect);
    }
    /**************************
      *
      *************************/   
    void draw_text( const WtCoord     pos,
                    const std::string text,
                    const std::string font="text")
    {
        puts_fb( pos.x, pos.y, text, ( font == "text" ? &m_text_font : &m_grid_font ) );
    }

    /**************************
      *
      *************************/   
    WtDim get_font_size()
    {
        return m_text_font.size();
    }



private:
    WtDrawingPolicySdl( const WtDrawingPolicySdl& ); 
    WtDrawingPolicySdl & operator = ( const WtDrawingPolicySdl& );

    /**************************
     *
     *************************/   
    WtCoord grid_pos_to_screen_pos( uint8_t row, uint8_t col,
                                    WtSdlFont* font )
    {
        WtCoord screen_pos(0,0);
        if ( NULL != font )
        {
            screen_pos.x = (col*static_cast<ssize_t>(font->width()))+col+GRID_OFFSET_X;
            screen_pos.y = ((row*static_cast<ssize_t>(font->height()))+row)+GRID_OFFSET_Y;
        }
        return screen_pos;
    }

    /**************************
     *
     *************************/
    void puts_fb( ssize_t x, ssize_t y, const std::string str, WtSdlFont* font )
    {
        if ( NULL != font )
        {
            // loop through all characters in the text string
            size_t l = str.length();
            size_t x_i = 0;
            WtCoord pos( x, y );
            for (size_t i = 0; i < l; i++) {
                pos.x = x+static_cast<ssize_t>(x_i*font->width());
                if (str[i] == '\n')
                {
                    x_i = 0;
                    pos.x = x;
                    pos.y += static_cast<ssize_t>(font->height() + (font->height() / 2));
                }
                else
                {
                    font->write( pos, str[i], m_renderer );
                    x_i++;
                }
            }
        }
    }

    /**************************
     *
     *************************/
    SDL_Texture* get_texture( std::string fname )
    {
        SDL_Texture* tex = NULL;
        SDL_TextureCache::const_iterator it = m_texture_cache.find(fname);
        if ( it != m_texture_cache.end() )
        {
            // load from cache
            tex = (*it).second;
        }
        else
        {
            // load from file
            // insert into cache
            if ( fname.empty() )
            {
                tex = SDL_CreateTexture( m_renderer,
                                         SDL_PIXELFORMAT_RGBA8888, 
                                         SDL_TEXTUREACCESS_TARGET, 
                                         1, 1 );
                SDL_SetRenderTarget( m_renderer, tex );
                SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_NONE );
                SDL_SetRenderDrawColor( m_renderer, 255, 0, 255, 0 );
                SDL_RenderFillRect( m_renderer, NULL );
                SDL_SetRenderTarget( m_renderer, NULL );

                m_texture_cache[fname] = tex;
            }
            else
            {
                std::cout << "load from file.. " << fname << std::endl;
                m_texture_cache[fname] = WtSdlUtils::loadAssetToTexture( m_renderer, fname, m_theme );

                tex = m_texture_cache[fname];
            }
        }
        return tex;
    }

    /**************************
     *
     *************************/
    void clear_texture_cache()
    {
        SDL_TextureCache::iterator it;
        for ( it = m_texture_cache.begin(); it != m_texture_cache.end(); it++ )
        {
            if ( NULL != it->second ) {
                SDL_DestroyTexture( it->second );
                it->second = NULL;
            }
        }
        m_texture_cache.clear();
    }


private:
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;

    std::string   m_bg_img_path;
    std::string   m_theme;

    WtSdlFont     m_grid_font;
    WtSdlFont     m_grid_font_inverse;
    WtSdlFont     m_text_font;

    SDL_TextureCache m_texture_cache;
};



#endif /* _WT_SDL_H */
