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

#include "wt_menu_if.h"
#include "menus/wt_menu_settings.h"
#include "menus/wt_menu_highscores.h"
#include "menus/wt_menu_pause.h"
#include "widgets/wt_horizontal_carousel.h"

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
        WtMenuIf( 0x100, "bg_menu_settings.bmp" ),
        m_drag_start_pos(0,0),
        m_was_drag(false),
        m_drag_button_id(0),
        m_settings(),
        m_pause_menu(),
        m_scores(),
        m_score_btn( WtCoord( 105, 800 ), 
                     WtDim( 100, 100 ), 
                     "score_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuCtr::enter_score_menu ) ),
        m_setting_btn( WtCoord( 332, 800 ),
                       WtDim( 100, 100 ),
                       "settings_btn.bmp",
                       WT_BIND_EVENT_HANDLER( WtMenuCtr::enter_settings_menu ) ),
        m_game_selection( WtCoord( 0, 493 ),
                          WtDim( ACTIVE_WINDOW_WIDTH, 200 ),
                          GAME_MODE_CTR.get_available_mode_titles(),
                          GAME_MODE_CTR.mode_idx_from_string( STORAGE.get_settings().game_mode ),
                          WT_BIND_EVENT_HANDLER_1( WtMenuCtr::game_mode_selected ) )
    {
        add_button( m_score_btn );
        add_button( m_setting_btn );
        add_horizontal_carousel( m_game_selection );
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
        m_pause_menu.listen( listener );
    }
private:
    /**************************
     *
     *************************/
    void enter_score_menu()
    {
        enter_child_menu( m_scores );
    }

    /**************************
     *
     *************************/
    void enter_settings_menu()
    {
        enter_child_menu( m_settings );
    }

    /**************************
     *
     *************************/
    void game_mode_selected( size_t idx )
    {
        std::cout << "selected = " << idx << std::endl;
        for( size_t l_idx = 0; l_idx < get_listener().size(); l_idx++ )
        {
            get_listener()[l_idx]->notify_game_mode_changed( GAME_MODE_CTR.get_available_modes()[idx] );
        }
        leave();
    }



private:
    WtCoord          m_drag_start_pos;
    bool             m_was_drag;
    uint16_t         m_drag_button_id;

    WtMenuSettings   m_settings;
    WtMenuPause      m_pause_menu;
    WtMenuHighscores m_scores;

    WtButton         m_score_btn;
    WtButton         m_setting_btn;

    WtHorizontalCarousel m_game_selection;
};

#endif /* _WT_MENU_CTR_H_ */
