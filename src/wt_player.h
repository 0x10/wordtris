/*!*****************************************************************************
 * @file wt_player.h
 * @brief class used to store/access/handle player game logic
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
#ifndef _WT_PLAYER_H_
#define _WT_PLAYER_H_

#include "wt_board.h"

#define PTS_P_WORD           (1000)

class WtPlayer
{
public:
    WtPlayer() :
        m_points( 0 ),
        m_words( 0 ),
        m_level( 1 )
    {}
    ~WtPlayer() {}
private: // no copy allowed
    WtPlayer( const WtPlayer& ); 
    WtPlayer & operator = (const WtPlayer &);

// api defintion
public:
    /**************************
     *
     *************************/
    void word_solved( size_t length=1, uint32_t base_points=PTS_P_WORD )
    {
        m_words ++;

        m_points += (static_cast<uint32_t>(length * base_points) * static_cast<uint32_t>(m_level));

        uint8_t new_level = 1 + static_cast<uint8_t>(m_words / 10);
        if ( new_level > m_level )
            m_level = new_level;
    }

    /**************************
     *
     *************************/
    uint8_t words_since_levelup() const
    {
        uint8_t solved_words = get_solved_word_count();
        uint8_t next_level_word_count = (( solved_words / 10 ) + 1 ) * 10;
        return 10 - ( next_level_word_count - solved_words );
    }

    /**************************
     *
     *************************/
    void row_cleared( uint32_t new_points )
    {
        m_words ++;

        m_points += new_points * m_level;

        m_level = 1 + static_cast<uint8_t>((m_words - 1) / 10);
        uint8_t new_level = 1 + static_cast<uint8_t>(m_words / 10);
        if ( new_level > m_level )
            m_level = new_level;
    }

    /**************************
     *
     *************************/
    void letter_dropped( uint8_t row_diff )
    {
        if ( row_diff > 0 )
        {
            uint32_t row_pts = static_cast<uint32_t>(row_diff);
            m_points += ( row_pts * ( 2 * static_cast<uint32_t>(m_level) ) );
        }
    }

    /**************************
     *
     *************************/
    void reset()
    {
        m_points = 0;
        m_words = 0;
        m_level = 1;
    }

    /**************************
     *
     *************************/
    void set_level_offset( uint8_t level )
    {
        m_level = level;
    }

    /**************************
     *
     *************************/
    uint32_t get_points() const            { return m_points; };
    uint16_t get_solved_word_count() const { return m_words;  };
    uint32_t get_current_level() const     { return m_level;  };

private:
    uint32_t m_points;
    uint16_t m_words;
    uint8_t  m_level;

};


#endif /* _WT_PLAYER_H */
