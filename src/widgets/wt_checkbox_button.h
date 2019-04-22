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
                  label,
                  WtCoord( -1 * (size.w / 6),0)),
        m_checked( checked ),
        m_on_tap( on_tap )
    {
    }
    
    ~WtCheckboxButton() {}

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
    void set_checked( bool new_checked )
    {
        m_checked = new_checked;
        m_button.set_image( ( m_checked ? m_checkbox_btn_checked : m_checkbox_btn_unchecked ) );
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
