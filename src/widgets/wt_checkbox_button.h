/*!*****************************************************************************
 * @file wt_checkbox_button.h
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
#ifndef _WT_CHECKBOX_BUTTON_H_
#define _WT_CHECKBOX_BUTTON_H_


#include "wt_clickable_if.h"
#include "widgets/wt_button.h"

class WtCheckboxButton 
{
private:
    const char* m_checkbox_btn_unchecked =   "checkbox_unchecked_btn.bmp";
    const char* m_checkbox_btn_checked   =   "checkbox_checked_btn.bmp";

public:
    using OnTapDelegate = std::function<void(bool)>;

    WtCheckboxButton( WtCoord pos, 
                      WtDim size,
                      std::string label,
                      bool checked,
                      OnTapDelegate on_tap ) :
        m_button( pos, size,
                  ( checked ? m_checkbox_btn_checked : m_checkbox_btn_unchecked ),
                  WT_BIND_EVENT_HANDLER( WtCheckboxButton::on_click ),
                  label ),
        m_checked( checked ),
        m_on_tap( on_tap )
    {
    }
    
    ~WtCheckboxButton() {}

    /**************************
     *
     *************************/
    operator WtClickableIf&()
    {
        return m_button.get_observable();
    }
    /**************************
     *
     *************************/
    WtClickableIf& get_observable()
    {
        return m_button;
    }

    /**************************
     *
     *************************/
    operator WtButton&()
    {
        return m_button;
    }

    /**************************
     *
     *************************/
    ssize_t width()
    {
        return m_button.width();
    }

    /**************************
     *
     *************************/
    ssize_t height()
    {
        return m_button.height();
    }

    /**************************
     *
     *************************/
    WtDim size() const
    {
        return m_button.size();
    }

    /**************************
     *
     *************************/
    WtCoord position() const
    {
        return m_button.position();
    }

    /**************************
     *
     *************************/
    void set_position( WtCoord new_pos )
    {
        m_button.set_position( new_pos );
    }

    /**************************
     *
     *************************/
    ssize_t x()
    {
        return m_button.x();
    }

    /**************************
     *
     *************************/
    void set_x( ssize_t x )
    {
        m_button.set_x( x );
    }

    /**************************
     *
     *************************/
    ssize_t y()
    {
        return m_button.y();
    }

    /**************************
     *
     *************************/
    void set_checked( bool new_checked )
    {
        m_checked = new_checked;
    }


    /**************************
     *
     *************************/
    bool is_checked()
    {
        return m_checked;
    }

public:

    /**************************
     *
     *************************/
    void on_click()
    {
        if ( m_checked )
        {
            m_button.set_image( m_checkbox_btn_unchecked );
            m_checked = false;
        }
        else
        {
            m_button.set_image( m_checkbox_btn_checked );
            m_checked = true;
        }

        if ( m_on_tap ) m_on_tap( m_checked );
    }

private:
    WtCheckboxButton( const WtCheckboxButton& ); 
    WtCheckboxButton& operator = (const WtCheckboxButton&);

private:
    WtButton m_button;
    bool     m_checked;

    OnTapDelegate m_on_tap;
};

#endif /* _WT_CHECKBOX_BUTTON_H_ */
