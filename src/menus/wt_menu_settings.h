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

#include "wt_menu_if.h"
#include "wt_storage.h"

#include "widgets/wt_tristate_button.h"
#include "widgets/wt_checkbox_button.h"


class WtMenuSettings : public WtMenuIf
{
private:
    static const size_t offset_x = (ACTIVE_WINDOW_WIDTH - 328) / 2;
    static const size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 8);

    const WtDim                      m_standard_btn_size = WtDim( 328, 69 );
    const std::array<const char*, 3> m_selectable_themes { "light",
                                                           "dark",
                                                           "mono" };

public:
    WtMenuSettings() :
        WtMenuIf( 0x300, "bg_menu_settings.bmp" ),
        m_current_diff(0),
        m_leave_btn( WtCoord( 105, 800 ), 
                     WtDim(100, 100), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuSettings::leave ) ),
        m_lang_select_btn( WtCoord( offset_x, offset_y + 69 + 20 ),
                           m_standard_btn_size,
                           WtL10n::get_available_languages(),
                           0,
                           WT_BIND_EVENT_HANDLER_1( WtMenuSettings::lang_changed ) ),
        m_diff_select_btn( WtCoord( offset_x, offset_y + (69 + 20)*2 ),
                           m_standard_btn_size,
                           std::array<const char*, 3>{{ WtGameModeIf::get_available_difficulties()[0].second,
                                                        WtGameModeIf::get_available_difficulties()[1].second,
                                                        WtGameModeIf::get_available_difficulties()[2].second }},
                           0,
                           WT_BIND_EVENT_HANDLER_1( WtMenuSettings::diff_changed ) ),
        m_theme_select_btn( WtCoord( offset_x, offset_y + (69 + 20)*3 ),
                            m_standard_btn_size,
                            m_selectable_themes,
                            0,
                            WT_BIND_EVENT_HANDLER_1( WtMenuSettings::theme_changed ) ),
        m_supporting_grid_btn( WtCoord( offset_x, offset_y + (69 + 20)*4 ),
                               m_standard_btn_size,
                               WtL10n_tr( "Supporting Grid" ),
                               STORAGE.get_settings().show_support_grid,
                               WT_BIND_EVENT_HANDLER_1( WtMenuSettings::supporting_grid_changed ) ),
        m_next_stone_btn( WtCoord( offset_x, offset_y + (69 + 20)*5 ),
                          m_standard_btn_size,
                          WtL10n_tr( "Show next stone" ),
                          STORAGE.get_settings().show_next_stone,
                          WT_BIND_EVENT_HANDLER_1( WtMenuSettings::show_next_stone_changed ) )

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


        add_button( m_leave_btn );
        add_tristate_button( m_lang_select_btn );
        add_tristate_button( m_diff_select_btn );
        add_tristate_button( m_theme_select_btn );
        add_button( m_supporting_grid_btn );
        add_button( m_next_stone_btn );
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
        if ( STORAGE.get_settings().difficulty != WtGameModeIf::get_available_difficulties()[diff_idx].first )
        {
            for( size_t idx = 0; idx < get_listener().size(); idx++ )
            {
                get_listener()[idx]->notify_difficulty_changed( WtGameModeIf::get_available_difficulties()[diff_idx].first );
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
            for( size_t idx = 0; idx < get_listener().size(); idx++ )
            {
                get_listener()[idx]->notify_theme_changed( std::string( m_selectable_themes[id] ) );
            }
        }
    }

    /**************************
     *
     *************************/
    void supporting_grid_changed( bool show_grid )
    {
        for( size_t idx = 0; idx < get_listener().size(); idx++ )
        {
            get_listener()[idx]->notify_supporting_grid_changed( show_grid );
        }
    }

    /**************************
     *
     *************************/
    void show_next_stone_changed( bool show_next )
    {
        for( size_t idx = 0; idx < get_listener().size(); idx++ )
        {
            get_listener()[idx]->notify_show_next_stone_changed( show_next );
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
};

#endif /* _WT_MENU_SETTINGS_H_ */
