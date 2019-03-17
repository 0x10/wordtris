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

#include "global.h"

#include "wt_sdl_utils.h"
#include "wt_sdl_font.h"
#include "wt_sdl_config.h"

#include "wt_types.h"
#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"
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
        m_renderer( 0 )
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

        m_grid_font = new WtSdlFont( "grid", GRID_FONT_SIZE, GRID_FONT_SIZE, "grid_font.bmp", m_renderer );
        m_text_font = new WtSdlFont( "text", TEXT_FONT_SIZE, TEXT_FONT_SIZE*2, "text_font.bmp", m_renderer );

    }

    ~WtDrawingPolicySdl()
    {
        delete m_grid_font;
        delete m_text_font;

        SDL_TextureCache::iterator it;
        for ( it = m_texture_cache.begin(); it != m_texture_cache.end(); it++ )
        {
            if ( NULL != it->second ) {
                SDL_DestroyTexture( it->second );
                it->second = NULL;
            }
        }
        m_texture_cache.clear();


        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    /**************************
     *
     *************************/
    void clr()
    {
        // clear canvas and init with background image
        SDL_RenderClear(m_renderer);
        draw_bg();
    }

    /**************************
     *
     *************************/
    void draw_player_stat( const WtPlayer& player )
    {
        SDL_Texture*  button_img = get_texture( "label_bg.bmp" );
        SDL_Rect Message_rect;
        Message_rect.x = 79;
        Message_rect.y = 32;
        Message_rect.w = 256;
        Message_rect.h = 65;
        SDL_RenderCopy(m_renderer, button_img, NULL, &Message_rect);

        std::string s_player = WtL10n::tr("Score: ");
        puts_fb( 90, 50, s_player.c_str(), m_text_font );
//        putUint_fb( 25+strlen("player ")*GRID_FONT_SIZE, 20, player.get_current_level() );
        putUint_fb( 90+s_player.length()*m_text_font->width(), 50, player.get_points(), m_text_font );
        //putUint_fb( 23, 0, player.get_solved_word_count() );
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
                    put_cell( j, ((WtBoard::row_count-i)), cell );
            }
    }

    /**************************
     *
     *************************/
    void draw_active_letter( const WtLetter& active )
    {
        put_cell( active.current_column(), WtBoard::row_count - active.current_row(), active.current_value() );
    }

    /**************************
     *
     *************************/
    void draw_hint( const std::string hint )
    {
        puts_fb( 79, 890,  hint.c_str(), m_text_font );
    }

     /**************************
      *
      *************************/   
    void draw_message( const std::string msg )
    {
//        puts_fb( 2, 23, msg.c_str(), m_text_font );
//        draw_button( WtCoord( (SDL_WIDTH-500)/2, SDL_HEIGHT/2-80/2 ),
  //                   msg );
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
    void draw_icon( const WtCoord&    pos,
                    const std::string fname )
    {
        SDL_Texture* button_img = get_texture( fname );
        SDL_Rect Message_rect;
        Message_rect.x = pos.x;
        Message_rect.y = pos.y;
        Message_rect.w = 500;//todo get_texture_size( buttom_img );
        Message_rect.h = 80;
        SDL_RenderCopy(m_renderer, button_img, NULL, &Message_rect);
    }

    /**************************
      *
      *************************/   
    void draw_button( const size_t& x, const size_t& y,
                      const size_t& w, const size_t& h,
                      const std::string img,
                      const std::string text )
    {
        SDL_Texture*  button_img = get_texture( img );
        SDL_Rect Message_rect;
        Message_rect.x = x;
        Message_rect.y = y;
        Message_rect.w = w;
        Message_rect.h = h;
        SDL_RenderCopy(m_renderer, button_img, NULL, &Message_rect);

        if ( ! text.empty() )
        {
            size_t text_center_w = ( text.length() / 2 ) * m_text_font->width();
            size_t button_center_x = ( w / 2 + x );
            size_t button_center_y = ( h / 2 + y );
            size_t text_center_h = m_text_font->height();

            puts_fb( button_center_x - text_center_w, button_center_y - text_center_h, text.c_str(), m_text_font );
        }
    }

    /**************************
      *
      *************************/   
    void draw_pause_button( const WtCoord& pos )
    {
        SDL_Texture*  pause_button_img = get_texture( "pause.bmp" );
        SDL_Rect Message_rect;
        Message_rect.x = pos.x;
        Message_rect.y = pos.y;
        Message_rect.w = 42;
        Message_rect.h = 42;
        SDL_RenderCopy(m_renderer, pause_button_img, NULL, &Message_rect);
    }

    /**************************
      *
      *************************/   
    void draw_text( const WtCoord&    pos,
                    const std::string text )
    {
        puts_fb( pos.x, pos.y, text.c_str(), m_text_font );
    }
private:
    WtDrawingPolicySdl( const WtDrawingPolicySdl& ); 
    WtDrawingPolicySdl & operator = ( const WtDrawingPolicySdl& );

    /**************************
     *
     *************************/
    void draw_bg()
    {
        SDL_Texture* bg_img = get_texture( m_bg_img_path );
        SDL_Rect Message_rect;
        Message_rect.x = 0;
        Message_rect.y = 0;
        Message_rect.w = SDL_WIDTH;
        Message_rect.h = SDL_HEIGHT;
        SDL_RenderCopy(m_renderer, bg_img, NULL, &Message_rect);
    }

    /**************************
     *
     *************************/
    void puts_fb( size_t x, size_t y, const char* str, WtSdlFont* font )
    {
        if ( ( NULL != str ) && ( NULL != font ) )
        {
            // loop through all characters in the text string
            size_t l = strlen(str);
            size_t x_i = 0;
            WtCoord pos( x, y );
            for (size_t i = 0; i < l; i++) {
                pos.x = x+x_i*font->width();
                if (str[i] == '\n')
                {
                    x_i = 0;
                    pos.x = x;
                    pos.y += font->height()+font->height()/2;
                }
                else
                {
                    putc( pos, str[i], font );
                    x_i++;
                }
            } // end "for i"
        }
    }

    /**************************
     *
     *************************/
    void put_cell( size_t col, size_t row, const char ch )
    {
        size_t x = (col*m_grid_font->width())+col+GRID_OFFSET_X;
        size_t y = ((row*m_grid_font->height())+row)+GRID_OFFSET_Y;

        putc( WtCoord(x, y), ch, m_grid_font );
    }
   
    /**************************
     *
     *************************/
    void putc( WtCoord pos, const char ch, WtSdlFont* font )
    {
        if ( NULL != font )
            font->write( pos, ch, m_renderer );
    }

    /**************************
     *
     *************************/
    void putUint_fb( size_t x, size_t y, uint32_t i, WtSdlFont* font )
    {
        if ( NULL != font )
        {
            size_t idx = (size_t) log10(i) + 1;
            if (i == 0)
                putc( WtCoord( x, y ), '0', font );

            while (i != 0) {
                putc( WtCoord( x+(idx*font->width()), y), (0x30 + (i % 10)), font );
                i /= 10;
                idx-- ;
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
                                   SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
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
                m_texture_cache[fname] = WtSdlUtils::loadAssetToTexture( fname, m_renderer );

                tex = m_texture_cache[fname];
            }
        }
        return tex;
    }

private:
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;
    //TTF_Font*     m_font;

    std::string   m_bg_img_path;

    WtSdlFont*    m_grid_font;
    WtSdlFont*    m_text_font;

    SDL_TextureCache m_texture_cache;
};



#endif /* _WT_SDL_H */
