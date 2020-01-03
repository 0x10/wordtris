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
    using OnClickDetected = std::function<void(WtCoord)>;


    WtGridTouchOverlay( WtCoord pos, WtDim size,
                        WtDim grid_item_size,
                        OnClickDetected on_click ) :

        m_clickable( pos, size,
                     WT_BIND_EVENT_HANDLER_1( WtGridTouchOverlay::on_click ),
                     WT_BIND_EVENT_HANDLER_3( WtGridTouchOverlay::on_pan ) ),

        m_pos( pos ),
        m_size( size ),
        m_grid_item_size( grid_item_size ),
        m_on_click( on_click )
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

    /**************************
     *
     *************************/
    void set_position( WtCoord new_pos )
    {
        m_pos = new_pos;
        m_clickable.set_position(m_pos);
    }

    /**************************
     *
     *************************/
    void set_size( WtDim new_size )
    {
        std::cout << "new size = " << new_size << std::endl;
        m_size = new_size;
        m_clickable.set_size(m_size);
    }
public:
    /**************************
     *
     *************************/
    void on_click( WtCoord& pos )
    {
        if ( pos.in_region( m_pos, m_size ) )
            if ( m_on_click ) m_on_click( pos_to_grid_index( pos ) );
    }

    /**************************
     *
     *************************/
    WtCoord pos_to_grid_index( WtCoord& pos )
    {
        WtCoord local_pos = pos - m_pos;
        WtCoord grid_pos = local_pos / m_grid_item_size;
        return grid_pos;
    }

    /**************************
     *
     *************************/
    void on_pan( WtCoord& press_start_pos, WtCoord& active_pos, WtCoord& /*d_pos*/ )
    {
        if ( !active_pos.in_region( m_pos, m_size ) )
            return;
        if ( (active_pos.x+m_grid_item_size.w) < press_start_pos.x )
        {
            if ( m_on_click ) m_on_click( pos_to_grid_index( active_pos ) );
        }
        if ( (active_pos.x-m_grid_item_size.w) > press_start_pos.x )
        {
            if ( m_on_click ) m_on_click( pos_to_grid_index( active_pos ) );
        }
    }


private:
    WtClickableIf m_clickable;

    WtCoord   m_pos;
    WtDim     m_size;
    const WtDim     m_grid_item_size;

    OnClickDetected  m_on_click;
};

#endif /* _WT_BUTTON_H_ */
