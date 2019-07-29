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
#include "SDL_ttf.h"

#include <cassert>
#include <iostream>
#include <cmath>

#include "wt_sdl_utils.h"
#include "wt_sdl_font.h"
#include "wt_sdl_config.h"

#define SDL_WINDOW WINDOW( WtDrawingPolicySdl )
class WtDrawingPolicySdl
{
private:
    static const uint8_t TEXT_FONT_SIZE = 24;
    static const uint8_t SMALL_TEXT_FONT_SIZE = 18;
    static const uint8_t TINY_TEXT_FONT_SIZE = 9;
    static const uint8_t BIG_TEXT_FONT_SIZE = 32;
    static const uint8_t GRID_FONT_SIZE = 56;
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
        m_grid_font( "grid", GRID_FONT_SIZE, GRID_FONT_SIZE, SDL_ASSETS"Muli-Bold.ttf", 42, true ),
        m_text_font( "text", TEXT_FONT_SIZE, TEXT_FONT_SIZE, SDL_ASSETS"Muli.ttf", TEXT_FONT_SIZE, true ),
        m_text_font_tiny( "text_tiny", TINY_TEXT_FONT_SIZE, TINY_TEXT_FONT_SIZE, SDL_ASSETS"Muli.ttf", TINY_TEXT_FONT_SIZE, true ),
        m_text_font_big( "text_big", BIG_TEXT_FONT_SIZE, BIG_TEXT_FONT_SIZE, SDL_ASSETS"Muli-Bold.ttf", BIG_TEXT_FONT_SIZE, true ),
        m_text_font_small( "text_small", SMALL_TEXT_FONT_SIZE, SMALL_TEXT_FONT_SIZE, SDL_ASSETS"Muli.ttf", SMALL_TEXT_FONT_SIZE, true ),
        m_texture_cache()
    {

    }

    ~WtDrawingPolicySdl()
    {

    }

public:
    /**************************
     *
     *************************/
    bool open()
    {
        bool success = false;
        if (SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_DisplayMode dm;

            if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
            {
                SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
                SDL_Quit();
            }
            else
                {
                std::cout << "const w/h = (" << SDL_WIDTH << "," << SDL_HEIGHT << ")" << std::endl;
                std::cout << "dm w/h = (" << dm.w << "," << dm.h << ")" << std::endl;

                m_window = SDL_CreateWindow( "main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SDL_WIDTH_PC, SDL_HEIGHT_PC, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS  );
                
                if ( NULL == m_window ) {
                    SDL_Quit();
                    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
                }
                else
                {
                    m_renderer = SDL_CreateRenderer( m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );
                    if ( NULL == m_renderer ) {
                        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
                        SDL_DestroyWindow(m_window);
                        SDL_Quit();
                    }
                    else
                    {
                        SDL_SetHint( SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "0" );
                        SDL_RenderSetLogicalSize(m_renderer, SDL_WIDTH, SDL_HEIGHT);
                        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
                        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
                        
                        SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "best" );
                        TTF_Init();
                        set_bg("#112238");

                        m_grid_font.load_font_data( m_theme, m_renderer );
                        m_text_font_tiny.load_font_data( m_theme, m_renderer );
                        m_text_font_big.load_font_data( m_theme, m_renderer );
                        m_text_font_small.load_font_data( m_theme, m_renderer );
                        m_text_font.load_font_data( m_theme, m_renderer );
                        success = true;
                    }
                }
            }
        }

        return success;
    }

    /**************************
     *
     *************************/
    void close()
    {
        std::cout << "destroy sdl..\n";

        m_text_font.close();
        m_text_font_tiny.close();
        m_text_font_big.close();
        m_text_font_small.close();
        m_grid_font.close();

        clear_texture_cache();

        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        TTF_Quit();
        SDL_Quit();
    }

    /**************************
     *
     *************************/
    void clr()
    {
        // clear canvas and init with background image
        SDL_RenderClear(m_renderer);

        // draw bg
        if ( m_bg_img_path[0] == '#' )
        {
            draw_color_plane( WtCoord( 0, 0 ),
                              WtDim( SDL_WIDTH, SDL_HEIGHT ),
                              m_bg_img_path );
        }
        else
        {
            draw_image( WtCoord( 0, 0 ),
                        WtDim( SDL_WIDTH, SDL_HEIGHT ),
                        m_bg_img_path );
        }
    }

    /**************************
     *
     *************************/
    virtual void set_theme( std::string name )
    {
        // todo base class themeable
        m_grid_font.set_theme( name, m_renderer );
        m_text_font.set_theme( name, m_renderer );
        m_text_font_tiny.set_theme( name, m_renderer );
        m_text_font_big.set_theme( name, m_renderer );
        m_text_font_small.set_theme( name, m_renderer );
        m_theme = name;
        // TODO check if available and if not keep old
        
        clear_texture_cache();
    }

    /**************************
     *
     *************************/
    void draw_custom_cell_bg( uint8_t     row,
                              uint8_t     col,
                              std::string image,
                              uint8_t     alpha=255 )
    {
        draw_image( grid_pos_to_screen_pos( row, col, &m_grid_font ),
                    m_grid_font.size(),
                    image,
                    alpha );
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

        if ( value >= selected_font->start_symbol() )
        {
            std::string val_str = WtL10n::localize_specials( std::string(1, value) );
            WtCoord pos = grid_pos_to_screen_pos_text( row, col, val_str, selected_font );

            if ( value == ' ' )
            {
                draw_custom_cell_bg( row, col,
                                     "grid_font_bg_space.bmp" );
            }
            else
            {
                std::string font_bg = "grid_font_bg.bmp";
                SDL_Color font_col = {0x12, 0x23, 0x39, 255};

                if ( font != "grid" )
                {
                    if ( font == "active_grid" )
                    {
                        font_bg = "grid_font_bg_active.bmp";
                        font_col.r = 0x12;
                        font_col.g = 0x23;
                        font_col.b = 0x39;
                        font_col.a = 255;
                    }
                    else
                    {
                        font_bg = "grid_font_bg_inverse.bmp";
                        font_col.r = 0x12;
                        font_col.g = 0x23;
                        font_col.b = 0x39;
                        font_col.a = 255;
                    }
                }

                draw_custom_cell_bg( row, col, font_bg );
                if ( value == '*' )
                {
                    draw_custom_cell_bg( row, col,
                                         "bomb.bmp" );
                }
                else
                {
                    puts_fb( pos.x, pos.y, val_str, selected_font, font_col );
                }
            }
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
    void draw_color_plane( const WtCoord     pos,
                           const WtDim       size,
                           const std::string fname )
    {
        SDL_Texture* plane = get_texture( fname );
        SDL_Rect rect;
        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = size.w;
        rect.h = size.h;

        SDL_RenderCopy(m_renderer, plane, NULL, &rect);
    }

    /**************************
      *
      *************************/   
    WtDim get_image_size( const std::string fname )
    {
        std::string fname_real = fname;
        if ( fname == "right.bmp" )
        {
            fname_real = "left.bmp";
        }
        if ( fname == "next_btn.bmp" )
        {
            fname_real = "back_btn.bmp";
        }


        return WtSdlUtils::get_image_size( fname_real, m_theme );
    }


    /**************************
      *
      *************************/   
    void draw_image( const WtCoord     pos,
                     const WtDim       size,
                     const std::string fname,
                     uint8_t           alpha=255 )
    {
        bool mirror_vertical = false;
        std::string fname_real = fname;
        if ( fname == "right.bmp" )
        {
            fname_real = "left.bmp";
            mirror_vertical = true;
        }
        if ( fname == "next_btn.bmp" )
        {
            fname_real = "back_btn.bmp";
            mirror_vertical = true;
        }


        SDL_Texture*  button_img = get_texture( fname_real );
        SDL_Rect rect;
        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = size.w;
        rect.h = size.h;
        if ( alpha != 255 )
        {
            SDL_SetTextureAlphaMod( button_img, alpha );
        }
        if (mirror_vertical)
        {
            SDL_RenderCopyEx(m_renderer, button_img, NULL, &rect, 180, NULL, SDL_FLIP_VERTICAL);
        }
        else
        {
            SDL_RenderCopy(m_renderer, button_img, NULL, &rect);
        }
    }

    /**************************
      *
      *************************/   
    void draw_text( const WtCoord     pos,
                    const std::string text,
                    const std::string font="text",
                    const std::string font_color="#4a90e2" )
    {
        if ( font == "text" )
        {
            puts_fb( pos.x, pos.y, text, &m_text_font, WtSdlUtils::get_color_from_string( font_color ) );
        }
        else if ( font == "text_tiny" )
        {
            puts_fb( pos.x, pos.y, text, &m_text_font_tiny, WtSdlUtils::get_color_from_string( font_color ) );
        }
        else if ( font == "text_big" )
        {
            puts_fb( pos.x, pos.y, text, &m_text_font_big, WtSdlUtils::get_color_from_string( font_color ) );
        }
        else if ( font == "text_small" )
        {
            puts_fb( pos.x, pos.y, text, &m_text_font_small, WtSdlUtils::get_color_from_string( font_color ) );
        }
        else
        {
            WtCoord working_pos = pos;
            for ( size_t c_idx = 0; c_idx < text.length(); c_idx++ )
            {
                std::string t = WtL10n::localize_specials( std::string(1, text[c_idx] ) );
                WtCoord screen_pos(0,0);
                ssize_t in_cell_x_offset = 0;
                ssize_t in_cell_y_offset = 0;
                WtDim c_size = m_grid_font.text_size( t );
                in_cell_x_offset = (static_cast<ssize_t>(m_grid_font.width())/2)-(c_size.w/2);
                in_cell_y_offset = ((c_size.h - static_cast<ssize_t>(m_grid_font.height())) / 2) + 1;
                screen_pos.x = working_pos.x + in_cell_x_offset;
                screen_pos.y = working_pos.y - in_cell_y_offset;

                draw_image( working_pos, m_grid_font.size(), "grid_font_bg.bmp" );
                if ( text == "*" )
                {
                    draw_image( working_pos, m_grid_font.size(), "bomb.bmp" );
                }
                else
                {
                    puts_fb( screen_pos.x, screen_pos.y, t, &m_grid_font, {0x12, 0x23, 0x39, 255}  );
                }
                working_pos.x = working_pos.x + m_grid_font.size().w + 2;
            }
        }
    }

    /**************************
     *
     *************************/
    WtDim get_font_size()
    {
        return m_text_font.size();
    }

    /**************************
     *
     *************************/
    WtDim get_grid_font_size()
    {
        return m_grid_font.size();
    }

    /**************************
     *
     *************************/
    const WtSdlFont& get_text_font()
    {
        return m_text_font;
    }

    /**************************
     *
     *************************/
    const WtSdlFont& get_text_font_tiny()
    {
        return m_text_font_tiny;
    }

    /**************************
     *
     *************************/
    WtDim get_text_size( const std::string &str, const std::string font="text" )
    {
        if ( font == "text" )
            return m_text_font.text_size( str );
        else if ( font == "text_tiny" )
            return m_text_font_tiny.text_size( str );
        else if ( font == "text_big" )
            return m_text_font_big.text_size( str );
        else if ( font == "text_small" )
            return m_text_font_small.text_size( str );
        else
            return m_grid_font.text_size( str );
    }

    /**************************
     *
     *************************/   
    WtCoord grid_pos_to_coord( uint8_t r, uint8_t c )
    {
        WtCoord grid_coord = grid_pos_to_screen_pos( r, c, &m_grid_font );
        grid_coord.x = grid_coord.x + static_cast<ssize_t>(m_grid_font.width() / 2);
        draw_red_debug_dot( grid_coord );
        return grid_coord;
    }

private:
    WtDrawingPolicySdl( const WtDrawingPolicySdl& ); 
    WtDrawingPolicySdl & operator = ( const WtDrawingPolicySdl& );

    /**************************
     *
     *************************/   
    WtCoord grid_pos_to_screen_pos( uint8_t row, uint8_t col, WtSdlFont* font )
    {
        WtCoord screen_pos(0,0);
        if ( NULL != font )
        {
            screen_pos.x = (((col*static_cast<ssize_t>(font->width()))+col)+GRID_OFFSET_X);
            screen_pos.y = (((row*static_cast<ssize_t>(font->height()))+row)+GRID_OFFSET_Y);
        }
        return screen_pos;
    }

    /**************************
     *
     *************************/   
    WtCoord grid_pos_to_screen_pos_text( uint8_t row, uint8_t col, std::string c,
                                    WtSdlFont* font )
    {
        WtCoord screen_pos = grid_pos_to_screen_pos( row, col, font );
        if ( NULL != font )
        {
            ssize_t in_cell_x_offset = 0;
            ssize_t in_cell_y_offset = 0;
            if ( c != "" )
            {
                WtDim c_size = font->text_size( c );
                in_cell_x_offset = (static_cast<ssize_t>(font->width())/2)-(c_size.w/2) - 1;
                in_cell_y_offset = ((c_size.h - static_cast<ssize_t>(font->height())) / 2) + 4;
            }
            screen_pos.x = screen_pos.x + in_cell_x_offset;
            screen_pos.y = screen_pos.y - in_cell_y_offset;
        }
        return screen_pos;
    }

    /**************************
     *
     *************************/
    void puts_fb( ssize_t x, ssize_t y, const std::string str, WtSdlFont* font, SDL_Color font_color={0x4a, 0x90, 0xe2,255} )
    {
        if ( NULL != font )
        {
            if ( font->is_ttf() )
            {
                WtCoord pos( x, y );

                WtDim size = font->text_size( str );
                SDL_Texture* text_tex = nullptr;
                std::string index = str;
                {
                    std::stringstream ss;
                    ss << index;
                    ss << std::hex << static_cast<size_t>(font_color.r);
                    ss << std::hex << static_cast<size_t>(font_color.g);
                    ss << std::hex << static_cast<size_t>(font_color.b);
                    ss << std::hex << static_cast<size_t>(font_color.a);
                    index = ss.str();
                }

                SDL_TextureCache::const_iterator it = m_texture_cache.find(index);
                if ( it != m_texture_cache.end() )
                {
                    // load from cache
                    text_tex = (*it).second;
                }
                else
                {
                    text_tex = font->write_text( str, size, font_color, m_renderer );
                    if ( nullptr != text_tex )
                    {
                        m_texture_cache[index] = text_tex;
                    }
                }

                if ( text_tex != nullptr )
                {
                    SDL_Rect small;
                    small.x = pos.x;
                    small.y = pos.y;
                    small.w = size.w;
                    small.h = size.h;
//                    draw_red_debug_dot( pos );
//                    draw_red_debug_dot( WtCoord(pos.x+size.w, pos.y+size.h) );
                    SDL_RenderCopy( m_renderer, text_tex, NULL, &small );
                }
            }
            else
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
                SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_BLEND );
                SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
                SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 0 );
                SDL_RenderClear( m_renderer );
                SDL_RenderFillRect( m_renderer, NULL );
                SDL_SetRenderTarget( m_renderer, NULL );

                m_texture_cache[fname] = tex;
            }
            else
            {
                if ( fname[0] == '#' )
                {
                    std::cout << "load from color.. " << fname << std::endl;
                    SDL_Color color = WtSdlUtils::get_color_from_string( fname );
                    tex = SDL_CreateTexture( m_renderer,
                                             SDL_PIXELFORMAT_RGBA8888, 
                                             SDL_TEXTUREACCESS_TARGET, 
                                             1, 1 );
                    SDL_SetRenderTarget( m_renderer, tex );
                    SDL_SetRenderDrawBlendMode( m_renderer, SDL_BLENDMODE_BLEND );
                    SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
                    SDL_SetRenderDrawColor( m_renderer, color.r, color.g, color.b, 255 );
                    SDL_RenderClear( m_renderer );
                    SDL_RenderFillRect( m_renderer, NULL );
                    SDL_SetRenderTarget( m_renderer, NULL );

                    m_texture_cache[fname] = tex;
                }
                else
                {
                    std::cout << "load from file.. " << fname << std::endl;
                    m_texture_cache[fname] = WtSdlUtils::load_asset_to_texture( m_renderer, fname, m_theme );
                }

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


    void draw_red_debug_dot( const WtCoord& pos )
    {
        /* Declaring the surface. */
        SDL_Surface *s;
        /* Creating the surface. */
        s = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
        /* Filling the surface with red color. */
        SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 255, 0, 0));
        SDL_Texture* t = SDL_CreateTextureFromSurface( m_renderer, s );
        SDL_FreeSurface(s);
        SDL_Rect small;
        small.x = pos.x;
        small.y = pos.y;
        small.w = 1;
        small.h = 1;
        SDL_RenderCopy( m_renderer, t, NULL, &small );
        SDL_DestroyTexture( t );
    }

private:
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;

    std::string   m_bg_img_path;
    std::string   m_theme;

    WtSdlFont     m_grid_font;
    WtSdlFont     m_text_font;
    WtSdlFont     m_text_font_tiny;
    WtSdlFont     m_text_font_big;
    WtSdlFont     m_text_font_small;

    SDL_TextureCache m_texture_cache;
};



#endif /* _WT_SDL_H */
