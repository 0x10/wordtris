/*!*****************************************************************************
 * @file wt_game_mode_guessing.h
 * @brief implementation of game mode "guess the word by building it"
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
#ifndef _WT_GAME_MODE_GUESSING_H_
#define _WT_GAME_MODE_GUESSING_H_

#include "wt_game_mode_if.h"
#include "wt_random.h"
#include "wt_game_mode_guessing_strings.h"

class WtGameModeGuessing : public WtGameModeIf
{
public:
    WtGameModeGuessing() :
        m_active_word( "BlAcK" )
    {
        m_active_word_guessed   = m_active_word;
        m_active_word_scrambled = scramble( m_active_word );
    }
    ~WtGameModeGuessing()
    {
    }

    /**************************
     *
     *************************/
    virtual std::string get_title()
    {
        return WtL10n::tr("Guess it!");
    }

    /**************************
     *
     *************************/
    virtual std::string get_id_string()
    {
        //shall not be translated
        return "Guess it";
    }

    /**************************
     * return false if game over
     *************************/
    virtual bool eval_board( WtBoard& board, WtPlayer& player )
    {
        bool game_continue = true;

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

        return game_continue;
    }

    /**************************
     *
     *************************/
    virtual char next_letter()
    {
        char next = '#';
        if ( ! m_active_word_guessed.empty() )
        {
            next = WtRandom::get_random_letter_of_word( m_active_word_guessed );
            remove_letter( m_active_word_guessed, next );
        }
        // else shall not happen -> game loop take care if eval_board checks for empty
        // and reloads whatever may next

        return next; 
    }

    /**************************
     *
     *************************/
    virtual std::string get_hint()
    {
        return WtL10n::tr("Guess the word: ").append(m_active_word_scrambled);
    }

private:
    WtGameModeGuessing( const WtGameModeGuessing& ); 
    WtGameModeGuessing & operator = (const WtGameModeGuessing &);


    /**************************
     *
     *************************/
    virtual void get_next_word()
    {
        size_t idx = 0;
        uint8_t buf;
        if ( WtRandom::getrandom( &buf, 1 ) <= 1 )
        {
            idx = (buf % (72-20));
        }
        m_active_word = std::string(guess_list[idx]);
        m_active_word_guessed = m_active_word;
        m_active_word_scrambled = scramble( m_active_word );
    }

    /**************************
     *
     *************************/
    virtual void remove_letter( std::string& word, char letter )
    {
        size_t first_idx = word.find_first_of( letter );
        if ( first_idx != std::string::npos )
        {
            word.erase( first_idx, 1 );
        }
    }

    /**************************
     *
     *************************/
    virtual std::string scramble( std::string word )
    {
        std::string word_copy = word;
        std::string result = std::string("");

        while ( ! word_copy.empty() )
        {
            char next = WtRandom::get_random_letter_of_word( word_copy );
            result.push_back( next );
            remove_letter( word_copy, next );
        }

        return result;
    }

private:
    std::string m_active_word;
    std::string m_active_word_guessed;
    std::string m_active_word_scrambled;
};

#endif /* _WT_GAME_MODE_GUESSING_H_ */
