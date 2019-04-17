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

#include "wt_backend_policy_if.h"

#define SDL_WINDOW WINDOW( WtDrawingPolicySdl )


class WtDrawingPolicySdl : public WtDrawingPolicyIf
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
        m_grid_font(0),
        m_grid_font_inverse(0),
        m_text_font(0),
        m_texture_cache()
    {
        if (SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            exit(-1);
        }
        if (SDL_CreateWindowAndRenderer(SDL_WIDTH, SDL_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &m_window, &m_renderer)) {
            std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
            exit(-1);
        }

        SDL_RenderSetLogicalSize(m_renderer, SDL_WIDTH, SDL_HEIGHT);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        /*
        TTF_Init();
        m_font = TTF_OpenFont("assets/DejaVuSansMono.ttf", 30);
        */
        set_bg("bg.bmp");

        m_grid_font = new WtSdlFont( "grid", GRID_FONT_SIZE, GRID_FONT_SIZE, "grid_font.bmp", m_theme, m_renderer );
        m_grid_font_inverse = new WtSdlFont( "grid_inverse", GRID_FONT_SIZE, GRID_FONT_SIZE, "grid_font_inverse.bmp", m_theme, m_renderer );
        m_text_font = new WtSdlFont( "text", TEXT_FONT_SIZE, TEXT_FONT_SIZE*2, "text_font.bmp", m_theme, m_renderer );

    }

    ~WtDrawingPolicySdl()
    {
        delete m_grid_font;
        delete m_text_font;

        clear_texture_cache();

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
        m_grid_font->set_theme( name, m_renderer );
        m_text_font->set_theme( name, m_renderer );
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
        WtSdlFont* font = m_grid_font;
        row = WtBoard::row_count - row;
        ssize_t x = (col*static_cast<ssize_t>(font->width()))+col+GRID_OFFSET_X;
        ssize_t y = ((row*static_cast<ssize_t>(font->height()))+row)+GRID_OFFSET_Y;

        draw_image( WtCoord( x, y ),
                    font->size(),
                    image );
    }

    /**************************
     *
     *************************/
    void draw_at_grid( uint8_t row,
                       uint8_t column,
                       bool horizontal,
                       std::string text,
                       std::string font )
    {
        for ( size_t i = 0; i < text.size(); i++ )
        {
            if ( horizontal )
                put_cell_custom( (column+i)%WtBoard::col_count,
                                 row,
                                 text[i], 
                                 ( font == "grid_inverse" ? m_grid_font_inverse : m_grid_font ) );
            else
                put_cell_custom( column,
                                 (row-i)%WtBoard::row_count, 
                                 text[(text.length()-1)-i], 
                                 ( font == "grid_inverse" ? m_grid_font_inverse : m_grid_font ) );
        }
    }

    /**************************
     *
     *************************/
    void draw_board( const WtBoard& board )
    {
        for( uint8_t i=0 ; i < WtBoard::row_count; i++ )
            for( uint8_t j=0; j < WtBoard::col_count; j++ )
            {
                char cell = board.get_cell( i, j );
                if ( cell != '\0' )
                    put_cell( j, WtBoard::row_count-i, cell );
            }
    }

    /**************************
     *
     *************************/
    void draw_active_letter( const WtLetter& active )
    {
        put_cell( active.current_column(), 
                  WtBoard::row_count - active.current_row(), 
                  active.current_value() );
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
                    const std::string font="text_font")
    {
        puts_fb( pos.x, pos.y, text, ( font == "text_font" ? m_text_font : m_grid_font ) );
    }

    /**************************
      *
      *************************/   
    WtDim get_font_size()
    {
        return m_text_font->size();
    }



private:
    WtDrawingPolicySdl( const WtDrawingPolicySdl& ); 
    WtDrawingPolicySdl & operator = ( const WtDrawingPolicySdl& );

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
    void put_cell_custom( size_t col, size_t row, const char ch, WtSdlFont* font )
    {
        size_t x = (col*font->width())+col+GRID_OFFSET_X;
        size_t y = ((row*font->height())+row)+GRID_OFFSET_Y;

        font->write( WtCoord( static_cast<ssize_t>(x), static_cast<ssize_t>(y)), ch, m_renderer );
    }

    /**************************
     *
     *************************/
    void put_cell( uint8_t col, uint8_t row, const char ch )
    {
        put_cell_custom( col, row, ch, m_grid_font );
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

    WtSdlFont*    m_grid_font;
    WtSdlFont*    m_grid_font_inverse;
    WtSdlFont*    m_text_font;

    SDL_TextureCache m_texture_cache;
};



#endif /* _WT_SDL_H */
