/*!*****************************************************************************
 * @file wt_menu_ctr.h
 * @brief main menu control class
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
#ifndef _WT_MENU_CTR_H_
#define _WT_MENU_CTR_H_

#include <vector>
#include <unistd.h>

#include "global.h"
#include "wt_menu_if.h"
#include "wt_drawing.h"
#include "wt_menu_settings.h"
#include "wt_menu_pause.h"

#define MENU_CTR  WtMenuCtr::instance()
class WtMenuCtr : public WtMenuIf
{
// singleton definition
public:
    static WtMenuCtr& instance()
    {
        static WtMenuCtr _instance;
        return _instance;
    }
    ~WtMenuCtr()
    {
    }
private:
    WtMenuCtr() :
        WtMenuIf( 0x100, "bg_menu_settings.bmp" )
    {
        add_button( WtButton( 1, WtCoord( 170, 493 ), WtDim( 200, 200 ), "start_btn.bmp" ) );
        add_button( WtButton( 2, WtCoord( 105, 800 ), WtDim( 100, 100 ), "score_btn.bmp" ) );
        add_button( WtButton( 3, WtCoord( 332, 800 ), WtDim( 100, 100 ), "settings_btn.bmp" ) );
    }
    WtMenuCtr( const WtMenuCtr& ); 
    WtMenuCtr & operator = (const WtMenuCtr &);

public:
    /**************************
     *
     *************************/
    WtMenuIf* get_pause_menu()
    {
        return &m_pause_menu;
    }

    /**************************
     *
     *************************/
    virtual void listen( WtSettingsChangeObserver* listener )
    {
        WtMenuIf::listen( listener );
        m_settings.listen( listener );
    }
private:
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
                //leave();
                break;
            case 3:
                enter_child_menu( m_settings );
                break;
            default: /*ignore*/ break;
        }
    }
    /**************************
     *
     *************************/
    virtual void notify_motion( WtCoord pos, WtCoord d_pos )
    {
    }

    /**************************
     *
     *************************/
    virtual void menu_update()
    {
#if 0
        std::string text[] = { "Team:","=====", "Coding / Artwork", "C. Kranz", "Idea / Android Support", "W. Krzeslowski" };
        static uint8_t text_id = 0;
        static float lastpos = 0.0;
        ACTIVE_WINDOW.draw_text( WtCoord(((ACTIVE_WINDOW_WIDTH) / 2)- (text[text_id].length()/2 * 12), ACTIVE_WINDOW_HEIGHT-lastpos), text[text_id]  );
        ACTIVE_WINDOW.draw_text( WtCoord(((ACTIVE_WINDOW_WIDTH) / 2)- (text[text_id+1].length()/2 * 12), ACTIVE_WINDOW_HEIGHT-lastpos+24), text[text_id+1]  );
        lastpos = (lastpos+0.5);
        if (lastpos >= 200.) lastpos = 0;
        
        if (lastpos == 0)
            text_id = (text_id+2)%6;
#endif
    }
private:
    WtMenuSettings   m_settings;
    WtMenuPause      m_pause_menu;
};

#endif /* _WT_MENU_CTR_H_ */
