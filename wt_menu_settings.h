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

#include "wt_l10n.h"
#include "wt_game_mode_if.h"
#include "wt_menu_if.h"
#include "wt_storage.h"
#include "wt_menu_game_mode_select.h"

class WtMenuSettings : public WtMenuIf
{
public:
    WtMenuSettings() :
        WtMenuIf( 0x300, "bg_menu_settings.bmp" ),
        m_current_diff(0)
    {
        size_t offset_x = (ACTIVE_WINDOW_WIDTH - 328) / 2;
        size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (ACTIVE_WINDOW_HEIGHT / 4);


        {
                const uint16_t button_ids[3] = { 1,2,3 };
                size_t selected_id = 0;
                for ( size_t idx = 0; idx < WtL10n::available_languages().size(); idx++ )
                {
                    if ( WtL10n::available_languages()[idx] == WtL10n::get_language_code() )
                    {
                        selected_id = button_ids[idx];
                        break;
                    }
                }
                
                const std::string label[3] = { WtL10n::available_languages()[0],
                                               WtL10n::available_languages()[1],
                                               WtL10n::available_languages()[2] };

                add_tri_state_button( button_ids, label,
                                      WtCoord( offset_x, offset_y + 69 + 20 ),
                                      WtDim( 328, 69 ),
                                      WtCoord( offset_x+1, offset_y+69+20+1 ),
                                      WtDim( 109, 67 ),
                                      selected_id );
        }

        {
                wt_difficulty diff = STORAGE.get_settings().difficulty;
                size_t selected_id = 0;
                switch( diff )
                {
                    default: break;
                    case wt_difficulty_EASY: selected_id = 0; break;
                    case wt_difficulty_MEDIUM: selected_id = 1; break;
                    case wt_difficulty_HARD: selected_id = 2; break;
                }

                const std::string label[3] = { WtGameModeIf::get_available_difficulties()[0].second,
                                               WtGameModeIf::get_available_difficulties()[1].second,
                                               WtGameModeIf::get_available_difficulties()[2].second };
                const uint16_t button_ids[3] = { 4,5,6 };

                add_tri_state_button( button_ids, label,
                                      WtCoord( offset_x, offset_y + (69 + 20)*2 ),
                                      WtDim( 328, 69 ),
                                      WtCoord( offset_x+1, offset_y+(69+20)*2+1 ),
                                      WtDim( 109, 67 ),
                                      button_ids[selected_id] );
        }


        add_button( WtButton( 7, WtCoord( offset_x, offset_y ), WtDim(328, 69), "menu_btn.bmp", WtL10n::tr("select game mode") ) );
        add_button( WtButton( 8, WtCoord( 105, 800 ), WtDim(100, 100), "back_btn.bmp" ) );
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
    virtual void notify_button_pressed( uint16_t id )
    {
        size_t old_diff = m_current_diff;
        bool language_changed = false;

        std::cout << "button pressed = " << id << std::endl;

        switch( TO_BUTTON_ID( id ) )
        {
            case 1:
            case 2:
            case 3:
                if ( WtL10n::get_language_code() != WtL10n::available_languages()[TO_BUTTON_ID(id) - 1] )
                {
                    WtL10n::set_language( WtL10n::available_languages()[TO_BUTTON_ID(id) - 1] );
                    language_changed = true;

                    modify_tri_state_button( 1, TO_BUTTON_ID(id) );
                }
                break;
            case 4:
            case 5:
            case 6:
                if ( STORAGE.get_settings().difficulty != WtGameModeIf::get_available_difficulties()[TO_BUTTON_ID(id) - 4].first )
                {
                    m_current_diff = TO_BUTTON_ID(id) - 4;
                    modify_tri_state_button( 4, TO_BUTTON_ID(id) );
                }
                break;
            case 7:
                enter_child_menu( m_select_mode );
                break;
            case 8:
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
        }
    }

private:
    size_t m_current_diff;
    WtMenuSelectMode m_select_mode;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
