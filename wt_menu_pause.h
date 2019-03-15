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
private:
    static constexpr const char* background_image = "bg.bmp";
public:
    WtMenuPause() :
        WtMenuIf( 0x200 )
    {
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

    /**************************
     *
     *************************/
    virtual void show_self()
    {
        size_t offset_x = (ACTIVE_WINDOW_WIDTH - 500) / 2;
        size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (ACTIVE_WINDOW_HEIGHT / 4 )+ 80;
        add_button( 1, WtCoord(offset_x, offset_y+40), WtDim(500, 80), WtL10n::tr("resume"), false );
        add_button( 2, WtCoord(offset_x, offset_y+40+80+40), WtDim(500, 80), WtL10n::tr("quit"), false );

    }

    /**************************
     *
     *************************/
    virtual std::string get_bg_img()
    {
        return WtMenuPause::background_image;
    }
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
