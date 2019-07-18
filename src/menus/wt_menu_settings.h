/*!*****************************************************************************
 * @file wt_menu_settings.h
 * @brief menu class for game settings
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
#ifndef _WT_MENU_SETTINGS_H_
#define _WT_MENU_SETTINGS_H_

#include "wt_view_if.h"
#include "wt_storage.h"

#include "widgets/wt_tristate_button.h"
#include "widgets/wt_checkbox_button.h"


class WtMenuSettings : public WtViewIf
{
private:
    const WtDim                      m_standard_btn_size = WtDim( 500, 69 );

    static const ssize_t offset_x = (ACTIVE_WINDOW_WIDTH / 2) - ((60+60+60+(500/3-60)*2) / 2);
    static const ssize_t offset_y = (ACTIVE_WINDOW_HEIGHT / 8) + (ACTIVE_WINDOW_HEIGHT / 8);

    const std::array<const char*, 3> m_selectable_themes { "light",
                                                           "dark",
                                                           "mono" };

public:
    WtMenuSettings() :
        WtViewIf("#122339"),
        m_current_diff(0),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     //WtDim(100, 100), 
                     WtDim(138, 124), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuSettings::leave ) ),
        m_lang_select_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - ((74+74+74+(m_standard_btn_size.w/3-74)*2) / 2), offset_y + m_standard_btn_size.h/2 ),
                           m_standard_btn_size,
                           0,
                           WT_BIND_EVENT_HANDLER_1( WtMenuSettings::lang_changed ),
                           WtL10n::get_available_languages(),
                           WtL10n::get_selected_language_names(),
                           WtL10n::get_available_language_names(),
                           WtDim( 74,58 ) ),
        m_diff_select_btn( WtCoord( offset_x, offset_y + (m_standard_btn_size.h * 3 ) ),
                           m_standard_btn_size,
                           0,
                           WT_BIND_EVENT_HANDLER_1( WtMenuSettings::diff_changed ),
                           std::array<const char*, 3>{{ WtGameModeIf::get_available_difficulties()[0].second,
                                                        WtGameModeIf::get_available_difficulties()[1].second,
                                                        WtGameModeIf::get_available_difficulties()[2].second }} ),
        m_theme_select_btn( WtCoord( offset_x, offset_y + (m_standard_btn_size.h * 4 ) ),
                            m_standard_btn_size,
                            1,
                            WT_BIND_EVENT_HANDLER_1( WtMenuSettings::theme_changed ),
                            m_selectable_themes ),
        m_supporting_grid_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + (m_standard_btn_size.h *6 ) - 20 ),
                               WtL10n_tr( "Show supporting grid" ),
                               STORAGE.get_settings().show_support_grid,
                               WT_BIND_EVENT_HANDLER_1( WtMenuSettings::supporting_grid_changed ) ),
        m_next_stone_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + (m_standard_btn_size.h * 7 )  ),
                          WtL10n_tr( "Show next stone" ),
                          STORAGE.get_settings().show_next_stone,
                          WT_BIND_EVENT_HANDLER_1( WtMenuSettings::show_next_stone_changed ) ),
        m_enable_audio_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + (m_standard_btn_size.h * 8 ) +20 ),
                            WtL10n_tr( "Play music and sounds" ),
                            STORAGE.get_settings().enable_audio,
                            WT_BIND_EVENT_HANDLER_1( WtMenuSettings::enable_audio_changed ) ),
        m_seperator0( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (m_standard_btn_size.h * 2) + (m_standard_btn_size.h / 2) ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b" ),
        m_seperator1( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (m_standard_btn_size.h * 5)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b" ),
        m_settings_bg( WtCoord( 0, 0 ),
                       WtDim( ACTIVE_WINDOW_WIDTH, 80 ),
                       "#172d4a",
                       [](){} ),
        m_settings_logo( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (133 / 2), 128 ),
                       WtDim( 133, 133 ),
                       "settings_btn.bmp",
                       [](){} )
    {
        for ( size_t idx = 0; idx < WtL10n::get_available_languages().size(); idx++ )
        {
            if ( WtL10n::get_available_languages()[idx] == WtL10n::get_language_code() )
            {
                m_lang_select_btn.select( idx );
                break;
            }
        }

        switch( STORAGE.get_settings().difficulty )
        {
            default: break;
            case wt_difficulty_EASY:
                m_diff_select_btn.select( 0 );
                break;
            case wt_difficulty_MEDIUM:
                m_diff_select_btn.select( 1 );
                break;
            case wt_difficulty_HARD:
                m_diff_select_btn.select( 2 );
                break;
        }

        if ( STORAGE.get_settings().active_theme != "default" )
        {
            for ( size_t l_idx = 0; l_idx < m_selectable_themes.size(); l_idx++ )
            {
                if ( m_selectable_themes[l_idx] == STORAGE.get_settings().active_theme )
                {
                    m_theme_select_btn.select( l_idx );
                    break;
                }
            }
        }

        //add_button( m_settings_bg );
        add_button( m_leave_btn );
        add_tristate_button( m_lang_select_btn );
        add_tristate_button( m_diff_select_btn );
        //add_tristate_button( m_theme_select_btn );
        add_button( m_supporting_grid_btn );
        add_button( m_next_stone_btn );
        add_button( m_enable_audio_btn );
        add_button( m_seperator0 );
        add_button( m_seperator1 );
        add_button( m_settings_logo );
    }

    ~WtMenuSettings()
    {
    }
private: // no copy allowed
    WtMenuSettings( const WtMenuSettings& ); 
    WtMenuSettings & operator = (const WtMenuSettings &);


    /**************************
     *
     *************************/
    void diff_changed( uint8_t id )
    {
        size_t diff_idx = id;
        wt_difficulty diffi = WtGameModeIf::get_available_difficulties()[diff_idx].first;
        WtSettings settings = STORAGE.get_settings();
        if ( settings.difficulty != diffi )
        {
            settings.difficulty = diffi;
            STORAGE.store_settings( settings );

            WtGameModeIf* active_mode = GAME_MODE_CTR.mode_from_string( STORAGE.get_settings().game_mode );
            if ( active_mode != INVALID_GAME_MODE )
            {
                active_mode->set_difficulty( diffi );
            }
        }
    }

    /**************************
     *
     *************************/
    void lang_changed( uint8_t id )
    {
        size_t lang_idx = id;
        if ( WtL10n::get_language_code() != WtL10n::get_available_languages()[lang_idx] )
        {
            // handles own observers
            WtL10n::set_language( WtL10n::get_available_languages()[lang_idx] );
        }
    }

    /**************************
     *
     *************************/
    void theme_changed( uint8_t id )
    {
        if ( STORAGE.get_settings().active_theme != m_selectable_themes[id] )
        {
            ACTIVE_WINDOW.set_theme( m_selectable_themes[id] );

            std::cout << "new theme selected = "<< m_selectable_themes[id] << std::endl;
            WtSettings settings = STORAGE.get_settings();
            settings.active_theme = m_selectable_themes[id];
            STORAGE.store_settings( settings );
        }
    }

    /**************************
     *
     *************************/
    void supporting_grid_changed( bool show_grid )
    {
        std::cout << "grid " << ( show_grid ? "active" : "inactive" ) << std::endl;
        WtSettings settings = STORAGE.get_settings();
        if ( settings.show_support_grid != show_grid )
        {
            settings.show_support_grid = show_grid;
            STORAGE.store_settings( settings );
        }
    }

    /**************************
     *
     *************************/
    void show_next_stone_changed( bool show_next )
    {
        std::cout << "preview " << ( show_next ? "active" : "inactive" ) << std::endl;
        WtSettings settings = STORAGE.get_settings();
        if ( settings.show_next_stone != show_next )
        {
            settings.show_next_stone = show_next;
            STORAGE.store_settings( settings );
        }
    }

    /**************************
     *
     *************************/
    void enable_audio_changed( bool enable )
    {
        std::cout << "audio " << ( enable ? "active" : "inactive" ) << std::endl;
        WtSettings settings = STORAGE.get_settings();
        if ( settings.enable_audio != enable )
        {
            settings.enable_audio = enable;
            STORAGE.store_settings( settings );

            ACTIVE_SFX.toggle_mute( settings.enable_audio );
        }
    }

    /**************************
     * signal
     *************************/
    void entered_view()
    {
        m_supporting_grid_btn.set_checked( STORAGE.get_settings().show_support_grid );
        m_next_stone_btn.set_checked( STORAGE.get_settings().show_next_stone );
        m_enable_audio_btn.set_checked( STORAGE.get_settings().enable_audio );
    }

    /**************************
     * signal
     *************************/
    virtual void update_view()
    {
        std::string credits = "Credits === "
                              "Game Design / Idea === "
                              "Christian Kranz +++ "
                              "Witold Krzeslowski === "
                              "Coding === "
                              "Christian Kranz === "
                              "Android Support === "
                              "Witold Krzeslowski === "
                              "UI Design === "
                              "Svenja Dittrich === "
                              "Music === "
                              "LittleRobotSoundFactory @ freesound.org +++ "
                              "Valo @ freesound.org +++ "
                              "Christian Kranz ==="
                              "        "
                              "Special thanks to: "
                              "       A.          "
                              " and all family and friends."
                              "                   ";
        static int32_t pos = ACTIVE_WINDOW_WIDTH;
        if ( ACTIVE_WINDOW.draw_sine_scroller_text( credits, WtCoord( pos, 60 ) ) )
        {
            pos = ACTIVE_WINDOW_WIDTH;
        }
        else
        {
            pos = (pos - 1);
        }
    }

private:
    size_t           m_current_diff;
    WtButton         m_leave_btn;
    WtTriStateButton m_lang_select_btn;
    WtTriStateButton m_diff_select_btn;
    WtTriStateButton m_theme_select_btn;
    WtCheckboxButton m_supporting_grid_btn;
    WtCheckboxButton m_next_stone_btn;
    WtCheckboxButton m_enable_audio_btn;
    WtButton         m_seperator0;
    WtButton         m_seperator1;
    WtButton         m_settings_bg;
    WtButton         m_settings_logo;
};

#endif /* _WT_MENU_SETTINGS_H_ */
