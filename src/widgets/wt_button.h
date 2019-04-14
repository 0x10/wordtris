/*!*****************************************************************************
 * @file wt_button.h
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
#ifndef _WT_BUTTON_H_
#define _WT_BUTTON_H_


#include "wt_clickable_if.h"


class WtButton 
{
public:
    using OnTapDelegate = std::function<void(void)>;

    WtButton( WtCoord pos, WtDim size,
              std::string button_image,
              OnTapDelegate on_tap,
              std::string label="" ) :

        m_clickable( pos, size,
                     WT_BIND_EVENT_HANDLER_1( WtButton::on_click ) ),

        m_pos( pos ),
        m_size( size ),
        m_img( button_image ),
        m_label( label ),
        m_on_tap( on_tap )
    {
    }
    
    ~WtButton() {}

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
    std::string label()
    {
        return WtL10n::translate(m_label);
    }

    /**************************
     *
     *************************/
    std::string image()
    {
        return m_img;
    }

    /**************************
     *
     *************************/
    void set_image( std::string img )
    {
        m_img = img;
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

public:

    /**************************
     *
     *************************/
    void on_click( WtCoord& )
    {
        if ( m_on_tap ) m_on_tap();
    }


private:
    WtClickableIf m_clickable;

    WtCoord m_pos;
    WtDim m_size;
    std::string m_img;
    std::string m_label;

    OnTapDelegate m_on_tap;
};

#endif /* _WT_BUTTON_H_ */
