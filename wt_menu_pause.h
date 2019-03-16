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
#include "wt_game_ctr.h"

class WtMenuPause : public WtMenuIf
{
public:
    WtMenuPause() :
        WtMenuIf( 0x200, "bg.bmp" )
    {
        size_t offset_x = (ACTIVE_WINDOW_WIDTH) / 2;
        size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2);

        add_button( WtButton( 1, WtCoord(offset_x - 158, offset_y-66), WtDim(58, 66), "redo_btn.bmp" ) );
        add_button( WtButton( 2, WtCoord(offset_x + 80, offset_y-73), WtDim(80, 73), "quit_btn.bmp" ) );
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
        if ( TO_BUTTON_ID( id ) == 1 )
        {
            leave();
        }

        if ( TO_BUTTON_ID( id ) == 2 )
        {
            GAME_CTR.quit();
            leave();
        }
    }

};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
