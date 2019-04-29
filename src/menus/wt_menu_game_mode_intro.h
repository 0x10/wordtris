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
public:
    WtMenuGameModeIntro() :
        WtViewIf( "bg_menu_pause.bmp", false ),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-105) - 100, 800 ), 
                     WtDim(100, 100), 
                     "next_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuGameModeIntro::leave ) ),
        m_current_mode( nullptr )
    {
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
            ACTIVE_WINDOW.draw_message( m_current_mode->get_hint() );
        }
        else
        {
            leave();
        }
    }

private:
    WtButton        m_leave_btn;
    WtGameModeIf*   m_current_mode;
};

#endif /* _WT_MENU_GAME_MODE_INTRO_H_ */
