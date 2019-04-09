/*!*****************************************************************************
 * @file wt_grid_touch_overlay.h
 * @brief grid touch helper class
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
#ifndef _WT_GRID_TOUCH_OVERLAY_H_
#define _WT_GRID_TOUCH_OVERLAY_H_


#include "wt_clickable_if.h"


class WtGridTouchOverlay
{
public:
    using OnLeftDetected = std::function<void(void)>;
    using OnRightDetected = std::function<void(void)>;
    using OnDropDetected = std::function<void(void)>;


    WtGridTouchOverlay( WtCoord pos, WtDim size,
                        OnLeftDetected on_left,
                        OnRightDetected on_right,
                        OnDropDetected on_drop ) :

        m_clickable( std::bind ( &WtGridTouchOverlay::on_press, this, std::placeholders::_1 ),
                     std::bind ( &WtGridTouchOverlay::on_release, this, std::placeholders::_1 ),
                     std::bind ( &WtGridTouchOverlay::on_motion, this, std::placeholders::_1, std::placeholders::_2 ) ),

        m_pos( pos ),
        m_size( size ),
        m_pos_left( pos ),
        m_pos_right( size.w / 2, pos.y ),
        m_pos_drop( pos.x, pos.y + (size.h / 2) + (size.h / 4) ),
        m_size_left( size.w / 2, (size.h / 2) + (size.h / 4) ),
        m_size_right( size.w / 2, (size.h / 2) + (size.h / 4) ),
        m_size_drop( size.w, size.h - ((size.h / 2) + (size.h / 4)) ),
        m_press_start_pos( -1, -1 ),
        m_active_motion_pos( -1, -1 ),
        m_on_left( on_left ),
        m_on_right( on_right ),
        m_on_drop( on_drop )
    {
    }
    
    ~WtGridTouchOverlay() {}

    /**************************
     *
     *************************/
    operator WtClickableIf&()
    {
        return get_observable();
    }

    /**************************
     *
     *************************/
    WtClickableIf& get_observable()
    {
        return m_clickable;
    }

public:
    /**************************
     *
     *************************/
    void on_press( WtCoord& pos )
    {
        std::cout << "on press at " << pos << std::endl;
        if ( pos.in_region( m_pos, m_size ) )
        {
            m_press_start_pos = pos;
            m_active_motion_pos = pos;
        }
    }

    /**************************
     *
     *************************/
    void on_release( WtCoord& pos )
    {
//        std::cout << "release detected: (" << pos.x << "," << pos.y << ") -> (" << m_pos.x << "," << m_pos.y << "):(" << m_size.w << "," << m_size.h << ") -> " << m_label << std::endl;
        if ( pos.in_region( m_pos_left, m_size_left ) )
        {
            if ( m_on_left ) m_on_left();
        }
        if ( pos.in_region( m_pos_right, m_size_right ) )
        {
            if ( m_on_right ) m_on_right();
        }
        if ( pos.in_region( m_pos_drop, m_size_drop ) )
        {
            if ( m_on_drop ) m_on_drop();
        }
        m_press_start_pos = WtCoord( -1, -1 );
        m_active_motion_pos = m_press_start_pos;
    }

    /**************************
     *
     *************************/
    void on_motion( WtCoord& pos, WtCoord& d_pos )
    {
        if ( m_press_start_pos != WtCoord( -1, -1 ) )
        {
            m_active_motion_pos = m_active_motion_pos + d_pos;
            if ( m_active_motion_pos.in_region( m_pos_left, m_size_left ) )
            {
                if ( m_on_left ) m_on_left();
            }
            if ( m_active_motion_pos.in_region( m_pos_right, m_size_right ) )
            {
                if ( m_on_right ) m_on_right();
            }
            if ( m_active_motion_pos.in_region( m_pos_drop, m_size_drop ) )
            {
                if ( m_on_drop ) m_on_drop();
            }
        }
    }

private:
    WtClickableIf m_clickable;

    const WtCoord   m_pos;
    const WtDim     m_size;

    const WtCoord   m_pos_left;
    const WtCoord   m_pos_right;
    const WtCoord   m_pos_drop;
    const WtDim     m_size_left;
    const WtDim     m_size_right;
    const WtDim     m_size_drop;

    WtCoord         m_press_start_pos;
    WtCoord         m_active_motion_pos;

    OnLeftDetected  m_on_left;
    OnRightDetected m_on_right;
    OnDropDetected  m_on_drop;
};

#endif /* _WT_BUTTON_H_ */
