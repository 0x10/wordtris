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

#include "widgets/wt_button.h"
#include "widgets/wt_tristate_button.h"

#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"

#define WINDOW( Policy )  WtDrawing<Policy>::instance()

template<typename DrawingPolicy>
class WtDrawing : public DrawingPolicy
{
// singleton definition
public:
    static WtDrawing& instance()
    {
        static WtDrawing _instance;
        return _instance;
    }
    virtual ~WtDrawing() {}
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
    void draw_player_stat( const WtPlayer& player )
    {
        std::string player_stat = "";
        player_stat.append( std::to_string( player.get_current_level() ) );
        std::string player_scores = "";
        player_scores.append( std::to_string( player.get_points() ) );

        DrawingPolicy::draw_image( WtCoord( 79, 32 ),
                                   WtDim( 317, 65 ),
                                   "label_bg.bmp" );
        WtDim font_sz = DrawingPolicy::get_text_size( player_stat );
        DrawingPolicy::draw_text( WtCoord( (79+33)-(font_sz.w/2), (32+33)-((font_sz.h/2)+2) ),
                                  player_stat );
        font_sz = DrawingPolicy::get_text_size( player_scores );
        DrawingPolicy::draw_text( WtCoord( 160, (32+(65/2))-((font_sz.h/2)+2) ),
                                  player_scores );

        /* TODO: business logic code! remove from drawing */
        uint8_t solved_words = player.get_solved_word_count();
        uint8_t next_level_word_count = (( solved_words / 10 ) + 1 ) * 10;
        uint8_t words_since_levelup = 10 - ( next_level_word_count - solved_words );

        WtCoord word_fill_pos( 79, 32 );

        switch( words_since_levelup )
        {
            case 10:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x+16, word_fill_pos.y ),
                                           WtDim( 31, 5 ),
                                           "hud_level_progress_10.bmp" );
            case 9:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_9.bmp" );
            case 8:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_8.bmp" );
            case 7:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_7.bmp" );
            case 6:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_6.bmp" );
            case 5:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_5.bmp" );
            case 4:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_4.bmp" );
            case 3:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_3.bmp" );
            case 2:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_2.bmp" );
            case 1:
                DrawingPolicy::draw_image( WtCoord( word_fill_pos.x, word_fill_pos.y+5 ),
                                           WtDim( 47, 6 ),
                                           "hud_level_progress_1.bmp" );
            default:
            case 0:
            break;
        }
    }


    /**************************
     *
     *************************/
    void draw_board( const WtBoard& board, 
                     const WtLetter& active,
                     const bool show_support_grid )
    {
        for( uint8_t i=0 ; i < WtBoard::row_count; i++ )
        {
            for( uint8_t j=0; j < WtBoard::col_count; j++ )
            {
                char val = board.get_cell( i, j );
                if ( val != WtBoard::empty_cell )
                {
                    DrawingPolicy::draw_at_grid( WtBoard::row_count-i, j, val );
                }
                else
                {
                    std::string cell_background = "grid.bmp";
                    if ( ( show_support_grid ) && ( i != active.current_row() ) && ( j == active.current_column() ) )
                    {
                        cell_background = "grid_font_helper.bmp";
                    }

                    DrawingPolicy::draw_custom_cell_bg( WtBoard::row_count-i, j, cell_background );
                }
            }
        }

        DrawingPolicy::draw_at_grid( WtBoard::row_count - active.current_row(),
                                     active.current_column(),
                                     active.current_value() );
    }


    /**************************
     *
     *************************/
    void draw_hint( const std::string hint, const char letter_after_next, bool show_letter_after_next )
    {
        if ( show_letter_after_next )
        {
            DrawingPolicy::draw_text( WtCoord( 308, 46 ),
                                      std::string(1, letter_after_next ),
                                      "grid" );
        }

        const size_t line_length = 30;
        size_t line_count = (hint.length() / line_length) + ( hint.length() % line_length > 0 ? 1 : 0 );
        for(size_t l_idx = 0; l_idx < line_count; l_idx++ )
        {
            std::string line = hint.substr( l_idx*line_length, line_length );
            DrawingPolicy::draw_text( WtCoord( 79,
                                               890+(static_cast<ssize_t>(l_idx) * DrawingPolicy::get_font_size().h *2) ),
                                      line );
        }
    }

    /**************************
     *
     *************************/
    void draw_help_box( const std::string help )
    {
        const size_t line_length = 30;
        size_t line_count = (help.length() / line_length) + ( help.length() % line_length > 0 ? 1 : 0 );
        for(size_t l_idx = 0; l_idx < line_count; l_idx++ )
        {
            std::string line = help.substr( l_idx*line_length, line_length );
            DrawingPolicy::draw_text( WtCoord( (ACTIVE_WINDOW_WIDTH / 2)-((static_cast<ssize_t>(line.length())*DrawingPolicy::get_font_size().w)/2), 
                                               (ACTIVE_WINDOW_HEIGHT / 4)+(static_cast<ssize_t>(l_idx) * DrawingPolicy::get_font_size().h *2) ),
                                      line );
        }
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
        WtDim font_sz = DrawingPolicy::get_text_size( msg );

        ssize_t text_center_w = font_sz.w / 2;
        ssize_t button_center_x = ( sz.w / 2 + pos.x );
        ssize_t button_center_y = ( sz.h / 2 + pos.y );

        DrawingPolicy::draw_text( WtCoord( button_center_x - text_center_w, 
                                           button_center_y - (font_sz.h/2) ),
                                  msg );
    }


    /**************************
      *
      *************************/   
    void draw_button( WtButton button )
    {
        if ( button.size() != WtDim( 0,0 ) )
        {
            DrawingPolicy::draw_image( button.position(),
                                       button.size(),
                                       button.image() );

            if ( ! button.label().empty() )
            {
                ssize_t text_center_w;
                ssize_t button_center_x;
                ssize_t button_center_y;

                WtDim font_sz = DrawingPolicy::get_text_size( button.label() );

                text_center_w = font_sz.w / 2;
                button_center_x = ( ( button.width() / 2 ) + button.x() ) + button.label_pos().x;
                button_center_y = ( ( button.height() / 2 ) + button.y() ) + button.label_pos().y;

                DrawingPolicy::draw_text( WtCoord( button_center_x - text_center_w,
                                                   button_center_y - (font_sz.h/2)  ),
                                          button.label() );
            }
        }
    }

};


#endif /* _WT_DRAWING_H */
