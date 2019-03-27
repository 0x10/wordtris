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
    void word_solved( size_t length=1 )
    {
        m_words ++;
        m_points += length * (m_level * PTS_P_WORD);
        m_level = 1 + (uint8_t)((m_words - 1) / 10);       
    }

    /**************************
     *
     *************************/
    void row_cleared( uint32_t new_points )
    {
        m_words ++;
        m_points += new_points * m_level;
        m_level = 1 + (uint8_t)((m_words - 1) / 10);       
    }

    /**************************
     *
     *************************/
    void letter_dropped( uint8_t row_diff )
    {
        m_points += ( 21 + (3 * m_level) - row_diff );
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
    uint8_t  get_current_level() const     { return m_level;  };

private:
    uint32_t m_points;
    uint16_t m_words;
    uint8_t  m_level;

};


#endif /* _WT_PLAYER_H */
