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
                     //WtDim(100, 100), 
                     WtDim(138, 124), 
                     "next_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuScoreSummary::leave ) ),
        m_textbox( WtCoord( (ACTIVE_WINDOW_WIDTH - 379) / 2,
                            ((ACTIVE_WINDOW_HEIGHT / 2) - (608 / 2)) - 50 ),
                   "", ACTIVE_WINDOW.get_text_font() ),
        m_new_highscore( false ),
        m_last_points(0),
        m_solved_words{0,0,0,0}
    {
        add_button( m_leave_btn );
        add_textbox( m_textbox );
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

        m_new_highscore   = insert_entry( scores, new_entry );
        m_last_points     = player.get_points();
        m_solved_words[0] = player.get_solved_word_count();
        m_solved_words[1] = player.get_solved_word_count(2);
        m_solved_words[2] = player.get_solved_word_count(3);
        m_solved_words[3] = player.get_solved_word_count(4);
    }

private: // no copy allowed
    WtMenuScoreSummary( const WtMenuScoreSummary& ); 
    WtMenuScoreSummary& operator = ( const WtMenuScoreSummary& );

    /**************************
     *
     *************************/
    virtual void entered_view()
    {
        std::vector<std::string>& tb_lines = m_textbox.lines();

        tb_lines.clear();
        tb_lines.push_back("");
        tb_lines.push_back("");
        if ( m_new_highscore )
        {
            tb_lines.push_back("wow! new highscore!");
        }
        else
        {
            tb_lines.push_back("you lost! :P");
        }
        tb_lines.push_back("");
        tb_lines.push_back("");
        tb_lines.push_back("");

        std::string score_summary = "points       " + std::to_string( m_last_points );
        tb_lines.push_back( score_summary );
        tb_lines.push_back("");
        score_summary = "words        " + std::to_string( static_cast<ssize_t>(m_solved_words[0]) ); 
        tb_lines.push_back( score_summary );
        score_summary = "          2x     " + std::to_string( static_cast<ssize_t>(m_solved_words[1]) ); 
        tb_lines.push_back( score_summary );
        score_summary = "          3x     " + std::to_string( static_cast<ssize_t>(m_solved_words[2]) ); 
        tb_lines.push_back( score_summary );
        score_summary = "          4x     " + std::to_string( static_cast<ssize_t>(m_solved_words[3]) ); 
        tb_lines.push_back( score_summary );

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
    WtButton  m_leave_btn;
    WtTextbox m_textbox;
    bool      m_new_highscore;
    uint32_t  m_last_points;
    uint16_t  m_solved_words[4];
};

#endif /* _WT_MENU_SCORE_SUMMARY_H_ */
