/*!*****************************************************************************
 * @file wt_menu_help.h
 * @brief menu class for help screen
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
#ifndef _WT_MENU_HELP_H_
#define _WT_MENU_HELP_H_

#include "wt_menu_if.h"
#include "wt_game_mode_if.h"
#include "wt_game_mode_ctr.h"
#include "wt_storage.h"

class WtMenuHelp : public WtMenuIf, public WtSettingsChangeObserver
{
public:
    WtMenuHelp() :
        WtMenuIf( 0x500, "bg_menu_pause.bmp", false ),
        m_selected_mode( GAME_MODE_CTR.mode_from_string( STORAGE.get_settings().game_mode ) ),
        m_leave_btn( WtCoord( 105, 800 ), 
                     WtDim(100, 100), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHelp::leave ) )
    {
        add_button( m_leave_btn );
    }

    ~WtMenuHelp()
    {
    }
private: // no copy allowed
    WtMenuHelp( const WtMenuHelp& ); 
    WtMenuHelp & operator = (const WtMenuHelp &);

    /**************************
     *
     *************************/
    void notify_language_changed( std::string ) {}
    /**************************
     *
     *************************/
    virtual void notify_difficulty_changed( wt_difficulty ) {}
    /**************************
     *
     *************************/
    virtual void notify_theme_changed( std::string ) {}

    /**************************
     *
     *************************/
    virtual void notify_game_mode_changed( WtGameModeIf* mode )
    {
        m_selected_mode = mode;
    }

    /**************************
     * 
     *************************/
    void menu_update()
    {
        if ( m_selected_mode != INVALID_GAME_MODE )
        {
            ACTIVE_WINDOW.draw_help_box( m_selected_mode->get_help() );
        }
    }

private:
    WtGameModeIf* m_selected_mode;

    WtButton      m_leave_btn;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
