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
#include "wt_menu_game_mode_select.h"
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
        WtMenuIf( 0x100 )
    {
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
        m_select_mode.listen( listener );
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
                enter_child_menu( m_select_mode );
                break;
            case 2:
                leave();
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
    virtual void show_self()
    {
        size_t offset_x = (ACTIVE_WINDOW_WIDTH - 500) / 2;
        size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (ACTIVE_WINDOW_HEIGHT / 4 )+ 120;

        add_button( 1, WtCoord(offset_x, offset_y), WtDim(500, 80), WtL10n::tr("select game mode"), false );
        add_button( 2, WtCoord(offset_x, offset_y+80+20), WtDim(500, 80), WtL10n::tr("start game"), false );
        add_button( 3, WtCoord(offset_x, offset_y+160+40), WtDim(500, 80), WtL10n::tr("options"), false );
    }

    /**************************
     *
     *************************/
    virtual void menu_update()
    {
        std::string text[] = { "Team:","=====", "Coding / Artwork", "C. Kranz", "Idea / Android Support", "W. Krzeslowski" };
        static uint8_t text_id = 0;
        static float lastpos = 0.0;
        ACTIVE_WINDOW.draw_text( WtCoord(((ACTIVE_WINDOW_WIDTH) / 2)- (text[text_id].length()/2 * 12), ACTIVE_WINDOW_HEIGHT-lastpos), text[text_id]  );
        ACTIVE_WINDOW.draw_text( WtCoord(((ACTIVE_WINDOW_WIDTH) / 2)- (text[text_id+1].length()/2 * 12), ACTIVE_WINDOW_HEIGHT-lastpos+24), text[text_id+1]  );
        lastpos = (lastpos+0.5);
        if (lastpos >= 200.) lastpos = 0;
        
        if (lastpos == 0)
            text_id = (text_id+2)%6;
    }
private:
    WtMenuSelectMode m_select_mode;
    WtMenuSettings   m_settings;
    WtMenuPause      m_pause_menu;
};

#endif /* _WT_MENU_CTR_H_ */
