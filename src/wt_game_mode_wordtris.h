/*!*****************************************************************************
 * @file wt_game_mode_wordtris.h
 * @brief implementation of game mode "wordtris classic mode"
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
#ifndef _WT_GAME_MODE_WORDTRIS_H_
#define _WT_GAME_MODE_WORDTRIS_H_

#include "wt_game_mode_if.h"
#include "wt_random.h"

class WtGameModeWordtris : public WtGameModeIf
{
public:
    WtGameModeWordtris() :
        m_letters( "ABCDEFGHIJKLMNOPQRSTUVWXYZ#" )
    {
    }
    ~WtGameModeWordtris()
    {
    }

    /**************************
     *
     *************************/
    virtual std::string get_title()
    {
        return WtL10n::tr("Classic Mode");
    }

    /**************************
     *
     *************************/
    virtual std::string get_id_string()
    {
        //shall not be translated
        return "WordtrisClassic";
    }

    /**************************
     * return false if game over
     *************************/
    virtual bool eval_board( WtBoard& board, WtPlayer& player )
    {
        bool game_continue = true;
#if 0
        if ( m_active_word_guessed.empty() )
        {
            bool found_word = false;

            // search rows for word
            //TODO: use DEA instead of string construction
            for ( uint8_t r_idx = 0; r_idx < WtBoard::row_count; r_idx++ )
            {
                std::string row = std::string("");
                for ( uint8_t c_idx = 0; c_idx < WtBoard::col_count; c_idx++ )
                {
                    char cell = board.get_cell( r_idx, c_idx );
                    if ( cell != WtBoard::empty_cell )
                        row.push_back( cell );
                    else
                        row.push_back( ' ' );
                }
              
                size_t found_idx = row.find( m_active_word );
                if ( found_idx != std::string::npos )
                {
                    for ( size_t c_idx = found_idx; c_idx < (m_active_word.length()+found_idx); c_idx++ )
                        board.set_cell( r_idx, (uint8_t)(c_idx), WtBoard::empty_cell );
                    //TODO gravity bitch!
                    found_word = true;
                    break;
                }
            }

            if ( found_word )
            {
                player.word_solved();
                get_next_word();
                game_continue = true;
            }
            else
            {
                get_next_word();
                game_continue = true;
            }
        }
#endif
        return game_continue;
    }

    /**************************
     *
     *************************/
    virtual char next_letter()
    {
        return WtRandom::get_random_letter_of_word( m_letters ); 
    }

    /**************************
     *
     *************************/
    virtual std::string get_hint()
    {
        return WtL10n::tr("try to build 3+ letter words!");
    }


    /**************************
     *
     *************************/
    virtual bool stone_blocked( WtBoard& board,
                                uint8_t  row,
                                uint8_t  col )
    {
        return ( board.cell_occupied( row, col ) || row < (WtBoard::row_count/2-1) );
    }
    
    /**************************
     *
     *************************/
    virtual void insert_stone_at( WtBoard& board,
                                  uint8_t  row,
                                  uint8_t  col,
                                  char     value )
    {
        board.set_cell( row, col, value );
    }


private:
    WtGameModeWordtris( const WtGameModeWordtris& ); 
    WtGameModeWordtris & operator = (const WtGameModeWordtris &);





private:
    std::string m_letters;
};

#endif /* _WT_GAME_MODE_WORDTRIS_H_ */
