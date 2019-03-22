/*!*****************************************************************************
 * @file wt_active_letter.h
 * @brief control class for active letter token.
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
#ifndef _WT_LETTER_H_
#define _WT_LETTER_H_

#include "global.h"
#include "wt_board.h"

class WtLetter
{
public:
    WtLetter() :
        m_col( 5 ),
        m_row( WtBoard::row_count - 1 ),
        m_value( '\0' )
    {}
    ~WtLetter() {}
private: // no copy allowed
    WtLetter( const WtLetter& ); 
    WtLetter & operator = (const WtLetter &);

// api defintion
public:
    /**************************
     *
     *************************/
    void init( char init_letter )
    {
        m_col = 5;
        m_row = (WtBoard::row_count - 1);
        m_value = init_letter;
    }

    /**************************
     *
     *************************/
    void get_next( char next_letter )
    {
        m_value = next_letter;
        m_col = 5;
        m_row = (WtBoard::row_count - 1);
    }

    /**************************
     *
     *************************/
    void move_left()
    {
        if ( m_col > 0 )
            m_col --;
    }

    /**************************
     *
     *************************/
    void move_right()
    {
        if ( m_col < ( WtBoard::col_count - 1 ) )
            m_col ++;
    }

    /**************************
     *
     *************************/
    void single_drop()
    {
        if ( m_row > 0 )
            m_row --;
    }

    /**************************
     *
     *************************/
    void drop_at( uint8_t r )
    {
        if ( r < WtBoard::row_count )
            m_row = r;
    }

    /**************************
     *
     *************************/
    uint8_t current_column() const { return m_col; };
    uint8_t current_row()    const { return m_row; };
    char    current_value()  const { return m_value; };
private:
    uint8_t m_col;
    uint8_t m_row;
    char    m_value;

};


#endif /* _WT_LETTER_H */
