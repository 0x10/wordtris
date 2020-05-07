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
        WtViewIf("#202020"),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     WtDim(138, 124),
                     "next_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHighscores::leave ),
                     WtL10n_tr( "B A C K"),
                     WtCoord( 0, 25 ),
                     WtFont( "#a8a8a8", "text_big" ) ),
        m_left_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 8)-(80/2), ((ACTIVE_WINDOW_HEIGHT / 8)+150)-(10) ), 
                    WtDim(80, 42), 
                    "left.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuHighscores::show_prev_mode ) ),
        m_right_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-(ACTIVE_WINDOW_WIDTH / 8))-(80/2), ((ACTIVE_WINDOW_HEIGHT / 8)+150)-(10) ), 
                     WtDim(80, 42), 
                     "right.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHighscores::show_next_mode ) ),
        m_title_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (188 / 2), (ACTIVE_WINDOW_HEIGHT / 8)-(134/2) ),
                       WtDim( 188, 134 ),
                       "label_score.bmp",
                       [](){},
                       "",
                       WtCoord( 0, 160 ),
                       WtFont( "#a8a8a8", "text_big" ) ),
        m_diff_left_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 8)-(40/2), ((ACTIVE_WINDOW_HEIGHT / 2))-(10) ), 
                    WtDim(60, 32), 
                    "left.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuHighscores::show_prev_diff ) ),
        m_diff_right_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-(ACTIVE_WINDOW_WIDTH / 8))-(40/2), ((ACTIVE_WINDOW_HEIGHT / 2))-(10) ), 
                     WtDim(60, 32), 
                     "right.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHighscores::show_next_diff ) ),
        m_diff_title_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (188 / 2), (ACTIVE_WINDOW_HEIGHT / 8)-(134/2) ),
                       WtDim( 188, 134 ),
                       "label_score.bmp",
                       [](){},
                       "",
                       WtCoord( 0, 200 ),
                       WtFont( "#a8a8a8", "text" ) ),
        m_list_bg( WtCoord(0, (ACTIVE_WINDOW_HEIGHT / 4)-50), WtDim(ACTIVE_WINDOW_WIDTH, 650), "#0e0e0e" ),
        m_scores( STORAGE.get_scores() ),
        m_game_mode_ids( GAME_MODE_CTR.get_available_mode_ids() ),
        m_game_mode_names( GAME_MODE_CTR.get_available_mode_names() ),
        m_selected_mode( 0 ),
        m_selected_diff( wt_difficulty_EASY )
    {
        WtL10n::register_lang_change_obsever( WT_BIND_EVENT_HANDLER( WtMenuHighscores::language_changed ) );

        add_button( m_list_bg );
        add_button( m_leave_btn );
#ifdef WT_PRO_MODE
        if ( m_game_mode_names.size() > 1 )
        {
            add_button( m_left_btn );
            add_button( m_right_btn );
        }
#endif
        add_button( m_title_btn );
        add_button( m_diff_title_btn );
        add_button( m_diff_left_btn );
        add_button( m_diff_right_btn );

        set_diff_label();
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
            m_selected_mode = m_game_mode_names.size() - 1;
        }
        else
        {
            m_selected_mode--;
        }
        m_title_btn.set_label( m_game_mode_names[m_selected_mode] );
    }

    /**************************
     *
     *************************/
    void show_next_mode()
    {
        m_selected_mode = ( m_selected_mode + 1 ) % m_game_mode_names.size();
        m_title_btn.set_label( m_game_mode_names[m_selected_mode] );
    }


    /**************************
     *
     *************************/
    void set_diff_label()
    {
        std::vector<std::pair<wt_difficulty, const char*> > diff_label= WtGameModeIf::get_available_difficulties();
        for( size_t i = 0; i < diff_label.size(); i++ )
        {
            if ( diff_label[i].first == m_selected_diff )
            {
                m_diff_title_btn.set_label( WtL10n::translate( WtGameModeIf::get_available_difficulties()[i].second) );
            }
        }
    }

    /**************************
     *
     *************************/
    void show_prev_diff()
    {
        switch(m_selected_diff)
        {
            case wt_difficulty_EASY:
                m_selected_diff = wt_difficulty_HARD;
                break;
            case wt_difficulty_MEDIUM:
                m_selected_diff = wt_difficulty_EASY;
                break;
            case wt_difficulty_HARD:
                m_selected_diff = wt_difficulty_MEDIUM;
                break;
            default:break;
        }
        set_diff_label();
    }

    /**************************
     *
     *************************/
    void show_next_diff()
    {
        switch(m_selected_diff)
        {
            case wt_difficulty_EASY:
                m_selected_diff = wt_difficulty_MEDIUM;
                break;
            case wt_difficulty_MEDIUM:
                m_selected_diff = wt_difficulty_HARD;
                break;
            case wt_difficulty_HARD:
                m_selected_diff = wt_difficulty_EASY;
                break;
            default:break;
        }
        set_diff_label();
    }

    /**************************
     *
     *************************/
    void language_changed()
    {
        m_game_mode_names = GAME_MODE_CTR.get_available_mode_names();
        m_title_btn.set_label( m_game_mode_names[m_selected_mode] );
    }

    /**************************
     *
     *************************/
    void draw_entry( size_t index, WtScoreEntry& entry )
    {
        WtDim   font_size = ACTIVE_WINDOW.get_font_size();
        WtCoord entry_pos ( (ACTIVE_WINDOW_WIDTH / 3) ,
                            (((ACTIVE_WINDOW_HEIGHT / 4)) + (font_size.h + (font_size.h / 2))) );

        entry_pos.y = entry_pos.y + ( static_cast<ssize_t>(index) * (font_size.h + (font_size.h)) );

        {
            std::stringstream ss;
            ss << index;
            ACTIVE_WINDOW.draw_text( entry_pos, ss.str(), "text", "#b2b2b2" ); 
        }

        if ( entry.score > 0 )
        {
            entry_pos.x = entry_pos.x + (font_size.w * 4);

            {
                std::string level_label = "Level ";

                std::stringstream ss;
                ss << WtTime::format_time( WtTime::from_seconds( entry.time_s ));
                std::string score = ss.str();
                ACTIVE_WINDOW.draw_text( entry_pos, score ); 
            }

        }
    }

    /**************************
     *
     *************************/
    void entered_view()
    {
        m_selected_mode = GAME_MODE_CTR.mode_idx_from_string( STORAGE.get_settings().game_mode );
        m_selected_diff = wt_difficulty_EASY;
        m_title_btn.set_label( m_game_mode_names[m_selected_mode] );
        set_diff_label();
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

            if (( entry.game_mode == m_game_mode_ids[m_selected_mode] )
              && (entry.diff == m_selected_diff))
            {
                draw_entry( score_entry, entry );
                score_entry++;
            }
        }
        if ( score_entry <= 10 )
        {
            WtScoreEntry entry;
            for(; score_entry <= 10; score_entry++ )
            {
                draw_entry( score_entry, entry );
            }
        }
    }

private:
    WtButton                       m_leave_btn;
    WtButton                       m_left_btn;
    WtButton                       m_right_btn;
    WtButton                       m_title_btn;
    WtButton                       m_diff_left_btn;
    WtButton                       m_diff_right_btn;
    WtButton                       m_diff_title_btn;
    WtButton                       m_list_bg;
    WtHighscores&                  m_scores;
    const std::vector<std::string> m_game_mode_ids;
    std::vector<std::string>       m_game_mode_names;
    size_t                         m_selected_mode;
    wt_difficulty                  m_selected_diff;
};

#endif /* _WT_MENU_HIGHSCORES_H_ */
