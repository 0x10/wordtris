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

#include "widgets/wt_textbox.h"

#include "wt_view_if.h"
#include "wt_game_mode_if.h"
#include "wt_game_mode_ctr.h"
#include "wt_storage.h"

class WtMenuHelp : public WtViewIf
{
public:
    WtMenuHelp() :
        WtViewIf( "#172d4a", false ),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     //WtDim(100, 100), 
                     WtDim(138, 124), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHelp::leave ) ),
        m_textbox( WtCoord( (ACTIVE_WINDOW_WIDTH - 379) / 2,
                            (ACTIVE_WINDOW_HEIGHT / 2) - (608 / 2) ),
                   "", ACTIVE_WINDOW.get_text_font() )
    {
        add_button( m_leave_btn );
        add_textbox( m_textbox );
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
    void entered_view()
    {
        WtGameModeIf* active_mode = GAME_MODE_CTR.mode_from_string( STORAGE.get_settings().game_mode );
        if ( active_mode != INVALID_GAME_MODE )
        {
            m_textbox.set_text( active_mode->get_help() );
        }
    }

private:
    WtButton  m_leave_btn;
    WtTextbox m_textbox;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
