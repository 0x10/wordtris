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

#define ROW_COUNT            (20)
#define COL_COUNT            (10)

class WtBoard
{
public:
    static const uint8_t row_count = ROW_COUNT;
    static const uint8_t col_count = COL_COUNT;
    typedef char RowSequence[WtBoard::col_count];

    WtBoard()
    {

    }
    ~WtBoard() {}
private:
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
        for( uint8_t r = 0; r<ROW_COUNT; r++ )
            for( uint8_t c = 0; c<COL_COUNT; c++ )
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
    const RowSequence& get_row_sequence( uint8_t r ) const
    {
        if ( r >= ROW_COUNT )
            return m_board[0];

        return m_board[r];
    }

    /**************************
     *
     *************************/
    std::string get_row_string( uint8_t r ) const
    {
        if ( r >= ROW_COUNT )
            return "";

        std::string row("");

        for( uint8_t c_idx = 0; c_idx < WtBoard::col_count; c_idx++ )
        {
            row.push_back( ( m_board[r][c_idx] == WtBoard::empty_cell ? 
                                    ' ' :
                                    m_board[r][c_idx] ) );
        }

        return row;

//        return std::string( get_row_sequence( r ) );
    }

    /**************************
     *
     *************************/
    std::string get_col_string( uint8_t c ) const
    {
        if ( c >= COL_COUNT )
            return "";

        std::string col("");

        for( uint8_t r_idx = 0; r_idx < WtBoard::row_count; r_idx++ )
        {
            //col.push_back( m_board[r_idx][c] );
            col.insert( col.begin(), ( m_board[r_idx][c] == WtBoard::empty_cell ? 
                                            ' ' : m_board[r_idx][c] ) );
        }

        return col;
    }

    /**************************
     *
     *************************/
    void collapse_above( uint8_t r, uint8_t c, char carry=WtBoard::empty_cell )
    {
        /* drop remaining stones afterwards */
        for( uint8_t row=r; row < WtBoard::row_count-1; row++ )
        {
            set_cell( row, c,
                      get_cell( row+1, c ) );
        }
        set_cell( WtBoard::row_count-1, c, carry );
    }

    /**************************
     *
     *************************/
    void collapse_below( uint8_t r, uint8_t c, char carry=WtBoard::empty_cell )
    {
        /* drop remaining stones afterwards */
        for( uint8_t row=r; row > 0; row-- )
        {
            set_cell( row, c,
                      get_cell( row-1, c ) );
        }
        set_cell( 0, c, carry );
    }

    /**************************
     *
     *************************/
    void collapse_above( uint8_t r, char carry=WtBoard::empty_cell )
    {
        /* drop remaining stones afterwards */
        for( uint8_t row=r; row < WtBoard::row_count-1; row++ )
        {
            for( uint8_t c=0; c < WtBoard::col_count; c++ )
            {
                set_cell( row, c,
                          get_cell( row+1, c ) );
            }
        }
        for( uint8_t c=0; c < WtBoard::col_count; c++ )
        {
           set_cell( WtBoard::row_count-1, c, carry );
        }
    }

private:
    char m_board[ROW_COUNT][COL_COUNT]; 
};


#endif /* _WT_BOARD_H */
