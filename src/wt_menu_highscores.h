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

#include "wt_menu_if.h"

class WtMenuHighscores : public WtMenuIf
{
public:
    WtMenuHighscores() :
        WtMenuIf( 0x400, "bg_menu_settings.bmp" ),
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
        m_scores( STORAGE.get_scores() ),
        m_game_mode_titles( GAME_MODE_CTR.get_available_mode_titles() ),
        m_selected_mode( 0 )
    {
        add_button( m_leave_btn );
        add_button( m_left_btn );
        add_button( m_right_btn );
    }
    virtual ~WtMenuHighscores() {}
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
    }

    /**************************
     *
     *************************/
    void show_next_mode()
    {
        m_selected_mode = ( m_selected_mode + 1 ) % m_game_mode_titles.size();
    }

    /**************************
     *
     *************************/
    void draw_title( const std::string& title )
    {
        WtDim   font_size = ACTIVE_WINDOW.get_font_size();
        WtCoord title_pos ( (ACTIVE_WINDOW_WIDTH / 2)-((static_cast<ssize_t>(title.length())*font_size.w) / 2),
                            (ACTIVE_WINDOW_HEIGHT / 8) + (2*font_size.h) );

        ACTIVE_WINDOW.draw_text( title_pos, title );
        ACTIVE_WINDOW.draw_text( WtCoord( title_pos.x, title_pos.y + font_size.h ), std::string("").append(title.length(), '=') ); 
    }

    /**************************
     *
     *************************/
    void draw_entry( size_t index, WtScoreEntry& entry )
    {
        WtDim   font_size = ACTIVE_WINDOW.get_font_size();
        WtCoord entry_pos ( 100,
                            ((ACTIVE_WINDOW_HEIGHT / 8) + (2*font_size.h))+ (6*font_size.h) );

        entry_pos.y = entry_pos.y + ( static_cast<ssize_t>(index) * (3*font_size.h) );

        {
            std::stringstream ss;
            ss << index;
            ACTIVE_WINDOW.draw_text( entry_pos, ss.str() ); 
        }

        entry_pos.x = entry_pos.x + (font_size.w * 4);

        {
            std::string level_label = "Level ";
            ACTIVE_WINDOW.draw_text( entry_pos, level_label );
            entry_pos.x = entry_pos.x + (font_size.w * static_cast<ssize_t>(level_label.length()+1));
            std::stringstream ss;
            ss << entry.level;
            ACTIVE_WINDOW.draw_text( entry_pos, ss.str() ); 
        }

        {
            std::stringstream ss;
            ss << entry.score;
            entry_pos.x = (ACTIVE_WINDOW_WIDTH - 100) - (font_size.w * static_cast<ssize_t>(ss.str().length()));
            ACTIVE_WINDOW.draw_text( entry_pos, ss.str() ); 
        }
    }

    /**************************
     *
     *************************/
    virtual void menu_update()
    {
        draw_title( m_game_mode_titles[m_selected_mode] );

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
        WtScoreEntry e;
        e.score = 100000;
        draw_entry( score_entry++, e );
        e.score = 99;
        draw_entry( score_entry++, e );
        e.score = 999;
        draw_entry( score_entry++, e );
        e.score = 9999;
        draw_entry( score_entry++, e );
        e.score = 99999;
        draw_entry( score_entry++, e );
        draw_entry( score_entry++, e );
        draw_entry( score_entry++, e );
        draw_entry( score_entry++, e );
        draw_entry( score_entry++, e );
        draw_entry( score_entry++, e );
    }

private:
    WtButton                       m_leave_btn;
    WtButton                       m_left_btn;
    WtButton                       m_right_btn;
    WtHighscores&                  m_scores;
    const std::vector<std::string> m_game_mode_titles;
    size_t                         m_selected_mode;
};

#endif /* _WT_MENU_HIGHSCORES_H_ */
