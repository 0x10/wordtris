/*!*****************************************************************************
 * @file wt_tristate_button.h
 * @brief button helper class
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
#ifndef _WT_TRISTATE_BUTTON_H_
#define _WT_TRISTATE_BUTTON_H_


#include "wt_clickable_if.h"


class WtTriStateButton 
{
public:
    using OnItemTapDelegate = std::function<void(uint8_t)>;
    const std::string m_tri_state_img[3] = { "tri_state_btn_select0.bmp", "tri_state_btn_select1.bmp", "tri_state_btn_select2.bmp" };


    WtTriStateButton( WtCoord pos, 
                      WtDim size,
                      std::string label[3],
                      uint8_t selected,
                      OnItemTapDelegate on_item_tap ) :

        m_clickable( std::bind ( &WtButton::on_press, this, std::placeholders::_1 ),
                     std::bind ( &WtButton::on_release, this, std::placeholders::_1 ),
                     std::bind ( &WtButton::on_motion, this, std::placeholders::_1, std::placeholders::_2 ) ),

        m_pos( pos ),
        m_size( size ),
        m_item_size( size.w - 1 / 3, size.h - 2 ),
        m_img( button_image ),
        m_label( label ),
        m_selected( selected ),
        m_on_item_tap( on_item_tap )
    {
    }
    
    ~WtTriStateButton() {}

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
    std::string label()
    {
        return WtL10n::translate(m_label[m_selected]);
    }

    /**************************
     *
     *************************/
    std::string image()
    {
        return m_tri_state_img[m_selected];
    }

    /**************************
     *
     *************************/
    ssize_t width()
    {
        return m_size.w;
    }

    /**************************
     *
     *************************/
    ssize_t height()
    {
        return m_size.h;
    }

    /**************************
     *
     *************************/
    WtDim size() const
    {
        return m_size;
    }

    /**************************
     *
     *************************/
    WtCoord position() const
    {
        return m_pos;
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
    ssize_t x()
    {
        return m_pos.x;
    }

    /**************************
     *
     *************************/
    void set_x( ssize_t x )
    {
        m_pos.x = x;
    }

    /**************************
     *
     *************************/
    ssize_t y()
    {
        return m_pos.y;
    }

private:
    /**************************
     *
     *************************/
    void on_press( WtCoord& pos )
    {
    }

    /**************************
     *
     *************************/
    void on_release( WtCoord& pos )
    {
        if ( pos.in_region( m_pos, m_size ) )
        {
            WtCoord working_state_pos = WtCoord( m_pos.x + 1, m_pos.y + 1 );

            uint8_t selected = 0;
            if ( pos.in_region( working_state_pos, m_item_size )
            {
                selected = 0;
            }

            working_state_pos.moveX( m_item_size );           
            if ( pos.in_region( working_state_pos, m_item_size )
            {
                selected = 1;
            }

            working_state_pos.moveX( m_item_size );           
            if ( pos.in_region( working_state_pos, m_item_size )
            {
                selected = 2;
            }

            m_selected = selected;

            if ( m_on_item_tap ) m_on_item_tap( m_selected );
        }
    }

    /**************************
     *
     *************************/
    void on_motion( WtCoord& pos, WtCoord& d_pos )
    {
    }

private:
    WtClickableIf m_clickable;

    WtCoord       m_pos;
    WtDim         m_size;
    WtDim         m_item_size;
    std::string   m_label[3];

    uint8_t       m_selected;

    OnTapDelegate m_on_item_tap;
};

#endif /* _WT_TRISTATE_BUTTON_H_ */
