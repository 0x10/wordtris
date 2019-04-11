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

class WtClickableIf
{
public:
    using PressDetectedDelegate = std::function<void(WtCoord&)>;
    using ReleaseDetectedDelegate = std::function<void(WtCoord&)>;
    using MotionDetectedDelegate = std::function<void(WtCoord&, WtCoord&)>;

public:
    WtClickableIf ( PressDetectedDelegate   press,
                    ReleaseDetectedDelegate release,
                    MotionDetectedDelegate  motion ) :
        m_press( press ),
        m_release( release ),
        m_motion( motion ),
        m_id( -1 )
    {
    };
 
    ~WtClickableIf ()
    {
    };

    /**************************
     *
     *************************/
    void set_id( ssize_t id )
    {
        m_id = id;
    }

    /**************************
     *
     *************************/
    ssize_t id()
    {
        return m_id;
    }

    /**************************
     *
     *************************/
    void trigger_press ( WtCoord& pos )
    {
        if ( m_press ) m_press ( pos );
    }

    /**************************
     *
     *************************/
    void trigger_release ( WtCoord& pos )
    {
        if ( m_release ) m_release ( pos );
    }

    /**************************
     *
     *************************/
    void trigger_motion ( WtCoord& pos, WtCoord& d_pos )
    {
        if ( m_motion ) m_motion ( pos, d_pos );
    }

protected:
    PressDetectedDelegate   m_press;
    ReleaseDetectedDelegate m_release;
    MotionDetectedDelegate  m_motion;
    ssize_t                 m_id;
};




#endif /* _WT_CLICKABLE_H_ */
