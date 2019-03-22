/*!*****************************************************************************
 * @file wt_drawing.h
 * @brief generic, configurable implementation for drawing
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
#ifndef _WT_DRAWING_H_
#define _WT_DRAWING_H_

#include "global.h"
#include "wt_button.h"
#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"

#define WINDOW( Policy )  WtDrawing<Policy>::instance()

template<typename DrawingPolicy>
class WtDrawing : private DrawingPolicy
{
// singleton definition
public:
    static WtDrawing& instance()
    {
        static WtDrawing _instance;
        return _instance;
    }
    ~WtDrawing() {}
private:
    WtDrawing() :
        DrawingPolicy()
    {
    }
    WtDrawing( const WtDrawing& ); 
    WtDrawing & operator = (const WtDrawing &);

// api defintion
public:
    /**************************
     *
     *************************/
    void init() {}

    /**************************
     *
     *************************/
    void clr()
    {
        DrawingPolicy::clr();
    }

    /**************************
     *
     *************************/
    void draw_player_stat( const WtPlayer& player )
    {
        std::string player_stat = "Score: ";
        player_stat.append( std::to_string( player.get_points() ) );

        DrawingPolicy::draw_image( WtCoord( 79, 32 ),
                                   WtDim( 256, 65 ),
                                   "label_bg.bmp" );
        DrawingPolicy::draw_text( WtCoord( 90, 50 ),
                                  player_stat );
    }

    /**************************
     *
     *************************/
    void draw_board( const WtBoard& board )
    {
        DrawingPolicy::draw_board( board );
    }

    /**************************
     *
     *************************/
    void draw_hint( const std::string hint )
    {
        DrawingPolicy::draw_text( WtCoord( 79, 890 ),
                                  hint );
    }

    /**************************
     *
     *************************/
    void draw_active_letter( const WtLetter& active )
    {
        DrawingPolicy::draw_active_letter( active );
    }

     /**************************
      *
      *************************/   
    void draw_message( const std::string msg )
    {
        //todo replace with proper message box
        WtDim sz( 328, 69 );
        WtCoord pos( (ACTIVE_WINDOW_WIDTH - 328) / 2,
                     (ACTIVE_WINDOW_HEIGHT / 2) - (69 / 2) );
        DrawingPolicy::draw_image( pos,
                                   sz,
                                   "menu_btn.bmp" );
        WtDim font_sz = DrawingPolicy::get_font_size();

        size_t text_center_w = ( msg.length() / 2 ) * font_sz.w;
        size_t button_center_x = ( sz.w / 2 + pos.x );
        size_t button_center_y = ( sz.h / 2 + pos.y );

        DrawingPolicy::draw_text( WtCoord( button_center_x - text_center_w, 
                                           button_center_y - font_sz.h ),
                                  msg );
    }

    /**************************
     *
     *************************/
    void update()
    {
        DrawingPolicy::update( );
    }

    /**************************
     *
     *************************/
    void set_bg( const std::string bg_file )
    {
        DrawingPolicy::set_bg( bg_file );
    }

    /**************************
      *
      *************************/   
    void draw_text( const WtCoord&    pos,
                    const std::string text )
    {
        DrawingPolicy::draw_text( pos, text );
    }

    /**************************
      *
      *************************/   
    void draw_button( WtButton& button )
    {
        DrawingPolicy::draw_image( WtCoord( button.x(), button.y() ),
                                   WtDim( button.width(), button.height() ),
                                   button.image() );

        if ( ! button.label().empty() )
        {
            WtDim font_sz = DrawingPolicy::get_font_size();

            size_t text_center_w = ( button.label().length() / 2 ) * font_sz.w;
            size_t button_center_x = ( button.width() / 2 + button.x() );
            size_t button_center_y = ( button.height() / 2 + button.y() );

            draw_text( WtCoord( button_center_x - text_center_w,
                                button_center_y - font_sz.h ),
                                button.label() );
        }
    }

    /**************************
      *
      *************************/   
    void draw_pause_button( const WtCoord& pos )
    {
        DrawingPolicy::draw_image( pos,
                                   WtDim( 42, 42 ),
                                   "pause.bmp" );
    }

    /**************************
      *
      *************************/   
    void draw_icon( const WtCoord&    pos,
                    const std::string fname )
    {
        DrawingPolicy::draw_image( pos,
                                   WtDim( 500, 80 ),//todo get_texture_size( buttom_img );
                                   fname );
    }
private:
};


#endif /* _WT_DRAWING_H */
