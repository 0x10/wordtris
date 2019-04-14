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

        m_clickable( pos, size,
                     WT_BIND_EVENT_HANDLER_1( WtGridTouchOverlay::on_click ),
                     WT_BIND_EVENT_HANDLER_3( WtGridTouchOverlay::on_pan ) ),

        m_pos( pos ),
        m_size( size ),
        m_pos_left( pos ),
        m_pos_right( size.w / 2, pos.y ),
        m_pos_drop( pos.x, pos.y + (size.h / 2) + (size.h / 4) ),
        m_size_left( size.w / 2, (size.h / 2) + (size.h / 4) ),
        m_size_right( size.w / 2, (size.h / 2) + (size.h / 4) ),
        m_size_drop( size.w, size.h - ((size.h / 2) + (size.h / 4)) ),
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
    void on_click( WtCoord& pos )
    {
        if ( pos.in_region( m_pos_left, m_size_left ) )
        {
           // std::cout << "click left\n";
            if ( m_on_left ) m_on_left();
        }
        if ( pos.in_region( m_pos_right, m_size_right ) )
        {
           // std::cout << "click right\n";
            if ( m_on_right ) m_on_right();
        }
        if ( pos.in_region( m_pos_drop, m_size_drop ) )
        {
            if ( m_on_drop ) m_on_drop();
        }
    }


    /**************************
     *
     *************************/
    void on_pan( WtCoord& press_start_pos, WtCoord& active_pos, WtCoord& /*d_pos*/ )
    {
        if ( active_pos.in_region( m_pos_drop, m_size_drop ) )
        {
            if ( m_on_drop ) m_on_drop();
        }
        else
        {
            if ( (active_pos.x+30) < press_start_pos.x )
            {
                press_start_pos = active_pos;
                //std::cout << "motion left\n";
                if ( m_on_left ) m_on_left();
            }
            if ( (active_pos.x-30) > press_start_pos.x )
            {
                press_start_pos = active_pos;
               // std::cout << "motion right\n";
                if ( m_on_right ) m_on_right();
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

    OnLeftDetected  m_on_left;
    OnRightDetected m_on_right;
    OnDropDetected  m_on_drop;
};

#endif /* _WT_BUTTON_H_ */
