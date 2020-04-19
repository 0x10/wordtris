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

#include "menus/wt_menu_credits.h"

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
        WtViewIf("#202020", 1, 1 ),
        m_current_diff(0),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     WtDim(138, 124), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuSettings::leave ),
                     WtL10n_tr( "B A C K"),
                     WtCoord( 0, 25 ),
                     WtFont( "#a8a8a8", "text_big" ) ),
        m_lang_select_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - ((74+74+74+(m_standard_btn_size.w/3-74)*2) / 2), offset_y + (m_standard_btn_size.h*5)/2 ),
                           m_standard_btn_size,
                           0,
                           WT_BIND_EVENT_HANDLER_1( WtMenuSettings::lang_changed ),
                           WtL10n::get_available_languages(),
                           WtL10n::get_selected_language_names(),
                           WtL10n::get_available_language_names(),
                           WtDim( 74,58 ) ),
        m_supporting_grid_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + (m_standard_btn_size.h *6 ) - 20 ),
                               WtL10n_tr( "Show supporting grid" ),
                               STORAGE.get_settings().show_support_grid,
                               WT_BIND_EVENT_HANDLER_1( WtMenuSettings::supporting_grid_changed ) ),
        m_immediate_error_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + (m_standard_btn_size.h *7 ) - 20 ),
                               WtL10n_tr( "Show errors immediately" ),
                               STORAGE.get_settings().show_error_on_input,
                               WT_BIND_EVENT_HANDLER_1( WtMenuSettings::show_error_changed ) ),

        m_seperator0( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (m_standard_btn_size.h * 5) ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a2a2a2" ),
        m_settings_logo( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (134 / 2), 128 ),
                       WtDim( 134, 134 ),
                       "settings_logo.bmp",
                       [](){} ),
        m_info_btn( WtCoord( (ACTIVE_WINDOW_WIDTH - 60) - 10, 10 ), WtDim(60,60), "icon_info.bmp", WT_BIND_EVENT_HANDLER( WtMenuSettings::enter_credits )),
        m_credits_menu()
    {
        for ( size_t idx = 0; idx < WtL10n::get_available_languages().size(); idx++ )
        {
            if ( WtL10n::get_available_languages()[idx] == WtL10n::get_language_code() )
            {
                m_lang_select_btn.select( idx );
                break;
            }
        }


        add_button( m_leave_btn );
        add_tristate_button( m_lang_select_btn );
        add_button( m_supporting_grid_btn );
        add_button( m_immediate_error_btn );
        add_button( m_seperator0 );
        add_button( m_settings_logo );
        add_button( m_info_btn );
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
    void show_error_changed( bool enable )
    {
        std::cout << "show error " << ( enable ? "active" : "inactive" ) << std::endl;
        WtSettings settings = STORAGE.get_settings();
        if ( settings.show_error_on_input != enable )
        {
            settings.show_error_on_input = enable;
            STORAGE.store_settings( settings );
        }
    }
    
    
    /**************************
     *
     *************************/
    void enter_credits()
    {
        set_fading( -1, -1 );
        enter_child_menu( m_credits_menu );
        set_fading( 1, 1 );
    }

    /**************************
     * signal
     *************************/
    void entered_view()
    {
        m_supporting_grid_btn.set_checked( STORAGE.get_settings().show_support_grid );
    }

    /**************************
     * signal
     *************************/
    virtual void update_view()
    {

    }

private:
    size_t           m_current_diff;
    WtButton         m_leave_btn;
    WtTriStateButton m_lang_select_btn;
    WtCheckboxButton m_supporting_grid_btn;
    WtCheckboxButton m_immediate_error_btn;
    WtButton         m_seperator0;
    WtButton         m_settings_logo;
    WtButton         m_info_btn;
    WtMenuCredits    m_credits_menu;
};

#endif /* _WT_MENU_SETTINGS_H_ */
