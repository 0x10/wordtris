/*!*****************************************************************************
 * @file wt_menu_game_mode_intro.h
 * @brief menu class for game mode intro
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
#ifndef _WT_MENU_GAME_MODE_INTRO_H_
#define _WT_MENU_GAME_MODE_INTRO_H_

#include "wt_view_if.h"

class WtMenuGameModeIntro : public WtViewIf
{
    static const size_t offset_x = (ACTIVE_WINDOW_WIDTH / 2);
    static const size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (69+20);
    const WtDim  m_standard_btn_size = WtDim( 328, 69 );
public:
    WtMenuGameModeIntro() :
        WtViewIf( "bg_menu_pause.bmp", false ),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-105) - 100, 800 ), 
                     WtDim(100, 100), 
                     "next_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuGameModeIntro::leave ) ),
        m_current_mode( nullptr ),
        m_show_next_time_btn( WtCoord( offset_x-164, offset_y + (20+((69 + 20)*2)) ),
                              m_standard_btn_size,
                              WtL10n_tr( "Show next time" ),
                             /* STORAGE.get_settings().show_intro*/true,
                              WT_BIND_EVENT_HANDLER_1( WtMenuGameModeIntro::show_next_time_changed ) ),
        m_textbox( WtCoord( (ACTIVE_WINDOW_WIDTH - 379) / 2,
                            ((ACTIVE_WINDOW_HEIGHT / 2) - (608 / 2)) - 50 ),
                   "", ACTIVE_WINDOW.get_text_font() ),
        m_show_next_time( true )
    {
        add_button( m_show_next_time_btn );
        add_button( m_leave_btn );
        add_textbox( m_textbox );
    }
    ~WtMenuGameModeIntro() {}

    /**************************
     *
     *************************/
    void set_game_mode( WtGameModeIf* mode )
    {
        m_current_mode = mode;
    }

private: // no copy allowed
    WtMenuGameModeIntro( const WtMenuGameModeIntro& ); 
    WtMenuGameModeIntro& operator = ( const WtMenuGameModeIntro& );

    /**************************
     *
     *************************/
    virtual void entered_view()
    {
        if ( ( nullptr != m_current_mode ) && ( m_show_next_time ) )
        {
            m_textbox.set_text( m_current_mode->get_hint() );
        }
        else
        {
            leave();
        }
    }

    /**************************
     *
     *************************/
    void show_next_time_changed( bool next_time_changed )
    {
        m_show_next_time = next_time_changed;
    }

private:
    WtButton         m_leave_btn;
    WtGameModeIf*    m_current_mode;
    WtCheckboxButton m_show_next_time_btn;
    WtTextbox        m_textbox;
    bool             m_show_next_time;
};

#endif /* _WT_MENU_GAME_MODE_INTRO_H_ */
