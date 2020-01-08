/*!*****************************************************************************
 * @file wt_menu_score_summary.h
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
#ifndef _WT_MENU_SCORE_SUMMARY_H_
#define _WT_MENU_SCORE_SUMMARY_H_

#include "wt_view_if.h"

class WtMenuScoreSummary : public WtViewIf
{
public:
    WtMenuScoreSummary() :
        WtViewIf( "#202020", 0, 1 ),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ),
                     WtDim( 138, 124 ),
                     "next_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuScoreSummary::leave ),
                     WtL10n_tr( "D O N E"),
                     WtCoord( 0, 25 ),
                     WtFont( "#a8a8a8", "text_big" ) ),
        m_scores_logo( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (188 / 2), (ACTIVE_WINDOW_HEIGHT / 8)-(134/2) ),
                       WtDim( 188, 134 ),
                       "label_score.bmp",
                       [](){},
                       "",
                       WtCoord( 0, 200 ),
                       WtFont( "#a8a8a8", "text_big" ) ),
        m_textbox_bg( WtCoord( 0,(ACTIVE_WINDOW_HEIGHT / 2) - 150 ),
                      WtDim( ACTIVE_WINDOW_WIDTH, 300 ), "#0e0e0e" ),
        m_points_txt( WtCoord( (ACTIVE_WINDOW_WIDTH / 8 ) + 160, (ACTIVE_WINDOW_HEIGHT / 2) - 112),
                    WtDim( 1, 1 ),
                   "", [](){},  WtL10n_tr("TIME") ),
        m_textbox0( WtCoord( (ACTIVE_WINDOW_WIDTH / 8 ) + 100, (ACTIVE_WINDOW_HEIGHT / 2) - 150),
                    WtDim( 0, 0 ),
                   "", ACTIVE_WINDOW.get_text_font() ),
        m_textbox1( WtCoord( ACTIVE_WINDOW_WIDTH / 2, (ACTIVE_WINDOW_HEIGHT / 2) - 150),
                    WtDim( 0, 0 ),
                   "", ACTIVE_WINDOW.get_text_font() ),
        m_new_highscore( false ),
        m_last_time(WtTime::from_seconds(0))
    {
        add_button( m_leave_btn );
        add_button( m_scores_logo );
        add_button( m_textbox_bg );
        add_button( m_points_txt );
        add_textbox( m_textbox0 );
        add_textbox( m_textbox1 );
    }
    ~WtMenuScoreSummary() {}

    /**************************
     *
     *************************/
    void update_highscores( WtPlayer& player, WtGameModeIf* mode, WtHighscores& scores )
    {
        // eval if player stat is within first 3 of game mode
        // if true add player at correct position
        size_t time_s = static_cast<size_t>(std::chrono::duration_cast<std::chrono::seconds>(player.get_current_time()).count());
        std::cout << "highscore entry: Lvl " << player.get_current_level() << " at " << player.get_points() << " t= " << time_s  << " within mode \"" << mode->get_title() << "\"" << std::endl;

        WtScoreEntry new_entry( mode->get_id_string(),
                                player.get_points(),
                                player.get_current_level(),
                                time_s,
                                mode->get_difficulty() );

        m_new_highscore   = insert_entry_by_time( scores, new_entry );
        m_last_time     = player.get_current_time();
    }

private: // no copy allowed
    WtMenuScoreSummary( const WtMenuScoreSummary& ); 
    WtMenuScoreSummary& operator = ( const WtMenuScoreSummary& );

    /**************************
     *
     *************************/
    virtual void entered_view()
    {
        std::vector<std::string>& tb_lines = m_textbox0.lines();
        std::vector<std::string>& tb_lines1 = m_textbox1.lines();

        tb_lines.clear();
        tb_lines1.clear();
        if ( m_new_highscore )
        {
            m_scores_logo.set_label(WtL10n_tr( "N E W    H I G H S C O R E !"));
        }
        else
        {
            m_scores_logo.set_label(WtL10n_tr( "   Y O U R    R E S U L T !  "));
        }

        std::string score_summary;
        std::string points = WtTime::format_time( m_last_time );
        tb_lines.push_back( score_summary );
        tb_lines1.push_back( points );

    }

    /**************************
     * signal
     *************************/
    virtual void left_view()
    {
        m_new_highscore = false;
    }

    /**************************
     *
     *************************/
    bool insert_entry( WtHighscores& scores, WtScoreEntry& entry )
    {
        bool entry_added = false;
        size_t game_mode_entries=0;
        for( size_t idx = 0; ( idx < scores.size() && game_mode_entries < 10 ); idx++ )
        {
            if ( scores[idx].game_mode == entry.game_mode )
            {
                if ( scores[idx].score < entry.score )
                {
                    WtHighscores::iterator it = scores.begin();
                    std::advance( it, idx );
                    scores.insert( it, entry );
                    entry_added = true;
                    break;
                }
                game_mode_entries++;
            }
        }

        if ( ( !entry_added ) && ( game_mode_entries < 10 ) )
        {
            scores.push_back( entry );
            entry_added = true;
        }
        return entry_added;
    }

    /**************************
     *
     *************************/
    bool insert_entry_by_time( WtHighscores& scores, WtScoreEntry& entry )
    {
        bool entry_added = false;
        size_t game_mode_entries=0;
        for( size_t idx = 0; ( idx < scores.size() && game_mode_entries < 10 ); idx++ )
        {
            if ( scores[idx].game_mode == entry.game_mode )
            {
                if ( scores[idx].time_s > entry.time_s )
                {
                    WtHighscores::iterator it = scores.begin();
                    std::advance( it, idx );
                    scores.insert( it, entry );
                    entry_added = true;
                    break;
                }
                game_mode_entries++;
            }
        }

        if ( ( !entry_added ) && ( game_mode_entries < 10 ) )
        {
            scores.push_back( entry );
            entry_added = true;
        }
        return entry_added;
    }
private:
    WtButton  m_leave_btn;
    WtButton  m_scores_logo;
    WtButton  m_textbox_bg;
    WtButton  m_points_txt;
    WtTextbox m_textbox0;
    WtTextbox m_textbox1;
    bool      m_new_highscore;
    WtTime::TimeType m_last_time;
};

#endif /* _WT_MENU_SCORE_SUMMARY_H_ */
