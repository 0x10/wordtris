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

#include "global.h"

#define WT_BIND_EVENT_HANDLER( func )   std::bind ( &func, this )
#define WT_BIND_EVENT_HANDLER_1( func ) std::bind ( &func, this, std::placeholders::_1 )
#define WT_BIND_EVENT_HANDLER_2( func ) std::bind ( &func, this, std::placeholders::_1, std::placeholders::_2  )
#define WT_BIND_EVENT_HANDLER_3( func ) std::bind ( &func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 )

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
        m_was_pan( false )
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
    void trigger_press ( WtCoord& pos )
    {
        if ( pos.in_region( m_pos, m_size ) )
        {
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
            // call on_release handler 
            if ( m_release )                     m_release ( pos );
            // call on_click handler 
            if ( ( !m_was_pan ) && ( m_click ) ) m_click ( pos );

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
                if ( !m_active_motion_pos.near_point( m_press_pos, 2 ) )
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
    const WtCoord           m_pos;
    const WtDim             m_size;

    WtCoord                 m_press_pos;
    WtCoord                 m_active_motion_pos;
    bool                    m_was_pan;
};




#endif /* _WT_CLICKABLE_H_ */
