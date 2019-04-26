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

#include "wt_view_if.h"
#include "wt_game_ctr.h"
#include "menus/wt_menu_settings.h"
#include "menus/wt_menu_highscores.h"
#include "widgets/wt_horizontal_carousel.h"


class WtMenuCtr : public WtViewIf
{
public:
    WtMenuCtr() :
        WtViewIf(),
        m_drag_start_pos(0,0),
        m_was_drag(false),
        m_drag_button_id(0),
        m_settings(),
        m_scores(),
        m_game_ctr(),
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
        m_game_ctr.set_game_mode( GAME_MODE_CTR.mode_from_string( STORAGE.get_settings().game_mode ) );

        add_button( m_score_btn );
        add_button( m_setting_btn );
        add_horizontal_carousel( m_game_selection );
    }

    ~WtMenuCtr() {}
private:
    WtMenuCtr( const WtMenuCtr& ); 
    WtMenuCtr & operator = (const WtMenuCtr &);

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
        WtGameModeIf* mode = GAME_MODE_CTR.get_available_modes()[idx];
        WtSettings settings = STORAGE.get_settings();
        
        if ( settings.game_mode != mode->get_id_string() )
        {
            settings.game_mode = mode->get_id_string();
            STORAGE.store_settings( settings );
        }

        m_game_ctr.set_game_mode( mode );
        enter_child_menu( m_game_ctr );
    }



private:
    WtCoord          m_drag_start_pos;
    bool             m_was_drag;
    uint16_t         m_drag_button_id;

    WtMenuSettings   m_settings;
    WtMenuHighscores m_scores;
    WtGameCtr        m_game_ctr;

    WtButton         m_score_btn;
    WtButton         m_setting_btn;

    WtHorizontalCarousel m_game_selection;
};

#endif /* _WT_MENU_CTR_H_ */
