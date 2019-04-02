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
#include <locale>         // std::locale, std::tolower
#include <algorithm>
#include <sstream>
#include <iterator>

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}


class WtGameModeWordtris : public WtGameModeIf
{
private:
    static const size_t min_word_length = 3;
public:
    WtGameModeWordtris() :
        WtGameModeIf( "WordtrisClassic" ),
        m_letters( "ETAOINSRHDLUCMFYWGPBVKXQJZ" ),
        m_wordlist()
    {
        m_wordlist.load_from_list( WtWordList::get_wordlist_by_name("5k list"), WtWordList::eToUpper );
        std::cout << "words = " << m_wordlist.size() << std::endl;
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
            std::vector<std::string> sequences = split( std::string(row_str) );
            for( size_t s_idx = 0; s_idx < sequences.size(); s_idx++ )
            {
                if ( sequences[s_idx].length() >= min_word_length )
                {
                    std::string word = contains_word( sequences[s_idx] );
                    if ( !word.empty() ) 
                    {
                        player.word_solved( word.length() );
                        erase_from_row( r_idx, row_str, word, board );
                        
                        break;
                    }
                }
            }
        }

        for ( uint8_t c_idx = 0; c_idx < WtBoard::col_count; c_idx++ )
        {
            std::string col_str = board.get_col_string( c_idx );
            std::string trimmed = col_str;
            trim( trimmed );

            if ( trimmed.length() >= min_word_length )
            {
                std::string word = contains_word( trimmed );
                if ( !word.empty() ) 
                {
                    player.word_solved( word.length() );
                    erase_from_col( c_idx, col_str, word, board );

                    break;
                }
            }
        }


        return game_continue;
    }

    /**************************
     *
     *************************/
    virtual char next_letter()
    {
                                                   // E  T A O I N S R H D L U C M F Y W G P B V K X Q J Z,?,*
        std::discrete_distribution<int> distribution {12,9,8,8,7,7,6,6,6,4,4,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,5,5 };
        return WtRandom::get_random_letter_of_weight_seq( std::string(m_letters).append("?*"),
                                                          distribution ); 
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
        std::string col_str = board.get_col_string( c_idx );
        if ( col_str.find_first_not_of( " " ) != std::string::npos )
        {
            std::string lower = col_str.substr( WtBoard::row_count/2 );
            std::string upper = col_str.substr( 0, WtBoard::row_count/2 );

            std::cout << "col_str before = \"" << col_str << "\""<< std::endl;
            std::cout << "upper before = \""<< upper << "\""<< std::endl;
            std::cout << "lower before = \""<< lower << "\""<< std::endl;

            upper.erase(std::remove(upper.begin(), upper.end(), ' '), upper.end());
            lower.erase(std::remove(lower.begin(), lower.end(), ' '), lower.end());

            std::cout << "upper after = \""<< upper << "\""<< std::endl;
            std::cout << "lower after = \""<< lower << "\""<< std::endl;
           
            std::string new_col_str;
            if ( ! upper.empty() )
            {
                std::string upper_col_str(WtBoard::row_count/2-upper.length(), ' ' );
                upper_col_str.append( upper );
                std::string lower_col_str(WtBoard::row_count/2-lower.length(), ' ' );
                lower.append( lower_col_str );
                new_col_str = upper_col_str;           
                new_col_str.append( lower );
            }
            else
            {
                std::string upper_col_str(WtBoard::row_count/2-1, ' ' );
                std::string lower_col_str(WtBoard::row_count/2+1-lower.length(), ' ' );
                lower.append( lower_col_str );
                new_col_str = upper_col_str;           
                new_col_str.append( lower );
            }

            std::cout << "new_col_str after = \"" << new_col_str << "\""<< std::endl;

            for ( uint8_t r_idx = 0; r_idx < WtBoard::row_count; r_idx++ )
            {
                char cell = new_col_str[ (WtBoard::row_count-1) - r_idx ];
                if ( cell == ' ' )
                {
                    if ( r_idx < WtBoard::row_count / 2 )
                    {
                        cell = ' ';
                    }
                    else
                    {
                        cell = WtBoard::empty_cell;
                    }
                }

                board.set_cell( r_idx, c_idx, cell );
            }

            std::cout << "col_str after = \"" << board.get_col_string( c_idx ) << "\""<< std::endl;
        }
    }


    /**************************
     *
     *************************/
    void erase_from_row( uint8_t r_idx, std::string row_str, std::string word, WtBoard& board )
    {
        //1. locate beginning
        size_t pos = row_str.find( word );
        std::cout << "erase '" << word << "' from '" << row_str << "'" <<std::endl;
        if ( pos != std::string::npos )
        {
            char replace_char = WtBoard::empty_cell;
            if ( r_idx < WtBoard::row_count/2 )
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
            std::cout << "colstr = \"" << col_str << "\"; word found at = "<<pos<<"w len = "<<word.length()<<std::endl;
            pos = (WtBoard::row_count - pos) - 1;
            // iterate over rows
            for( size_t w_idx = 0; w_idx < word.length(); w_idx++ )
            {
                uint8_t r_idx = pos - w_idx;

                //2. if row <= row_count/2 replace with ' '
                //   else replace with empty_cell
                char replace_char = WtBoard::empty_cell;
                if ( r_idx < WtBoard::row_count/2 )
                    replace_char = ' ';

                board.set_cell( r_idx, c_idx, replace_char );
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

        if ( found_words.size() > 0 )
        {
            struct compare {
                bool operator()(const std::string& first, const std::string& second) {
                    return first.size() > second.size();
                }
            } c;

            std::sort(found_words.begin(), found_words.end(), c);

            for( size_t i = 0; i < found_words.size(); i++ )
                std::cout << "(" << i << ") " << found_words[i] << std::endl;

            result = found_words[0];
        }

        return result;
    }


    /**************************
     *
     *************************/
    std::vector<std::string> split( std::string board_str )
    {
        std::istringstream iss(board_str);
        std::vector<std::string> result_list(std::istream_iterator<std::string>{iss},
                                         std::istream_iterator<std::string>());
        return result_list;
    }

private:
   const std::string m_letters;
   WtWordList        m_wordlist;
};

#endif /* _WT_GAME_MODE_WORDTRIS_H_ */
