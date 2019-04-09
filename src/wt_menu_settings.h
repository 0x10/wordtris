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
#include "wt_menu_game_mode_select.h"

#include "wt_tristate_button.h"


class WtMenuSettings : public WtMenuIf
{
private:
    static const size_t offset_x = (ACTIVE_WINDOW_WIDTH - 328) / 2;
    static const size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (ACTIVE_WINDOW_HEIGHT / 4);
public:
    WtMenuSettings() :
        WtMenuIf( 0x300, "bg_menu_settings.bmp" ),
        m_current_diff(0),
        m_leave_btn( WtCoord( 105, 800 ), 
                     WtDim(100, 100), 
                     "back_btn.bmp",
                     std::bind ( &WtMenuSettings::leave, this ) ),
        m_select_mode_btn( WtCoord( offset_x, offset_y ), 
                           WtDim(328, 69), 
                           "menu_btn.bmp", 
                           std::bind ( &WtMenuSettings::select_pressed, this ),
                           WtL10n::tr("select game mode") ),
        m_lang_select_btn( WtCoord( offset_x, offset_y + 69 + 20 ),
                           WtDim( 328, 69 ),
                           WtL10n::available_languages(),
                           0,
                           std::bind ( &WtMenuSettings::lang_changed, this, std::placeholders::_1 ) ),
        m_diff_select_btn( WtCoord( offset_x, offset_y + (69 + 20)*2 ),
                           WtDim( 328, 69 ),
                           std::vector<std::string>({ WtGameModeIf::get_available_difficulties()[0].second,
                             WtGameModeIf::get_available_difficulties()[1].second,
                             WtGameModeIf::get_available_difficulties()[2].second }),
                           0,
                           std::bind ( &WtMenuSettings::diff_changed, this, std::placeholders::_1 ) )
    {
        for ( size_t idx = 0; idx < WtL10n::available_languages().size(); idx++ )
        {
            if ( WtL10n::available_languages()[idx] == WtL10n::get_language_code() )
            {
                m_lang_select_btn.select( (uint8_t) idx );
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


        add_button( m_leave_btn );
        add_button( m_select_mode_btn );
        add_tristate_button( m_lang_select_btn );
        add_tristate_button( m_diff_select_btn );
#if 0

        {
                std::vector< std::pair<uint16_t, std::string> > labeled_ids = {
                        std::make_pair( 6, WtGameModeIf::get_available_difficulties()[0].second ),
                        std::make_pair( 7, WtGameModeIf::get_available_difficulties()[1].second ),
                        std::make_pair( 8, WtGameModeIf::get_available_difficulties()[2].second ),
                };

                size_t selected_id = 0;
                switch( STORAGE.get_settings().difficulty )
                {
                    default: break;
                    case wt_difficulty_EASY:   selected_id = 0; break;
                    case wt_difficulty_MEDIUM: selected_id = 1; break;
                    case wt_difficulty_HARD:   selected_id = 2; break;
                }
                m_current_diff = selected_id;

                add_radio_group_button( labeled_ids,
                                        WtCoord( offset_x, offset_y + (69 + 20)*2 ),
                                        WtDim( 328, 69 ),
                                        labeled_ids[selected_id].first );
        }

        {
                size_t selected_id = 9;
                m_themes = {
                        std::make_pair( 9, "light" ),
                        std::make_pair( 10, "dark" ),
                        std::make_pair( 11, "mono" ),
                };
                if ( STORAGE.get_settings().active_theme != "default" )
                {
                    for ( size_t l_idx = 0; l_idx < m_themes.size(); l_idx++ )
                    {
                        if ( m_themes[l_idx].second == STORAGE.get_settings().active_theme )
                        {
                            selected_id = m_themes[l_idx].first;
                            break;
                        }
                    }
                }
                add_radio_group_button( m_themes,
                                        WtCoord( offset_x, offset_y + (69 + 20)*3 ),
                                        WtDim( 328, 69 ),
                                        selected_id );
        }
#endif
    }

    ~WtMenuSettings()
    {
    }
private: // no copy allowed
    WtMenuSettings( const WtMenuSettings& ); 
    WtMenuSettings & operator = (const WtMenuSettings &);


public:
    /**************************
     *
     *************************/
    virtual void listen( WtSettingsChangeObserver* listener )
    {
        WtMenuIf::listen( listener );
        m_select_mode.listen( listener );
    }

private:
    /**************************
     *
     *************************/
    void select_pressed()
    {
        enter_child_menu( m_select_mode );
    }

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
        if ( WtL10n::get_language_code() != WtL10n::available_languages()[lang_idx] )
        {
            WtL10n::set_language( WtL10n::available_languages()[lang_idx] );
            for( size_t idx = 0; idx < get_listener().size(); idx++ )
            {
                get_listener()[idx]->notify_language_changed( WtL10n::get_language_code() );
            }
        }
    }

    #if 0
    /**************************
     *
     *************************/
    virtual void notify_button_pressed( uint16_t id )
    {
        size_t old_diff = m_current_diff;
        bool language_changed = false;
        bool theme_changed = false;
        size_t theme_idx = 0;

        std::cout << "button pressed = " << id << std::endl;

        switch( TO_BUTTON_ID( id ) )
        {
            case 3:
            case 4:
            case 5:
                {
                    size_t lang_idx = TO_BUTTON_ID(id)-3;
                    if ( WtL10n::get_language_code() != WtL10n::available_languages()[lang_idx] )
                    {
                        WtL10n::set_language( WtL10n::available_languages()[lang_idx] );
                        language_changed = true;

                        modify_radio_group_button( 3, 3, TO_BUTTON_ID(id) );
                    }
                }
                break;
            case 6:
            case 7:
            case 8:
                {
                    size_t diff_idx = TO_BUTTON_ID(id)-6;
                    if ( STORAGE.get_settings().difficulty != WtGameModeIf::get_available_difficulties()[diff_idx].first )
                    {
                        m_current_diff = diff_idx;
                        modify_radio_group_button( 6, 3, TO_BUTTON_ID(id) );
                    }
                }
                break;
            case 9:
            case 10:
            case 11:
                {
                    theme_idx = TO_BUTTON_ID(id)-9;
                    
                    if ( STORAGE.get_settings().active_theme != m_themes[theme_idx].second )
                        theme_changed = true;

                    modify_radio_group_button( 9, m_themes.size(), TO_BUTTON_ID(id) );
                }
                break;
                break;
            case 1:
                break;
            case 2:
                leave();
                break;
            default: /*ignore*/ break;
        }

        for( size_t idx = 0; idx < get_listener().size(); idx++ )
        {
            if ( old_diff != m_current_diff )
                get_listener()[idx]->notify_difficulty_changed( WtGameModeIf::get_available_difficulties()[m_current_diff].first );
            if ( language_changed )
                get_listener()[idx]->notify_language_changed( WtL10n::get_language_code() );
            if ( theme_changed )
                get_listener()[idx]->notify_theme_changed( m_themes[theme_idx].second );
        }
    }
#endif
private:
    size_t                                          m_current_diff;
    WtMenuSelectMode                                m_select_mode;
    std::vector< std::pair<uint16_t, std::string> > m_themes;
    WtButton                                        m_leave_btn;
    WtButton                                        m_select_mode_btn;
    WtTriStateButton                                m_lang_select_btn;
    WtTriStateButton                                m_diff_select_btn;
};

#endif /* _WT_MENU_SETTINGS_H_ */
