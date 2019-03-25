/*!*****************************************************************************
 * @file wt_game_mode_tetris.h
 * @brief implementation for game mode "tetris like build rows of equal symbols"
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
#ifndef _WT_GAME_MODE_TETRIS_H_
#define _WT_GAME_MODE_TETRIS_H_

#include "wt_game_mode_if.h"
#include "wt_random.h"

class WtGameModeTetris : public WtGameModeIf
{
public:
    WtGameModeTetris() :
        WtGameModeIf( "Fill Rows" ),
        m_active_wordbase( "()+&" )
    {
    }
    ~WtGameModeTetris()
    {
    }

    /**************************
     *
     *************************/
    virtual std::string get_title()
    {
        return WtL10n::tr("Fill Rows");
    }

    /**************************
     * return false if game over
     *************************/
    virtual bool eval_board( WtBoard& board, WtPlayer& player )
    {
        bool game_continue = true;

        // search rows for word
        for ( uint8_t r_idx = 0; r_idx < WtBoard::row_count; r_idx++ )
        {
            bool row_full = true;
            size_t active_word = 0;
            std::string words[WtBoard::col_count];

            words[active_word].push_back( board.get_cell( r_idx, 0 ) );
            for ( uint8_t c_idx = 1; c_idx < WtBoard::col_count; c_idx++ )
            {
                char next = board.get_cell( r_idx, c_idx );
                if ( next != '\0' )
                {
                    if ( words[active_word][0] != next )
                    {
                        active_word++;
                    }
                    words[active_word].push_back( next );
                }
                else
                {
                    row_full = false;
                }
            }

            if ( row_full )
            {
                uint32_t points = 0;
                for ( uint8_t w_idx = 0; w_idx < WtBoard::col_count; w_idx ++ )
                {
                    points += (words[w_idx].length() * words[w_idx].length()) * 20;
                }
                player.row_cleared( points );
                /* drop remaining stones afterwards */
                board.collapse_above( r_idx );
            }
        }

        return game_continue;
    }

    /**************************
     *
     *************************/
    virtual char next_letter()
    {
        return WtRandom::get_random_letter_of_word( m_active_wordbase ); 
    }

    /**************************
     *
     *************************/
    virtual std::string get_hint()
    {
        return WtL10n::tr("build rows.\nthe more equal symbols, more points.");
    }

private:
    WtGameModeTetris( const WtGameModeTetris& ); 
    WtGameModeTetris & operator = (const WtGameModeTetris &);



private:
    std::string m_active_wordbase;
};

#endif /* _WT_GAME_MODE_TETRIS_H_ */
