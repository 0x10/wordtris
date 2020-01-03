/*!*****************************************************************************
 * @file wt_clickable_if.h
 * @brief input observer interface class
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
#ifndef _WT_CLICKABLE_H_
#define _WT_CLICKABLE_H_

#include "wt_utils.h"

class WtClickableIf
{
public:
    
    using ClickDetectedDelegate = std::function<void(WtCoord&)>;
    using PressDetectedDelegate = std::function<void(WtCoord&)>;
    using ReleaseDetectedDelegate = std::function<void(WtCoord&)>;


    using PanDetectedDelegate = std::function<void( WtCoord& press_start_pos, 
                                                    WtCoord& active_pos,
                                                    WtCoord& diff_pos )>;
    using MotionDetectedDelegate = std::function<void( WtCoord& active_pos, 
                                                       WtCoord& diff_pos )>;


public:
    WtClickableIf ( WtCoord pos, 
                    WtDim size,
                    ClickDetectedDelegate   click   = nullptr,
                    PanDetectedDelegate     pan     = nullptr,
                    PressDetectedDelegate   press   = nullptr,
                    ReleaseDetectedDelegate release = nullptr,
                    MotionDetectedDelegate  motion  = nullptr ) :
        m_press( press ),
        m_release( release ),
        m_click( click ),
        m_pan( pan ),
        m_motion( motion ),
        m_id( 0xFFFFFFFF ),
        m_pos( pos ),
        m_size( size ),
        m_press_pos( -1, -1 ),
        m_active_motion_pos( -1, -1 ),
        m_was_pan( false ),
        m_press_time()
    {
    };
 
    ~WtClickableIf ()
    {
    };

    /**************************
     *
     *************************/
    void set_id( size_t id )
    {
        m_id = id;
    }

    /**************************
     *
     *************************/
    size_t id()
    {
        return m_id;
    }

    /**************************
     *
     *************************/
    void set_position( WtCoord new_pos )
    {
        m_pos = new_pos;
    }

    /**************************
     *
     *************************/
    void set_size( WtDim new_size )
    {
        std::cout << "new size = " << new_size << std::endl;
        m_size = new_size;
    }
    /**************************
     *
     *************************/
    void trigger_press ( WtCoord& pos )
    {
        if ( pos.in_region( m_pos, m_size ) )
        {
            m_press_time = WtTime::get_time();

            m_press_pos = pos;
            m_active_motion_pos = pos;
            if ( m_press ) m_press ( pos );
        }
    }

    /**************************
     *
     *************************/
    void trigger_release ( WtCoord& pos )
    {
        if ( m_press_pos != WtCoord( -1, -1 ) )
        {
            WtTime::TimePoint release_time = WtTime::get_time();

            // call on_release handler 
            if ( m_release )                     m_release ( pos );

            // call on_click handler
            WtTime::TimeType elapsed = WtTime::get_time_elapsed( m_press_time, release_time );
            if ( elapsed < WtTime::from_milliseconds(125) )
            {
                if ( m_click ) m_click ( pos );
            }
            else
            {
                if ( !m_was_pan && ( elapsed < WtTime::from_milliseconds(500) ))
                {
                    if ( m_click ) m_click ( pos );
                }
            }

            m_press_pos = WtCoord( -1, -1 );
            m_active_motion_pos = WtCoord( -1, -1 );
            m_was_pan = false;
        }
    }

    /**************************
     *
     *************************/
    void trigger_motion ( WtCoord& pos, WtCoord& d_pos )
    {
        if ( m_press_pos != WtCoord( -1, -1 ) )
        {
          //  std::cout << "pressed motion at delta " << d_pos << std::endl;
            if ( m_pan )
            {
                m_active_motion_pos = m_active_motion_pos + d_pos;
                if ( !m_active_motion_pos.near_point( m_press_pos, 20 ) )
                {
                  //  std::cout << "drag started: " << pos << " != " << m_press_start_pos << std::endl;
                    m_was_pan = true;
                }

                m_pan( m_press_pos, m_active_motion_pos, d_pos );
            }
        }
        if ( m_motion ) m_motion ( pos, d_pos );
    }

protected:
    PressDetectedDelegate   m_press;
    ReleaseDetectedDelegate m_release;
    ClickDetectedDelegate   m_click;
    PanDetectedDelegate     m_pan;
    MotionDetectedDelegate  m_motion;
    size_t                  m_id;
    WtCoord           m_pos;
    WtDim             m_size;

    WtCoord                 m_press_pos;
    WtCoord                 m_active_motion_pos;
    bool                    m_was_pan;
    WtTime::TimePoint       m_press_time;
};




#endif /* _WT_CLICKABLE_H_ */
