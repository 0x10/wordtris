/*!*****************************************************************************
 * @file wt_menu_highscores.h
 * @brief menu class for game scores
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
#ifndef _WT_MENU_HIGHSCORES_H_
#define _WT_MENU_HIGHSCORES_H_

#include "wt_view_if.h"

class WtMenuHighscores : public WtViewIf
{
public:
    WtMenuHighscores() :
        WtViewIf(),
        m_leave_btn( WtCoord( 105, 800 ), 
                     WtDim(100, 100), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHighscores::leave ) ),
        m_left_btn( WtCoord( 25, (ACTIVE_WINDOW_HEIGHT/2)-50 ), 
                    WtDim(40, 40), 
                    "left.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuHighscores::show_prev_mode ) ),
        m_right_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-25)-40, (ACTIVE_WINDOW_HEIGHT/2)-50 ), 
                     WtDim(40, 40), 
                     "right.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHighscores::show_next_mode ) ),
        m_title_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2)-(379 / 2),
                              (ACTIVE_WINDOW_HEIGHT / 8)  ),
                     WtDim( 379, 100 ),
                     "score_header.bmp" ),
        m_scores( STORAGE.get_scores() ),
        m_game_mode_titles( GAME_MODE_CTR.get_available_mode_titles() ),
        m_selected_mode( 0 )
    {
        add_button( m_leave_btn );
        add_button( m_left_btn );
        add_button( m_right_btn );
        add_button( m_title_btn );
    }
    ~WtMenuHighscores() {}
private: // no copy allowed
    WtMenuHighscores( const WtMenuHighscores& ); 
    WtMenuHighscores & operator = (const WtMenuHighscores &);

    /**************************
     *
     *************************/
    void show_prev_mode()
    {
        if ( m_selected_mode == 0 )
        {
            m_selected_mode = m_game_mode_titles.size() - 1;
        }
        else
        {
            m_selected_mode--;
        }
        m_title_btn.set_label( m_game_mode_titles[m_selected_mode] );
    }

    /**************************
     *
     *************************/
    void show_next_mode()
    {
        m_selected_mode = ( m_selected_mode + 1 ) % m_game_mode_titles.size();
        m_title_btn.set_label( m_game_mode_titles[m_selected_mode] );
    }

    /**************************
     *
     *************************/
    void draw_entry( size_t index, WtScoreEntry& entry )
    {
        WtDim   font_size = ACTIVE_WINDOW.get_font_size();
        WtCoord entry_pos ( 100,
                            ((ACTIVE_WINDOW_HEIGHT / 8) + (5*font_size.h)) );

        entry_pos.y = entry_pos.y + ( static_cast<ssize_t>(index) * (font_size.h + (font_size.h/2)) );

        {
            std::stringstream ss;
            ss << index;
            ACTIVE_WINDOW.draw_text( entry_pos, ss.str() ); 
        }

        entry_pos.x = entry_pos.x + (font_size.w * 4);

        {
            std::string level_label = "Level ";
            WtDim text_font_size = ACTIVE_WINDOW.get_text_size( level_label );

            ACTIVE_WINDOW.draw_text( entry_pos, level_label );
            entry_pos.x = entry_pos.x + text_font_size.w;
            std::stringstream ss;
            ss << entry.level;
            ACTIVE_WINDOW.draw_text( entry_pos, ss.str() ); 
        }

        {
            std::stringstream ss;
            ss << entry.score;
            WtDim text_font_size = ACTIVE_WINDOW.get_text_size( ss.str() );
            entry_pos.x = (ACTIVE_WINDOW_WIDTH - 100) - (text_font_size.w);
            ACTIVE_WINDOW.draw_text( entry_pos, ss.str() ); 
        }
    }

    /**************************
     *
     *************************/
    void entered_view()
    {
        m_selected_mode = GAME_MODE_CTR.mode_idx_from_string( STORAGE.get_settings().game_mode );
        m_title_btn.set_label( m_game_mode_titles[m_selected_mode] );
    }

    /**************************
     *
     *************************/
    void update_view()
    {
        size_t score_entry = 1;

        for( size_t e_idx=0; (e_idx < m_scores.size()) && (score_entry <= 10); e_idx++ )
        {
            WtScoreEntry entry =  m_scores[e_idx];
            if ( entry.game_mode == m_game_mode_titles[m_selected_mode] )
            {
                draw_entry( score_entry, entry );
                score_entry++;
            }
        }
    }

private:
    WtButton                       m_leave_btn;
    WtButton                       m_left_btn;
    WtButton                       m_right_btn;
    WtButton                       m_title_btn;
    WtHighscores&                  m_scores;
    const std::vector<std::string> m_game_mode_titles;
    size_t                         m_selected_mode;
};

#endif /* _WT_MENU_HIGHSCORES_H_ */
