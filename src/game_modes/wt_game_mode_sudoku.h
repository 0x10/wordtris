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
#include <set>
class WtGameModeSudoku : public WtGameModeIf
{
public:
    WtGameModeSudoku() :
        WtGameModeIf( "Sudoku" )
    {
    }
    ~WtGameModeSudoku()
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
        return WtL10n::translate( WtL10n_tr("Sudoku") );
    }

    /**************************
     * 
     *************************/
    void init_game( WtBoard& board, WtPlayer& )
    {
        board.set_cell( 1, 1, '1' );
    }

    /**************************
     *
     *************************/
    virtual void eval_board( WtBoard& board, WtPlayer& , WtGameModeState& gs )
    {
       gs.game_over = is_valid_config( board ); 
    }

    /**************************
     *
     *************************/
    char next_letter()
    {
        return ' ';
    }

    /**************************
     *
     *************************/
    virtual std::string letter_after_next()
    {
        return " ";
    }


    /**************************
     *
     *************************/
    std::string get_hint()
    {
        return WtL10n::translate( WtL10n_tr("Try to solve the sudoku.") );
    }

private:
    WtGameModeSudoku( const WtGameModeSudoku& ); 
    WtGameModeSudoku & operator = (const WtGameModeSudoku &);
#if 0
    /**************************
     * Checks whether there is any duplicate  
     * in current row or not 
     *************************/
    bool not_in_row(WtBoard& board, int row) 
    { 
        // Set to store characters seen so far. 
        size_t sum = 0;
        for (uint8_t c = 0; c < 9; c++) { 
            char cell = board.get_cell(row,c);
      
            // If it is not an empty cell, insert value 
            // at the current cell in the set 
            if (cell != WtBoard::empty_cell) 
                sum += (cell - 0x30);
        } 
        return (sum == 45);
    } 
  
    /**************************
     * Checks whether there is any duplicate 
     * in current column or not. 
     *************************/
    bool not_in_col(WtBoard& board, int col) 
    { 
        size_t sum = 0;
      
        for (uint8_t r = 0; r < 9; r++) { 
            char cell = board.get_cell(r,col);
      
            // If it is not an empty cell, 
            // insert value at the current cell in the set 
            if (cell != WtBoard::empty_cell) 
                sum += (cell - 0x30);
        } 
        return (sum == 45);
    }

    /**************************
     * Checks whether there is any duplicate 
     * in current 3x3 box or not. 
     *************************/
    bool not_in_box(WtBoard& board, int startRow, int startCol) 
    { 
        size_t sum = 0;
      
        for (uint8_t row = 0; row < 3; row++) { 
            for (uint8_t col = 0; col < 3; col++) { 
                char curr = board.get_cell(row + startRow, col + startCol);
      
                // If it is not an empty cell, 
                // insert value at current cell in set 
                if (curr != WtBoard::empty_cell) 
                    sum += (cell - 0x30);
            } 
        } 
        return (sum == 45);
    } 
#endif
    /**************************
     * Checks whether there is any duplicate  
     * in current row or not 
     *************************/
    bool not_in_row(WtBoard& board, uint8_t row) 
    { 
        // Set to store characters seen so far. 
        std::set<char> st; 
      
        for (uint8_t c = 0; c < 9; c++) { 
            char cell = board.get_cell(row,c);
            // If already encountered before, return false 
            if (st.find(cell) != st.end()) 
                return false; 
      
            // If it is not an empty cell, insert value 
            // at the current cell in the set 
            if (cell != WtBoard::empty_cell) 
                st.insert(cell); 
        } 
        return true; 
    } 
  
    /**************************
     * Checks whether there is any duplicate 
     * in current column or not. 
     *************************/
    bool not_in_col(WtBoard& board, uint8_t col) 
    { 
        std::set<char> st; 
      
        for (uint8_t r = 0; r < 9; r++) { 
            char cell = board.get_cell(r,col);
      
            // If already encountered before, return false 
            if (st.find(cell) != st.end()) 
                return false; 
      
            // If it is not an empty cell, 
            // insert value at the current cell in the set 
            if (cell != WtBoard::empty_cell) 
                st.insert(cell); 
        } 
        return true; 
    }

    /**************************
     * Checks whether there is any duplicate 
     * in current 3x3 box or not. 
     *************************/
    bool not_in_box(WtBoard& board, uint8_t startRow, uint8_t startCol) 
    { 
        std::set<char> st; 
      
        for (uint8_t row = 0; row < 3; row++) { 
            for (uint8_t col = 0; col < 3; col++) { 
                char curr = board.get_cell(row + startRow, col + startCol);
      
                // If already encountered before, return false 
                if (st.find(curr) != st.end()) 
                    return false; 
      
                // If it is not an empty cell, 
                // insert value at current cell in set 
                if (curr != WtBoard::empty_cell) 
                    st.insert(curr); 
            } 
        } 
        return true; 
    } 
    /**************************
     * Checks whether current row and current column and 
     * current 3x3 box is valid or not 
     *************************/ 
    bool is_valid_config(WtBoard& board) 
    {
        bool is_valid = true;

        not_in_row( board, 0 );
        not_in_row( board, 1 );
        not_in_row( board, 2 );
        not_in_row( board, 3 );
        not_in_row( board, 4 );
        not_in_row( board, 5 );
        not_in_row( board, 6 );
        not_in_row( board, 7 );
        not_in_row( board, 8 );

        not_in_col( board, 0 );
        not_in_col( board, 1 );
        not_in_col( board, 2 );
        not_in_col( board, 3 );
        not_in_col( board, 4 );
        not_in_col( board, 5 );
        not_in_col( board, 6 );
        not_in_col( board, 7 );
        not_in_col( board, 8 );

        not_in_box( board, 0, 0 );not_in_box( board, 0, 3 );not_in_box( board, 0, 6 );
        not_in_box( board, 3, 0 );not_in_box( board, 3, 3 );not_in_box( board, 3, 6 );
        not_in_box( board, 6, 0 );not_in_box( board, 6, 3 );not_in_box( board, 6, 6 );
        return is_valid;
    } 
};

#endif /* _WT_GAME_MODE_GUESSING_H_ */
