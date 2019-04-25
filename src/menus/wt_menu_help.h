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

#include "wt_view_if.h"
#include "wt_game_mode_if.h"
#include "wt_game_mode_ctr.h"
#include "wt_storage.h"

class WtMenuHelp : public WtViewIf
{
public:
    WtMenuHelp() :
        WtViewIf( "bg_menu_pause.bmp", false ),
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
    void update_view()
    {
        WtGameModeIf* active_mode = GAME_MODE_CTR.mode_from_string( STORAGE.get_settings().game_mode );
        if ( active_mode != INVALID_GAME_MODE )
        {
            ACTIVE_WINDOW.draw_help_box( active_mode->get_help() );
        }
    }

private:

    WtButton      m_leave_btn;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
