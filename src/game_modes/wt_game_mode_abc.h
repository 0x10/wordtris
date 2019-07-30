/*!*****************************************************************************
 * @file wt_game_mode_abc.h
 * @brief implementation for game mode "guess the letters" 
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
#ifndef _WT_GAME_MODE_ABC_H_
#define _WT_GAME_MODE_ABC_H_

#include "wt_game_mode_if.h"
#include "wt_utils.h"


class WtGameModeAbc : public WtGameModeIf
{
public:
    WtGameModeAbc() :
      WtGameModeIf( "ABC?E" ),
      m_working_letters(""),
      m_active_row(0),
      m_next(' ')
    {
    }
    ~WtGameModeAbc()
    {
    }

    /**************************
     * 
     *************************/
    virtual void init_game( WtBoard& board, WtPlayer& player ) 
    {
        (void)player; // currently unused

        m_active_row = 0;
        setup_sequences();
        show_sequence( board );

        m_next = WtRandom::get_random_letter_of_word( m_working_letters );
        remove_letter( m_working_letters, m_next );

    }

    /**************************
     *
     *************************/
    virtual std::string get_title()
    {
        return "@dark/label_mode_alphabet.bmp";
    }

    /**************************
     *
     *************************/
    std::string get_name()
    {
        return WtL10n_tr("Alphabet Challenge");
    }


    /**************************
     *
     *************************/
    virtual void eval_board( WtBoard& board, WtPlayer& player, WtGameModeState& gs )
    {
        if ( ( m_working_letters.empty() ) && ( m_next == WtBoard::empty_cell ) )
        {
            bool correct = true;
            for( uint8_t c=0; c < WtBoard::col_count; c++ )
            {
                char cell = board.get_cell( m_active_row, c );
                if ( cell == WtBoard::empty_cell )
                {
                    board.set_cell( m_active_row, c, ' ' );
                }
                if ( m_current_sequence_control[c] != cell )
                {
                    correct = false;
                }
            }

            if ( correct )
            {
                
                {
                    WtGridAnimation* blink = new WtGridAnimation("wordsolved.ogg");
                    WtGridAnimation::GridAnimationStep step( WtGridAnimation::fromGridText( WtGridAnimation::GridText( WtBoard::row_count-m_active_row,
                                                                                        0,
                                                                                        true,
                                                                                        m_current_sequence_control, 
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
                
                
                board.collapse_above( m_active_row );
            }
            else
            {
                m_active_row++;
            }

            setup_sequences();
            show_sequence( board );
            m_next = WtRandom::get_random_letter_of_word( m_working_letters );
            remove_letter( m_working_letters, m_next );
            stone_added();
            stone_added();
        }
    }

    /**************************
     *
     *************************/
    virtual char next_letter()
    {
        char next = m_next;
        if ( ! m_working_letters.empty() )
        {
            m_next = WtRandom::get_random_letter_of_word( m_working_letters );
            remove_letter( m_working_letters, m_next );
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
        return std::string(1, m_next);
    }

    /**************************
     *
     *************************/
    virtual std::string get_hint()
    {
        return WtL10n::translate( WtL10n_tr("Try to place the falling letters at the correct position of the given sequence.") );
    }

    /**************************
     *
     *************************/
    virtual std::string get_help()
    { 
        return WtL10n::translate( WtL10n_tr("Try to place the falling letters at the correct position of the given sequence.") );
    }

private:
    /**************************
     *
     *************************/
    void setup_sequences()
    {
        generate_sequence(m_current_sequence_control);
        m_working_letters = prepare_sequence( m_current_sequence_control, 
                                              get_difficulty(), 
                                              m_current_sequence );
    }

    /**************************
     *
     *************************/
    void show_sequence( WtBoard& board )
    {
        for( uint8_t c=0; c < WtBoard::col_count; c++ )
        {
            board.set_cell( m_active_row, c, m_current_sequence[c] );
        }
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
    std::string prepare_sequence( WtBoard::RowSequence& in_seq, 
                                  wt_difficulty diff, 
                                  WtBoard::RowSequence& out_seq )
    {
        std::string removed;
        std::vector<size_t> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        uint8_t lettersToRemove = 0;
        switch( diff )
        {
            case wt_difficulty_EASY:   lettersToRemove = 2; break;
            case wt_difficulty_MEDIUM: lettersToRemove = 4; break;
            case wt_difficulty_HARD:   lettersToRemove = 8; break;
            default: break;
        }

        for( size_t idx = 0; idx < WtBoard::col_count; idx++ )
        {
            out_seq[idx] = in_seq[idx];
        }

        for( uint8_t removedCount = 0; removedCount < lettersToRemove; removedCount++ )
        {
            size_t r_idx = WtRandom::get_random_from_sequence<size_t>( indices );

            removed.push_back( out_seq[r_idx] );
            out_seq[r_idx] = WtBoard::empty_cell;

            indices.erase( find(indices.begin(), indices.end(), r_idx ) );
        }
        return removed;
    }

    /**************************
     *
     *************************/
    void generate_sequence( WtBoard::RowSequence& seq )
    {
        uint8_t starting_idx = WtRandom::roll_dice(26-WtBoard::col_count);
        for( size_t idx = 0; idx < WtBoard::col_count; idx++ )
        {
            seq[idx] = ( 'A'+(idx+starting_idx) );
        }
    }

private:
    WtBoard::RowSequence m_current_sequence;
    WtBoard::RowSequence m_current_sequence_control;
    std::string          m_working_letters;
    uint8_t              m_active_row;
    char                 m_next;
};



#endif /* _WT_GAME_MODE_ABC_H_ */
