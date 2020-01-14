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

#include "wt_storage.h"

class WtBoard
{
private:
    static constexpr uint8_t row_cnt = 9;
    static constexpr uint8_t col_cnt = row_cnt;
    struct HistoryEntry
    {
        uint8_t r;
        uint8_t c;
        char value;
    };
public:
    static const char empty_cell = '\0';
    typedef char RowSequence[WtBoard::col_cnt];

    WtBoard() :
        m_board(),
        m_row_count(WtBoard::row_cnt),
        m_col_count(WtBoard::col_cnt),
        m_history(),
        m_board_notes()
    {
    }
    ~WtBoard() {}
private:
    WtBoard( const WtBoard& ); 
    WtBoard & operator = (const WtBoard &);

// api defintion
public:
    /**************************
     *
     *************************/
    void init()
    {
        for( uint8_t r = 0; r<row_count(); r++ )
            for( uint8_t c = 0; c<col_count(); c++ )
            {
                set_cell( r, c, empty_cell );
                clear_notes( r, c );
            }
        clear_history();
    }

    /**************************
     *
     *************************/
    bool cell_occupied( uint8_t r,
                        uint8_t c ) const
    {
        return ( get_cell( r, c ) != empty_cell );
    }

    /**************************
     *
     *************************/
    uint8_t row_count() const
    {
        return STORAGE.get_settings().gridsize;// m_row_count;
    }

    /**************************
     *
     *************************/
    uint8_t col_count() const
    {
        return STORAGE.get_settings().gridsize;//m_col_count;
    }

    /**************************
     *
     *************************/
    bool is_full() const
    {
        bool full = true;

        for( uint8_t r = 0; r<row_count(); r++ )
            for( uint8_t c = 0; c<col_count(); c++ )
                full = (cell_occupied( r, c ) && full);

        return full;
    }

    /**************************
     *
     *************************/
    void set_cell( uint8_t r,
                   uint8_t c,
                   char    val )
    {
        if ( value_valid( val ) && index_valid( r, c ) )
        {
            HistoryEntry action{r, c, m_board[r][c]};
            m_board[r][c] = val;
            m_history.push_back( action );
        }
    }

    /**************************
     *
     *************************/
    void undo()
    {
        if ( m_history.size() > 0 )
        {
            HistoryEntry action = m_history.back();
            m_history.pop_back();
            m_board[action.r][action.c] = action.value;
        }
    }
    /**************************
     *
     *************************/
    bool undo_available() const
    {
        return m_history.size() > 0;
    }
    /**************************
     *
     *************************/
    char get_cell( uint8_t r, uint8_t c ) const
    {
        if ( ! index_valid( r, c ) )
            return ' ';

        return m_board[r][c];
    }

    /**************************
     *
     *************************/
    const RowSequence& get_row_sequence( uint8_t r ) const
    {
        if ( ! index_valid( r, 0 ) )
            return m_board[0];

        return m_board[r];
    }

    /**************************
     *
     *************************/
    std::string get_row_string( uint8_t r ) const
    {
        if ( ! index_valid( r, 0 ) )
            return "";

        std::string row("");

        for( uint8_t c_idx = 0; c_idx < col_count(); c_idx++ )
        {
            row.push_back( ( m_board[r][c_idx] == WtBoard::empty_cell ? 
                                    ' ' :
                                    m_board[r][c_idx] ) );
        }

        return row;
    }

    /**************************
     *
     *************************/
    std::string get_col_string( uint8_t c ) const
    {
        if ( ! index_valid( 0, c ) )
            return "";

        std::string col("");

        for( uint8_t r_idx = 0; r_idx < row_count(); r_idx++ )
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
    bool value_valid( char val ) const
    {
        return ( val == WtBoard::empty_cell || (( val >= 0x30 ) && ( val <= 0x39 )));
    }

    /**************************
     *
     *************************/   
    bool index_valid( uint8_t r, uint8_t c ) const
    {
        return ( ( r < row_count() ) && ( c < col_count() ) );
    }
    /**************************
     *
     *************************/   
    void clear_history()
    {
        m_history.clear();
    }

    /**************************
     *
     *************************/   
    void clear_notes( uint8_t r, uint8_t c)
    {
        for ( bool &notes : m_board_notes[r][c] )
        {
            notes = false;
        }
    }

    /**************************
     *
     *************************/   
    void set_note( uint8_t r, uint8_t c, uint8_t value )
    {
        uint8_t local_value = value - 1;
        if ( local_value < m_row_count )
            m_board_notes[r][c][local_value] = true;
    }

    /**************************
     *
     *************************/
    bool is_note_set( uint8_t r, uint8_t c, uint8_t value ) const
    {
        uint8_t local_value = value - 1;
        if ( local_value < m_row_count )
            return m_board_notes[r][c][local_value] == true;
        else
            return false;
    }
private:
    char m_board[WtBoard::row_cnt][WtBoard::col_cnt]; 
    uint8_t m_row_count;
    uint8_t m_col_count;
    std::vector<HistoryEntry> m_history;
    bool m_board_notes[WtBoard::row_cnt][WtBoard::col_cnt][WtBoard::row_cnt];
};


#endif /* _WT_BOARD_H */
