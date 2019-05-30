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
#include "wt_utils.h"
#include "wt_wordlist.h"

class WtGameModeGuessing : public WtGameModeIf
{
public:
    WtGameModeGuessing() :
        WtGameModeIf( "Guess it" ),
        m_active_word( "BlAcK" ),
        m_active_word_guessed(""),
        m_active_word_scrambled(""),
        m_next(' '),
        m_wordlist("short.txt")
    {
    }
    ~WtGameModeGuessing()
    {
    }

    /**************************
     *
     *************************/
    std::string get_title()
    {
        return WtL10n_tr("Guess it!");
    }

    /**************************
     * 
     *************************/
    void init_game( WtBoard&, WtPlayer& )
    {
        get_next_word();
        m_next = WtRandom::get_random_letter_of_word( m_active_word_guessed );
        remove_letter( m_active_word_guessed, m_next );
    }

    /**************************
     *
     *************************/
    virtual void eval_board( WtBoard& board, WtPlayer& player, WtGameModeState& )
    {
        if ( ( m_active_word_guessed.empty() ) && ( m_next == WtBoard::empty_cell ) )
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
                        board.set_cell( r_idx, static_cast<uint8_t>(c_idx), WtBoard::empty_cell );
                    found_word = true;
                    break;
                }
            }

            if ( found_word )
            {
                player.word_solved();
            }

            get_next_word();
            m_next = WtRandom::get_random_letter_of_word( m_active_word_guessed );
            remove_letter( m_active_word_guessed, m_next );
        }
    }

    /**************************
     *
     *************************/
    char next_letter()
    {
        char next = m_next;
        if ( ! m_active_word_guessed.empty() )
        {
            m_next = WtRandom::get_random_letter_of_word( m_active_word_guessed );
            remove_letter( m_active_word_guessed, m_next );
        }
        else
        {
            m_next = WtBoard::empty_cell;
        }
        // else shall not happen -> game loop take care if eval_board checks for empty
        // and reloads whatever may next

        return next; 
    }

    /**************************
     *
     *************************/
    char letter_after_next()
    {
        return m_next;
    }


    /**************************
     *
     *************************/
    std::string get_hint()
    {
        return std::string( WtL10n_tr("Guess the word: ") ).append(m_active_word_scrambled);
    }

private:
    WtGameModeGuessing( const WtGameModeGuessing& ); 
    WtGameModeGuessing & operator = (const WtGameModeGuessing &);


    /**************************
     *
     *************************/
    void get_next_word()
    {
        size_t idx = 0;
        uint8_t buf;
        const std::vector<std::string>& guess_list = m_wordlist;
        if ( WtRandom::getrandom( &buf, 1 ) <= 1 )
        {
            idx = (buf % guess_list.size());
        }
        m_active_word = guess_list[idx];
        m_active_word_guessed = m_active_word;
        m_active_word_scrambled = scramble( m_active_word );
    }

    /**************************
     *
     *************************/
    void remove_letter( std::string& word, char letter )
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
    std::string scramble( std::string word )
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
    char        m_next;
    WtWordList  m_wordlist;
};

#endif /* _WT_GAME_MODE_GUESSING_H_ */
