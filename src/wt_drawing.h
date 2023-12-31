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
#include "widgets/wt_textbox.h"

#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"

#include "wt_string_utils.h"



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
    std::string points_to_string( uint32_t points )
    { 
        if ( points < 1000 )
        {
            
        }
        return std::to_string( points );
    }

    /**************************
     *
     *************************/
    void draw_player_stat( const WtPlayer& player )
    {
        
        DrawingPolicy::draw_image( WtCoord( 20, 2 ),
                                   WtDim( 65, 75 ),
                                   "level_progress_bg_neu.bmp" );

        std::string player_scores = "";
        uint32_t points = player.get_points();
        player_scores.append( points_to_string( points ) );
        WtCoord base_pos( 22,8 );
        if ( points < 1000 )
        {
            WtDim font_sz = DrawingPolicy::get_text_size( player_scores );
            WtCoord score_pos = base_pos;
            score_pos.x = (score_pos.x + 65/2)-((font_sz.w/2)+2);
            score_pos.y = (score_pos.y+(65/2))-((font_sz.h/2)+2);
            
            DrawingPolicy::draw_text( score_pos,
                                      player_scores, "text", "#00bafa" );
        }
        else
        {
            WtDim font_sz = DrawingPolicy::get_text_size( player_scores, "text_small" );
            WtCoord score_pos = base_pos;
            score_pos.x = (score_pos.x + 65/2)-((font_sz.w/2)+2);
            score_pos.y = (score_pos.y+(65/2))-((font_sz.h/2)+2);
            
            DrawingPolicy::draw_text( score_pos,
                                      player_scores, "text_small", "#00bafa" );
        }

        draw_level_up_indicator( player, WtCoord( 20, 2 ) );
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
                    if ( show_support_grid )
                    {
                        std::string cell_background = "grid.bmp";
                        DrawingPolicy::draw_custom_cell_bg( WtBoard::row_count-i, j, cell_background );
                        if ( ( i < active.current_row() ) && ( j == active.current_column() ) )
                        {
                            cell_background = "grid_font_helper.bmp";
                            uint16_t alpha_diff = 40 * (active.current_row() - i);
                            DrawingPolicy::draw_custom_cell_bg( WtBoard::row_count-i, j, cell_background, ( alpha_diff < 255 ? 255 - alpha_diff : 0 ) );
                        }
                    }
                    if ( ( i > active.current_row() ) && ( j == active.current_column() ) )
                    {
                        std::string cell_background = "grid_font_helper.bmp";
                        uint16_t alpha_diff = 40 * (i - active.current_row());
                        DrawingPolicy::draw_custom_cell_bg( WtBoard::row_count-i, j, cell_background, ( alpha_diff < 255 ? 255 - alpha_diff : 0 ) );
                    }
                }
            }
        }

        DrawingPolicy::draw_at_grid( WtBoard::row_count - active.current_row(),
                                     active.current_column(),
                                     active.current_value(),
                                     "active_grid" );
    }


    /**************************
     *
     *************************/
    void draw_hint( const std::string letter_after_next )
    {
        DrawingPolicy::draw_text( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - ((static_cast<ssize_t>(letter_after_next.length()) * (DrawingPolicy::get_grid_font_size().w+2)) / 2), 12 ),
                                  letter_after_next,
                                  "grid" );
    }


    /**************************
     *
     *************************/
    void draw_help( const WtCoord pos, const std::string hint )
    {
        WtDim icon_info_size = WtDim(60, 60);
        WtCoord icon_info_pos = pos; //WtCoord( 79, 890 );

        DrawingPolicy::draw_image( pos,
                                   icon_info_size,
                                   "icon_info.bmp" );

        const ssize_t line_length = (icon_info_pos.x + icon_info_size.w)+350;
        
        std::vector<std::string> hint_words = split(hint);
        WtCoord cursor_pos(icon_info_pos.x + icon_info_size.w, icon_info_pos.y + 5);
        for ( size_t w_idx = 0; w_idx<hint_words.size(); w_idx++)
        {
            std::string word = hint_words[w_idx];
            word.append(" ");
            WtDim w_sz = DrawingPolicy::get_text_size( word );
           // std::cout << "word = " << word << " sz = " << w_sz.w << " cursor_pos = " << cursor_pos << std::endl;
            if ( (cursor_pos.x + w_sz.w) > line_length )
            {
                cursor_pos.x = icon_info_pos.x + icon_info_size.w;
                cursor_pos.y += (DrawingPolicy::get_font_size().h * 2);
            }
            DrawingPolicy::draw_text( cursor_pos,
                                      word );
            cursor_pos.x += w_sz.w;
        }
    }


    /**************************
     *
     *************************/
    void draw_textbox( WtTextbox& textbox )
    {
        // DrawingPolicy::draw_image( textbox.position(),
        //                            textbox.size(),
        //                            textbox.background() );

        WtCoord text_pos = textbox.text_position();
        std::vector<std::string>& lines = textbox.lines();
        for( size_t l_idx = 0; l_idx < lines.size(); l_idx++ )
        {
            WtDim t_sz = DrawingPolicy::get_text_size( lines[l_idx], textbox.font() );

            DrawingPolicy::draw_text( text_pos,
                                      lines[l_idx],
                                      textbox.font() );

            text_pos.y += t_sz.h;
        }
    }


    /**************************
      *
      *************************/   
    bool draw_sine_scroller_text( std::string text, WtCoord pos )
    {
        bool restart_needed = true;
        WtCoord working_pos = pos;
        for ( size_t c_idx=0; c_idx < text.length(); c_idx++ )
        {
            const char& c = text[c_idx];
            WtCoord c_pos = working_pos;
            if ( c_idx > 0 )
                c_pos.x += 20;

            double max_width = ACTIVE_WINDOW_WIDTH;
            double freq_pos = c_pos.x;

            //c_pos.y = pos.y + ( sin( c_pos.x ) * 15 );
            c_pos.y = cos(12.0 * freq_pos / max_width ) * 40.0 + pos.y;

            DrawingPolicy::draw_text( c_pos, std::string( 1, c ) );

            if ( ( c_pos.x >= 0 ) && ( c_pos.x <= ACTIVE_WINDOW_WIDTH ) )
                restart_needed = false;

            working_pos.x = c_pos.x;
        }
        return restart_needed;
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
                ssize_t button_center_x;
                ssize_t button_center_y;
                button_center_x = ( ( button.width() / 2 ) + button.x() ) + button.label_pos().x;
                button_center_y = ( ( button.height() / 2 ) + button.y() ) + button.label_pos().y;

                std::string label = button.label();
                if ( label.find_first_of("#@") != 0 )
                {
                    label = WtL10n::translate( label );
                    WtDim font_sz = DrawingPolicy::get_text_size( label, button.label_font().fontname );
                    WtCoord label_pos( button.x() + button.label_pos().x, button.y() + button.label_pos().y);
                    if ( button.label_centered() )
                    {
                        ssize_t text_center_w;

                        text_center_w = font_sz.w / 2;
                        button_center_x = ( ( button.width() / 2 ) + button.x() ) + button.label_pos().x;
                        label_pos.x = button_center_x - text_center_w;                        
                    }

                    button_center_y = ( ( button.height() / 2 ) + button.y() ) + button.label_pos().y;
                    label_pos.y = button_center_y - (font_sz.h/2);

                    DrawingPolicy::draw_text( label_pos,
                                              label,
                                              button.label_font().fontname,
                                              button.label_font().color );
                }
                else
                {
                    if ( label[0] == '#' )
                        label = label.substr(1);

                    WtDim sz = DrawingPolicy::get_image_size( label );
                    DrawingPolicy::draw_image( WtCoord( button_center_x - (sz.w/2), button_center_y - (sz.h/2) ),
                                               sz,
                                               label );
                }

            }
        }
    }

private:
    /**************************
      *
      *************************/   
    void draw_level_up_indicator( const WtPlayer& player, const WtCoord& base_pos )
    {
        size_t w_count = static_cast<size_t>(player.words_since_levelup());
        if ( w_count > 0 )
        {
            const WtDim indicator_bar_sizes[10] = {
                {11, 16}, {12, 8}, {12, 2}, {12, 9}, {10, 15}, {9, 13}, {12, 9}, {12, 2}, {12, 8}, {10, 14}
            };
            const WtCoord indicator_bar_pos[10] = {
                {4, 13}, {15, 8}, {27, 8}, {39, 8}, {51, 14}, {51, 48}, {39, 58}, {27, 65}, {15, 59}, {5, 48}
            };
            std::string fname = "hud_level_progress_";

            if ( w_count >= 1 )
            {
                DrawingPolicy::draw_image( base_pos + indicator_bar_pos[0],
                                           indicator_bar_sizes[0],
                                           std::string( fname ).append("1").append(".bmp") );
            }

            if ( w_count > 1 )
            {
                for ( size_t w_count_idx = 2; w_count_idx <= w_count; w_count_idx++ )
                {
                    DrawingPolicy::draw_image( base_pos + indicator_bar_pos[w_count_idx - 1],
                                               indicator_bar_sizes[w_count_idx - 1],
                                               std::string( fname ).append( std::to_string(w_count_idx) ).append(".bmp") );
                }
            }
        }
    }
};


#endif /* _WT_DRAWING_H */
