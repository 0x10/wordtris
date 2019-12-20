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


class WtLetter
{
public:
    WtLetter() :
        m_col( 0 ),
        m_row( 0 ),
        m_value( ' ' )
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
        m_col = 0;
        m_row = 0;
        m_value = init_letter;
    }

    /**************************
     *
     *************************/
    void get_next( char next_letter )
    {
        m_value = next_letter;
        m_col = 0;
        m_row = 0;
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
    void move_right( const uint8_t max )
    {
        if ( m_col < ( max - 1 ) )
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
    uint8_t current_column() const { return m_col; };
    uint8_t current_row()    const { return m_row; };
    char    current_value()  const { return m_value; };

    void set_pos( uint8_t r, uint8_t c )
    {
        m_row = r;
        m_col = c;
    }
private:
    uint8_t m_col;
    uint8_t m_row;
    char    m_value;

};


#endif /* _WT_LETTER_H */
