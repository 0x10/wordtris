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
    void draw_note( const uint8_t  r,
                    const uint8_t  c,
                    const uint8_t  v )
    {
        std::string text = std::string(1, 0x30 + v);
        WtCoord grid_pos = DrawingPolicy::grid_pos_to_coord( r, c );
        WtDim text_size = DrawingPolicy::get_text_size( text, "text_small" );

        grid_pos.x += 5 + ( ((v-1)%3) * (2 * text_size.w + (text_size.w / 2)) );
        if ( v > 3 )
        {
            grid_pos.y += text_size.h;
            if ( v > 6 )
            {
                grid_pos.y += text_size.h;
            }
        }

        grid_pos.y -= 5;

        DrawingPolicy::draw_text( grid_pos, text, "text_small", "#757575" );
    }

    /**************************
     *
     *************************/
    void draw_board( const WtBoard& board, 
                     const WtLetter& active,
                     const bool show_support_grid )
    {
        DrawingPolicy::set_gridsize( STORAGE.get_settings().gridsize );
        DrawingPolicy::draw_custom_cell_bg( active.current_row(), active.current_column(), "#777777", 70 );

        bool draw_error_state = STORAGE.get_settings().show_error_on_input;

        for( uint8_t i=0 ; i < board.row_count(); i++ )
        {
            for( uint8_t j=0; j < board.col_count(); j++ )
            {

                char val = board.get_cell( i, j );


                if ( val != WtBoard::empty_cell )
                {
                    if ( draw_error_state && board.is_erroneous( i, j ) )
                    {
                        DrawingPolicy::draw_at_grid( i, j, val, "grid_error" );
                    }
                    else
                    {
                        DrawingPolicy::draw_at_grid( i, j, val );
                    }
                }
                else
                {
                    std::string cell_background = "";// "grid.bmp";
                    DrawingPolicy::draw_custom_cell_bg( i, j, cell_background, 155 );

                    for ( uint8_t note_val : std::array<uint8_t, 9>{1,2,3,4,5,6,7,8,9} )
                    {
                        if ( board.is_note_set( i, j, note_val ) )
                        {
                            draw_note( i, j, note_val );
                        }
                    }
                }

                if ( show_support_grid )
                {
                    std::string cell_background = "";//"grid.bmp";
                    if ( j == active.current_column() )
                    {
                        cell_background = "grid_font_helper_col.bmp";
                        DrawingPolicy::draw_custom_cell_bg( i, j, cell_background );
                    }
                    if ( i == active.current_row() )
                    {
                        cell_background = "grid_font_helper.bmp";
                        DrawingPolicy::draw_custom_cell_bg( i, j, cell_background );
                    }
                }
            }
        }

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
            if ( button.image().length() > 0 )
            {
                DrawingPolicy::draw_image( button.position(),
                                           button.size(),
                                           button.image() );
            }

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


};


#endif /* _WT_DRAWING_H */
