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
#include "wt_animation_builder.h"

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
        return "@dark/label_mode_guessing_" + WtL10n::get_language_code() + ".bmp";
    }

    /**************************
     *
     *************************/
    std::string get_name()
    {
        return WtL10n::translate( WtL10n_tr("Guess the word") );
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
    virtual void eval_board( WtBoard& board, WtPlayer& player, WtGameModeState& gs )
    {
        if ( ( m_active_word_guessed.empty() ) && ( m_next == WtBoard::empty_cell ) )
        {
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
                    // found! finish                    

                    {
                        WtGridAnimation* blink = new WtGridAnimation("wordsolved.ogg");
                        WtGridAnimation::GridAnimationStep step( WtGridAnimation::fromGridText( WtGridAnimation::GridText( WtBoard::row_count-r_idx,
                                                                                            found_idx,
                                                                                            true,
                                                                                            m_active_word, 
                                                                                            "grid_inverse" ) ),
                                                                 WtTime::from_milliseconds(200) );
                        blink->push_back( step );
                        step.content.font = "grid";
                        blink->push_back( step );
                        step.content.font = "grid_inverse";
                        blink->push_back( step );
                        gs.add_animation( blink ); // will handle destruction
                    }

                    {
                        uint32_t current_level = player.get_current_level();
                        player.word_solved();
                        if ( player.get_current_level() != current_level )
                        {
                            WtGridAnimation* blink = new WtGridAnimation("levelup.ogg");
                            WtGridAnimationBuilder::construct_level_up_animation( *blink );
                            gs.add_animation( blink ); // will handle destruction
                        }
                    }

                    
                    for ( size_t c_idx = found_idx; c_idx < (m_active_word.length()+found_idx); c_idx++ )
                        board.set_cell( r_idx, static_cast<uint8_t>(c_idx), WtBoard::empty_cell );

                    // found! finish
                    break;
                }
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
    virtual std::string letter_after_next()
    {
        return m_active_word_scrambled;
    }


    /**************************
     *
     *************************/
    std::string get_hint()
    {
        return WtL10n::translate( WtL10n_tr("Try to solve the puzzled word by placing the stones in correct order.") );
    }

private:
    WtGameModeGuessing( const WtGameModeGuessing& ); 
    WtGameModeGuessing & operator = (const WtGameModeGuessing &);


    /**************************
     *
     *************************/
    void get_next_word()
    {
        wt_difficulty diff = get_difficulty();
        size_t min_len = 0;
        size_t max_len = 4;
        switch( diff )
        {
            case wt_difficulty_EASY:   break;
            case wt_difficulty_MEDIUM: min_len = 4; max_len = 7; break;
            case wt_difficulty_HARD:   min_len = 6; max_len = 9; break;
            default: break;
        }
        size_t idx = 0;
        uint8_t buf;
        const std::vector<std::string>& guess_list = m_wordlist.get_word_list( min_len, max_len );
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
