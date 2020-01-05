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
#include "wt_sfx.h"
#include "wt_utils.h"

#define PTS_P_WORD           (1000)

class WtPlayer
{
public:
    WtPlayer() :
        m_points( 0 ),
        m_words{0,0,0,0},
        m_level( 1 ),
        m_time( WtTime::from_seconds(0))
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
    void word_solved( size_t length=1, size_t successive_idx=0, uint32_t base_points=PTS_P_WORD )
    {
        m_words[0] ++;
        if (successive_idx != 0)
        {
            switch( successive_idx )
            {
                case 2:
                    m_words[1] ++;
                    break;
                case 3:
                    m_words[1] --;
                    m_words[2] ++;
                    break;
                case 4:
                    m_words[0] --;
                    m_words[1] --;
                    m_words[2] ++;
                    break;
                default: break;
            }
        }

        m_points += (static_cast<uint32_t>(length * base_points) * m_level);

        ACTIVE_SFX.play_solved_sound();

        uint8_t new_level = 1 + static_cast<uint32_t>(m_words[0] / 10);
        if ( new_level > m_level )
        {
            m_level = new_level;
            ACTIVE_SFX.play_levelup_sound();
        }

        
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
    void letter_dropped( uint8_t row_diff )
    {
        if ( row_diff > 0 )
        {
            uint32_t row_pts = static_cast<uint32_t>(row_diff);
            m_points += ( row_pts * ( 2 * m_level ) );

            ACTIVE_SFX.play_drop_sound();
        }
    }

    /**************************
     *
     *************************/
    void reset()
    {
        m_points = 0;
        m_words[0] = 0;
        m_words[1] = 0;
        m_words[2] = 0;
        m_words[3] = 0;
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
    void set_words_offset( uint16_t words )
    {
        m_words[0] = words;
    }

 
    /**************************
     *
     *************************/
     void set_time( WtTime::TimeType t )
     {
        m_time = t;
     }

    /**************************
     *
     *************************/
    uint32_t get_points() const            { return m_points; }
    uint16_t get_solved_word_count( uint8_t successive=1 ) const { return m_words[successive-1];  }
    uint32_t get_current_level() const     { return m_level;  }
    WtTime::TimeType get_current_time() const { return m_time; }

private:
    uint32_t m_points;
    uint16_t m_words[4];
    uint32_t m_level;
    WtTime::TimeType m_time;
};


#endif /* _WT_PLAYER_H */
