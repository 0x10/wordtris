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
#include "wt_wordlist.h"

class WtGameModeWordtris : public WtGameModeIf
{
public:
    WtGameModeWordtris() :
        WtGameModeIf( "WordtrisClassic" ),
        m_letters( "ETAOINSRHDLUCMFYWGPBVKXQJZ" )
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
    virtual void init_game( WtBoard& board, WtPlayer& player )
    {
        (void)player; //currently not used

        for( size_t r_idx = 0; r_idx < WtBoard::row_count/2; r_idx++ )
            for( size_t c_idx = 0; c_idx < WtBoard::col_count; c_idx++ )
            {
                board.set_cell( r_idx, c_idx, ' ' );
            }
    }

    /**************************
     * return false if game over
     *************************/
    virtual bool eval_board( WtBoard& board, WtPlayer& player )
    {
        bool game_continue = true;
        
        for ( uint8_t r_idx = 0; r_idx < WtBoard::row_count; r_idx++ )
        {
            std::string row_str = board.get_row_string( r_idx );

            std::string word = contains_word( row_str );
            if ( !word.empty() ) 
            {
                player.word_solved( word.length() );
                erase_from_row( r_idx, row_str, word, board );
                
                break;
            }
        }

        for ( uint8_t c_idx = 0; c_idx < WtBoard::col_count; c_idx++ )
        {
            std::string col_str = board.get_col_string( c_idx );

            std::string word = contains_word( col_str );
            if ( !word.empty() ) 
            {
                player.word_solved( word.length() );
                erase_from_col( c_idx, col_str, word, board );

                break;
            }
        }


        return game_continue;
    }

    /**************************
     *
     *************************/
    virtual char next_letter()
    {
        return WtRandom::get_random_letter_of_weight_seq( std::string(m_letters).append("???***") ); 
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
        if ( value == '*' )
        {
            // a bomb has been dropped!
            if ( 
                 ( ( board.get_cell( row-1, col ) != WtBoard::empty_cell )
                   &&
                   ( board.get_cell( row-1, col ) != ' ' )
                 )
               )
            {
                if ( row <= (WtBoard::row_count/2+1) )
                {
                    for ( uint8_t idx = row-1; idx > 0; idx-- )
                    {
                        char next = board.get_cell( idx-1, col );
                        if ( ( next == ' ' ) && ( idx == row-1 ) )
                            next = WtBoard::empty_cell;

                        board.set_cell( idx, col, next );

                        if ( ( next == ' ' ) || ( next == WtBoard::empty_cell ) )
                            break;
                    }
                    board.set_cell( 0, col, ' ' );
                }
                else
                {
                    board.set_cell( row-1, col, WtBoard::empty_cell );
                }
            }
        }
        else
        {
            if ( value == '?' )
            {
                value = WtRandom::get_random_letter_of_word( m_letters );
            }

            // if we stack up, we push the other rows downward until they reach
            // the end of the board and then we stack up
            if ( 
                 ( ( board.get_cell( row-1, col ) != WtBoard::empty_cell )
                   &&
                   ( board.get_cell( row-1, col ) != ' ' )
                 )
                 &&
                 ( board.get_cell( 0, col ) == ' ' )
               )
            {
                for ( uint8_t idx = 0; idx < row-1; idx++ )
                {
                    board.set_cell( idx, col, board.get_cell( idx+1, col ) );
                }
                board.set_cell( row - 1, col, value );
            }
            else
            {
                board.set_cell( row, col, value );
            }
        }
    }


private:
    WtGameModeWordtris( const WtGameModeWordtris& ); 
    WtGameModeWordtris & operator = (const WtGameModeWordtris &);

    /**************************
     *
     *************************/
    void column_gravity( uint8_t c_idx, WtBoard& board )
    {
        //   two iterations: (1) from middle to ceil and (2) from middle to floor
        //                   (1) pull down towards middle
        //                   (2) push up towards middle
        uint8_t r_idx = WtBoard::row_count/2+1;
        while ( board.get_cell( r_idx, c_idx ) == WtBoard::empty_cell )
        {
            // pull necessary
            board.collapse_above( r_idx, c_idx );
        }
        
        r_idx = WtBoard::row_count/2;
        if ( board.get_cell( r_idx, c_idx ) == ' ' )
        {
            // push necessary
        }
    }


    /**************************
     *
     *************************/
    void erase_from_row( uint8_t r_idx, std::string row_str, std::string word, WtBoard& board )
    {
        //1. locate beginning
        size_t pos = row_str.find( word );
        if ( pos != std::string::npos )
        {
            char replace_char = WtBoard::empty_cell;
            if ( r_idx <= WtBoard::row_count/2 )
                replace_char = ' ';
            
            //2. replace acc.
            for( size_t c_idx = pos; c_idx < (pos + word.length()); c_idx++ )
            {
                board.set_cell( r_idx, (uint8_t)c_idx, replace_char );

                //3. push up or pull down if necessary
                column_gravity( c_idx, board );            
            }
        }
    }

    /**************************
     *
     *************************/
    void erase_from_col( uint8_t c_idx, std::string col_str, std::string word, WtBoard& board )
    {
        //1. locate beginning
        size_t pos = col_str.find( word );
        if ( pos != std::string::npos )
        {
            // iterate over rows
            for( ssize_t r_idx = pos; r_idx >= 0; r_idx-- )
            {
                //2. if row <= row_count/2 replace with ' '
                //   else replace with empty_cell
                char replace_char = WtBoard::empty_cell;
                if ( r_idx <= WtBoard::row_count/2 )
                    replace_char = ' ';

                board.set_cell( (uint8_t)r_idx, c_idx, replace_char );
            }

            //3. push up or pull down if necessary
            column_gravity( c_idx, board );            
        }
    }

    /**************************
     *
     *************************/
    std::string contains_word( std::string sequence )
    {
        std::string result = "";

        // use dea with preprocessed contains logic to eval word

        // preprocessor of word list need to create a dea for each word
        // and the contains logic needs to take a list argment 
        // and process as logical OR dea which means parallel symbol
        // processing
        // dea list preprocessor sorts string list by length
        // and gets the max length as parameter for more efficient
        // searching
        // something like this:
        // WtWordList::search_for_word( sequence );
        // length is taken by sequence.length() within search
        // also language selection
        std::vector<std::string> found_words = m_wordlist.get_contained_words( sequence );

        return result;
    }

private:
   const std::string m_letters;
   WtWordList        m_wordlist;
};

#endif /* _WT_GAME_MODE_WORDTRIS_H_ */
