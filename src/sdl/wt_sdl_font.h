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
#include "SDL_ttf.h"

#include "wt_sdl_utils.h"

class WtSdlFont
{
private:
    const char m_start_symbol = 0x20; // font data starts at printable space
public:
    WtSdlFont( std::string name="", 
               size_t w=0,
               size_t h=0,
               std::string filename="",
               size_t font_size =0,
               bool is_ttf=false ) :
        m_name( name ),
        m_font_w( w ),
        m_font_h( h ),
        m_fname( filename ),
        m_font_data(),
        m_is_ttf( is_ttf ),
        m_ttf_font(NULL),
        m_font_size(font_size)
    {
    }
    ~WtSdlFont()
    {
    }

    /**************************
     *
     *************************/
    bool is_ttf() const
    {
        return m_is_ttf;
    }


    /**************************
     *
     *************************/
    void close()
    {
        for(size_t i = 0; i < m_font_data.size(); i++)
            SDL_DestroyTexture(m_font_data[i]);

        if ( NULL != m_ttf_font )
        {
            std::cout << "destroy font\n";
            TTF_CloseFont( m_ttf_font );
            m_ttf_font = NULL;
        }
    }
    /**************************
     *
     *************************/
    void load_font_data( std::string theme, SDL_Renderer* renderer )
    {
        std::cout << "load font data = "<< m_fname <<std::endl;
        if ( ! m_is_ttf )
        {
            if ( m_font_data.size() > 0 )
            {
                for(size_t i = 0; i < m_font_data.size(); i++)
                    SDL_DestroyTexture( m_font_data[i] );
                m_font_data.clear();
            }

            SDL_Texture* font_complete = WtSdlUtils::load_asset_to_texture( renderer, m_fname, theme );
            // fonts are organized by ascii code...
            for(char i = m_start_symbol; i < '~'; i++ )
            {
                m_font_data.push_back( get_letter_texture( font_complete, i, renderer ) );
            }
            SDL_DestroyTexture(font_complete);
        }
        else
        {
            //Open the font 
            if ( m_ttf_font == NULL )
            {
                std::cout << "load font\n";
                m_ttf_font = TTF_OpenFont( m_fname.c_str(), m_font_size ); 
                if( m_ttf_font == NULL ) 
                { 
                    std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                }
            }
        }
    }

    /**************************
     *
     *************************/
    void set_theme( std::string name, SDL_Renderer* renderer )
    {
        if ( ! m_is_ttf )
            load_font_data( name, renderer );
    }

    /**************************
     *
     *************************/
    size_t width() const
    {
        return m_font_w;
    }

    /**************************
     *
     *************************/
    size_t height() const
    {
        return m_font_h;
    }

    /**************************
     *
     *************************/
    WtDim size() const
    {
        return WtDim( static_cast<ssize_t>(m_font_w),
                      static_cast<ssize_t>(m_font_w/*m_font_h*/) ); //intentional square
    }

    /**************************
     *
     *************************/
    WtDim text_size( const std::string text ) const
    {
        WtDim sz(0,0);
        if ( m_is_ttf )
        {
            int w=0;
            int h=0;
            TTF_SizeUTF8( m_ttf_font, text.c_str(), &w, &h );
            sz.w = w;
            sz.h = h;
        }
        return sz;
    }

    /**************************
     *
     *************************/
    SDL_Texture* write_text( std::string text, WtDim& sz, SDL_Color color, SDL_Renderer* renderer )
    {
        SDL_Texture* text_tex = NULL;
        if ( m_is_ttf )
        {
            SDL_Surface* text_surface = TTF_RenderUTF8_Blended( m_ttf_font, text.c_str(), color );
            if ( NULL != text_surface )
            {
                text_tex = SDL_CreateTextureFromSurface( renderer, text_surface );
                sz.w = text_surface->w;
                sz.h = text_surface->h;
                SDL_FreeSurface( text_surface );
            }
        }
        return text_tex;
    }

    /**************************
     *
     *************************/
    void write( WtCoord pos, char ch, SDL_Renderer* renderer )
    {
        SDL_Texture* glyph = NULL;
        WtDim glyph_size( static_cast<ssize_t>(m_font_w), static_cast<ssize_t>(m_font_h) );

        if ( ! m_is_ttf )
        {
            if ( ch >= m_start_symbol )
            {
                glyph = m_font_data[static_cast<size_t>(ch - m_start_symbol)];
            }
        }
        else
        {
            SDL_Color text_color = { 0x4a, 0x90, 0xe2, 0 };
            SDL_Surface* text_surface = TTF_RenderUTF8_Blended( m_ttf_font, std::string( &ch, 1 ).c_str(), text_color );
            if ( NULL != text_surface )
            {
                glyph = SDL_CreateTextureFromSurface( renderer, text_surface );
                glyph_size.w = text_surface->w;
                glyph_size.h = text_surface->h;
                SDL_FreeSurface( text_surface );
            }
        }

        if ( NULL != glyph )
        {
            SDL_Rect small;
            small.x = pos.x;
            small.y = pos.y;
            small.w = glyph_size.w;
            small.h = glyph_size.h;

            SDL_RenderCopy(renderer, glyph, NULL, &small );

            if ( m_is_ttf )
                SDL_DestroyTexture( glyph );
        }
    }

    /**************************
     *
     *************************/
    char start_symbol() const
    {
        return m_start_symbol;
    }

private:
    WtSdlFont( const WtSdlFont& ); 
    WtSdlFont & operator = ( const WtSdlFont& );

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
        letter_rect.x = static_cast<size_t>(letter - m_start_symbol) * m_font_w;
        letter_rect.y = 0;
        letter_rect.w = m_font_w;
        letter_rect.h = m_font_h;
        return GetAreaTexture( letter_rect, renderer, t_letters );
    }

private:
    std::string                 m_name;
    size_t                      m_font_w;
    size_t                      m_font_h;
    std::string                 m_fname;
    std::vector<SDL_Texture*>   m_font_data;
    bool                        m_is_ttf;
    TTF_Font*                   m_ttf_font;
    size_t                      m_font_size;
};

#endif /* _WT_SDL_FONT_H_ */
