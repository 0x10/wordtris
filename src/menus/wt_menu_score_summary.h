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
        WtViewIf( "#182e4b", false ),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ),
                     WtDim( 138, 124 ),
                     "start_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuScoreSummary::leave ),
                     WtL10n_tr( "D O N E"),
                     WtCoord( -20, 25 ),
                     WtFont( "#498fe1", "text_big" ) ),
        m_scores_logo( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (188 / 2), (ACTIVE_WINDOW_HEIGHT / 8)-(134/2) ),
                       WtDim( 188, 134 ),
                       "label_score.bmp",
                       [](){},
                       "",
                       WtCoord( -50, 200 ),
                       WtFont( "#498fe1", "text_big" ) ),
        m_textbox_bg( WtCoord( 0,(ACTIVE_WINDOW_HEIGHT / 2) - 150 ),
                      WtDim( ACTIVE_WINDOW_WIDTH, 300 ), "#122338" ),
        m_textbox0( WtCoord( ACTIVE_WINDOW_WIDTH / 8, (ACTIVE_WINDOW_HEIGHT / 2) - 150),
                    WtDim( 0, 0 ),
                   "", ACTIVE_WINDOW.get_text_font() ),
        m_textbox1( WtCoord( ACTIVE_WINDOW_WIDTH / 2, (ACTIVE_WINDOW_HEIGHT / 2) - 150),
                    WtDim( 0, 0 ),
                   "", ACTIVE_WINDOW.get_text_font() ),
        m_new_highscore( false ),
        m_last_points(0),
        m_solved_words{0,0,0,0}
    {
        add_button( m_leave_btn );
        add_button( m_scores_logo );
        add_button( m_textbox_bg );
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

        std::string score_summary = "               POINTS";
        std::string points = std::to_string( m_last_points );
        tb_lines.push_back( score_summary );
        tb_lines1.push_back( points );
        tb_lines.push_back("");
        tb_lines1.push_back( "" );
        score_summary = "SOLVED WORDS";
        points = std::to_string( static_cast<ssize_t>(m_solved_words[0]) );
        tb_lines1.push_back( points );
        tb_lines.push_back( score_summary );
        if ( m_solved_words[1] > 0 )
        {
            score_summary = "                  2x";
            points = std::to_string( static_cast<ssize_t>(m_solved_words[1]) );
            tb_lines.push_back( score_summary );
            tb_lines1.push_back( points );
        }
        if ( m_solved_words[2] > 0 )
        {
            score_summary = "                  3x";
            points = std::to_string( static_cast<ssize_t>(m_solved_words[2]) );
            tb_lines.push_back( score_summary );
            tb_lines1.push_back( points );
        }
        if ( m_solved_words[3] > 0 )
        {
            score_summary = "                  4x";
            points = std::to_string( static_cast<ssize_t>(m_solved_words[3]) );
            tb_lines.push_back( score_summary );
            tb_lines1.push_back( points );
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
    WtButton  m_leave_btn;
    WtButton  m_scores_logo;
    WtButton  m_textbox_bg;
    WtTextbox m_textbox0;
    WtTextbox m_textbox1;
    bool      m_new_highscore;
    uint32_t  m_last_points;
    uint16_t  m_solved_words[4];
};

#endif /* _WT_MENU_SCORE_SUMMARY_H_ */
