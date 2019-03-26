/*!*****************************************************************************
 * @file wt_menu_pause.h
 * @brief menu class for pause mode
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
#ifndef _WT_MENU_PAUSE_H_
#define _WT_MENU_PAUSE_H_

#include "wt_menu_if.h"
#include "wt_menu_help.h"
#include "wt_game_mode_if.h"
#include "wt_game_ctr.h"

class WtMenuPause : public WtMenuIf
{
public:
    WtMenuPause() :
        WtMenuIf( 0x200, "bg_menu_pause.bmp", false )
    {
        size_t offset_x = (ACTIVE_WINDOW_WIDTH) / 2;
        size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2);

        add_button( WtButton( 1, WtCoord(offset_x - 158, offset_y-100), WtDim(100, 100), "back_btn.bmp" ) );
        add_button( WtButton( 2, WtCoord(offset_x + 58, offset_y-100), WtDim(100, 100), "redo_btn.bmp" ) );
        add_button( WtButton( 3, WtCoord(offset_x + 58, offset_y+100), WtDim(100, 100), "quit_btn.bmp" ) );
        add_button( WtButton( 4, WtCoord(offset_x - 158, offset_y+100), WtDim(100, 100), "help_btn.bmp" ) );
    }

    ~WtMenuPause()
    {
    }
private: // no copy allowed
    WtMenuPause( const WtMenuPause& ); 
    WtMenuPause & operator = (const WtMenuPause &);


    /**************************
     *
     *************************/
    virtual void notify_button_pressed( uint16_t id )
    {
        switch( TO_BUTTON_ID( id ) )
        {
            case 1:
                leave();
                break;
            case 2:
                GAME_CTR.restart();
                leave();
                break;
            case 3:
                GAME_CTR.quit();
                leave();
                break;
            case 4:
                enter_child_menu( m_help );
                break;
            default: break;
        }
    }

public:
    /**************************
     *
     *************************/
    virtual WtSettingsChangeObserver* get_help_listener()
    {
        return &m_help;
    }

private:
    WtMenuHelp m_help;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
