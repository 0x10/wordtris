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
        WtViewIf( "bg_menu_pause.bmp", false ),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-105) - 100, 800 ), 
                     WtDim(100, 100), 
                     "next_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuScoreSummary::leave ) ),
        m_new_highscore( false )
    {
        add_button( m_leave_btn );
    }
    ~WtMenuScoreSummary() {}

    /**************************
     *
     *************************/
    void update_highscores( WtPlayer& player, WtGameModeIf* mode, WtHighscores& scores )
    {
        // eval if player stat is within first 3 of game mode
        // if true add player at correct position
        std::cout << "highscore entry: Lvl " << player.get_current_level() << " at " << player.get_points() << " within mode \"" << mode->get_title() << "\"" << std::endl;

        WtScoreEntry new_entry( mode->get_title(),
                                player.get_points(),
                                player.get_current_level() );

        m_new_highscore = insert_entry( scores, new_entry );
    }

private: // no copy allowed
    WtMenuScoreSummary( const WtMenuScoreSummary& ); 
    WtMenuScoreSummary& operator = ( const WtMenuScoreSummary& );

    /**************************
     *
     *************************/
    virtual void update_view()
    {
        if ( m_new_highscore )
        {
            ACTIVE_WINDOW.draw_message(WtL10n_tr("wow! new highscore"));
        }
        else
        {
            ACTIVE_WINDOW.draw_message(WtL10n_tr("you lost! :P"));
        }
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

private:
    WtButton m_leave_btn;
    bool     m_new_highscore;
};

#endif /* _WT_MENU_SCORE_SUMMARY_H_ */
