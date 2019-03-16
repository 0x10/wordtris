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

#include "wt_menu_game_mode_select.h"

class WtMenuSettings : public WtMenuIf
{
public:
    WtMenuSettings() :
        WtMenuIf( 0x300, "bg_menu_settings.bmp" ),
        m_current_diff(0)
    {
        size_t offset_x = (ACTIVE_WINDOW_WIDTH - 328) / 2;
    #if 0
        size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (ACTIVE_WINDOW_HEIGHT / 4 )+ 80;

        add_button( 1, WtCoord(offset_x, offset_y), WtDim(500, 80), WtL10n::tr("language: ").append( WtL10n::get_language_code() ), false );
        add_button( 2, WtCoord(offset_x, offset_y+80+40), WtDim(500, 80), WtL10n::tr("difficulty: ").append( WtGameModeIf::get_available_difficulties()[m_current_diff].second ), false );

        #endif
        
        add_button( WtButton( 3, WtCoord( offset_x, (ACTIVE_WINDOW_HEIGHT / 2)-69/2 ), WtDim(328, 69), "menu_btn.bmp", WtL10n::tr("choose game mode") ) );
        add_button( WtButton( 4, WtCoord( 105, 800 ), WtDim(100, 100), "back_btn.bmp" ) );
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

        switch( TO_BUTTON_ID( id ) )
        {
            case 1:
                WtL10n::set_language( WtL10n::get_next_language_code() );
                language_changed = true;
                break;
            case 2:
                m_current_diff = ( m_current_diff + 1 ) % WtGameModeIf::get_available_difficulties().size();
                break;
            case 3:
                enter_child_menu( m_select_mode );
                break;
            case 4:
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
