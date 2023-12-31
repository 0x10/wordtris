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
        WtViewIf( "#172d4a", -1, -1, WtTime::TimeType(0), WT_BIND_EVENT_HANDLER_1( WtMenuCtr::handle_key_press ) ),
        m_drag_start_pos(0,0),
        m_was_drag(false),
        m_drag_button_id(0),
        m_settings(),
        m_scores(),
        m_game_ctr(),
        m_score_btn( WtCoord( 24, (80-57)/2 ),
                     WtDim( 80, 57 ),
                     "label_score.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuCtr::enter_score_menu ) ),
        m_setting_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-65)-24, (80-65)/2 ),
                       WtDim( 65, 65 ),
                       "settings_logo.bmp",
                       WT_BIND_EVENT_HANDLER( WtMenuCtr::enter_settings_menu ) ),
        m_start_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ),
                     WtDim( 138, 124 ),
                     "next_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuCtr::game_mode_selected ),
                     WtL10n_tr( "S T A R T"),
                     WtCoord( 0, 25 ),
                     WtFont( "#498fe1", "text_big" ) ),
        m_select_label( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (366 / 2), (ACTIVE_WINDOW_HEIGHT / 4)-(70/2) ),
                       WtDim( 366, 70 ),
                       "label_select_bbfix.bmp",
                       [](){} ),
        m_settings_bg( WtCoord( 0, 0 ),
                       WtDim( ACTIVE_WINDOW_WIDTH, 80 ),
                       "#112238",
                       [](){} ),
        m_game_selection( WtCoord( 0, (ACTIVE_WINDOW_HEIGHT / 2) - (297 / 2) ),
                          WtDim( ACTIVE_WINDOW_WIDTH, 297 ),
                          GAME_MODE_CTR.get_available_mode_titles(),
#ifdef WT_PRO_MODE
                          GAME_MODE_CTR.mode_idx_from_string( STORAGE.get_settings().game_mode ),
#else
                          GAME_MODE_CTR.mode_idx_from_string( "WordtrisClassic" ),                     
#endif
                          [](size_t){},
                          false )
    {
        WtL10n::register_lang_change_obsever( WT_BIND_EVENT_HANDLER( WtMenuCtr::language_changed ) );

        ACTIVE_INPUT.register_key_press_delegate( WT_BIND_EVENT_HANDLER_1( WtMenuCtr::on_key_press ) );
        m_game_ctr.set_game_mode( GAME_MODE_CTR.mode_from_string( STORAGE.get_settings().game_mode ) );

        add_button( m_settings_bg );
        add_button( m_score_btn );
        add_button( m_setting_btn );
        add_button( m_start_btn );
        add_button( m_select_label );
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
        set_fading( 1, 1 );
        enter_child_menu( m_scores );
        set_fading( -1, -1 );
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
    void language_changed()
    {
        m_game_selection.set_labels(GAME_MODE_CTR.get_available_mode_titles());
    }

    /**************************
     *
     *************************/
    void game_mode_selected()
    {
    	size_t idx = m_game_selection.selected();
        std::cout << "selected = " << idx << std::endl;
        WtGameModeIf* mode = GAME_MODE_CTR.get_available_modes()[idx];

        if ( mode->get_id_string() != "LOCKED" )
        {
	        WtSettings settings = STORAGE.get_settings();
	        
	        if ( settings.game_mode != mode->get_id_string() )
	        {
	            settings.game_mode = mode->get_id_string();
	            STORAGE.store_settings( settings );
	        }

	        m_game_ctr.set_game_mode( mode );
	        enter_child_menu( m_game_ctr );
    	}
    	else
    	{
    		//TODO show popup
    	}
    }

    /**************************
     *
     *************************/
    void handle_key_press( wt_control key )
    {
        if ( key == wt_control_BACK )
        {
            do_exit();
        }
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
    WtButton         m_start_btn;
    WtButton         m_select_label;
    WtButton         m_settings_bg;

    WtHorizontalCarousel m_game_selection;
};

#endif /* _WT_MENU_CTR_H_ */
