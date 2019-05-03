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
#include "widgets/wt_checkbox_button.h"

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
        m_show_next_time_btn( WtCoord( offset_x-164, offset_y + (20+((69 + 20)*3)) ),
                              m_standard_btn_size,
                              WtL10n_tr( "Show next time" ),
                             /* STORAGE.get_settings().show_intro*/true,
                              WT_BIND_EVENT_HANDLER_1( WtMenuGameModeIntro::show_next_time_changed ) )
    {
        add_button( m_show_next_time_btn );
        add_button( m_leave_btn );
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
    virtual void update_view()
    {
        if ( nullptr != m_current_mode )
        {
            ACTIVE_WINDOW.draw_help_box( m_current_mode->get_hint() );
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

    }

private:
    WtButton        m_leave_btn;
    WtGameModeIf*   m_current_mode;
    WtCheckboxButton m_show_next_time_btn;
};

#endif /* _WT_MENU_GAME_MODE_INTRO_H_ */
