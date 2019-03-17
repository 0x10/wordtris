/*!*****************************************************************************
 * @file wt_board.h
 * @brief game board access/control class
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
#ifndef _WT_BOARD_H_
#define _WT_BOARD_H_

#include <array>

#include "global.h"

#define ROW_COUNT            (20)
#define COL_COUNT            (10)

#define BOARD  WtBoard::instance()

class WtBoard
{
// singleton definition
public:
    static const uint8_t row_count = ROW_COUNT;
    static const uint8_t col_count = COL_COUNT;
    typedef char RowSequence[WtBoard::col_count];

    static WtBoard& instance()
    {
        static WtBoard _instance;
        return _instance;
    }
    ~WtBoard() {}
private:
    WtBoard()
    {

    }
    WtBoard( const WtBoard& ); 
    WtBoard & operator = (const WtBoard &);

// api defintion
public:
    static const char empty_cell = '\0';

    /**************************
     *
     *************************/
    void init()
    {
        for( size_t r = 0; r<ROW_COUNT; r++ )
            for( size_t c = 0; c<COL_COUNT; c++ )
                set_cell( r, c, empty_cell );
    }
    

    /**************************
     *
     *************************/
    bool cell_occupied( uint8_t r,
                        uint8_t c )
    {
        return ( get_cell( r, c ) != empty_cell );
    }

    /**************************
     *
     *************************/
    void set_cell( uint8_t r,
                   uint8_t c,
                   char    val )
    {
        m_board[r][c] = val;
    }

    /**************************
     *
     *************************/
    char get_cell( uint8_t r, uint8_t c ) const
    {
        if ( ( r >= ROW_COUNT ) || ( c >= COL_COUNT ) )
            return ' ';

        return m_board[r][c];
    }

    /**************************
     *
     *************************/
    void collapse_above( uint8_t r )
    {
        /* drop remaining stones afterwards */
        for( uint8_t row=r; row < WtBoard::row_count-1; row++ )
            for( uint8_t c=0; c < WtBoard::col_count; c++ )
            {
                set_cell( row, c,
                          get_cell( row+1, c ) );
            }       
    }

private:
    char m_board[ROW_COUNT][COL_COUNT]; 
};


#endif /* _WT_BOARD_H */
