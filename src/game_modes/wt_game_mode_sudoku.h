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
    virtual void eval_board( WtBoard& board, WtPlayer& player, WtGameModeState& gs )
    {
             
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


    /**************************
     *
     *************************/
    void get_next_word()
    {

    }

    /**************************
     *
     *************************/
    void remove_letter( std::string& word, char letter )
    {

    }

};

#endif /* _WT_GAME_MODE_GUESSING_H_ */
