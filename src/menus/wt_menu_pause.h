/*!*****************************************************************************
 * @file wt_menu_pause.h
 * @brief menu class for pause mode
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
#ifndef _WT_MENU_PAUSE_H_
#define _WT_MENU_PAUSE_H_

#include "wt_view_if.h"
//#include "menus/wt_menu_help.h"
#include "widgets/wt_checkbox_button.h"
#include "wt_game_mode_if.h"

class WtMenuPause : public WtViewIf
{
public:
    using OnQuitDelegate = std::function<void(void)>;
    using OnRestartDelegate = std::function<void(void)>;
private:
    static const size_t offset_x = (ACTIVE_WINDOW_WIDTH / 2);
    static const ssize_t offset_y = (ACTIVE_WINDOW_HEIGHT / 8);
           const WtDim  m_standard_btn_size = WtDim( 500, 69 );
public:
    WtMenuPause( OnRestartDelegate restart_handler,
                 OnQuitDelegate    quit_handler ) :
        WtViewIf( "#202020", 0, 0 ),
        //m_help(),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     WtDim(138, 124),
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuPause::leave ),
                     WtL10n_tr( "B A C K"),
                     WtCoord( 0, 25 ),
                     WtFont( "#a8a8a8", "text_big" ) ),
        m_redo_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 4) - (138 / 2), (ACTIVE_WINDOW_HEIGHT / 2)+100 ), 
                    WtDim(138, 124), 
                    "redo_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::restart_pressed ),
                    WtL10n_tr("RESTART"),
                    WtCoord( 0, 40 ),
                    WtFont( "#a8a8a8", "text_big" ) ),
        m_quit_btn( WtCoord(ACTIVE_WINDOW_WIDTH - ((ACTIVE_WINDOW_WIDTH / 4) + (138 / 2)), (ACTIVE_WINDOW_HEIGHT / 2)+100),
                    WtDim(138, 124), 
                    "quit_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::quit_pressed ),
                    WtL10n_tr("EXIT GAME"),
                    WtCoord( 0, 40 ),
                    WtFont( "#a8a8a8", "text_big" ) ),
        m_supporting_grid_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + 150 + (69-20) ),
                               WtL10n_tr( "Supporting Grid" ),
                               STORAGE.get_settings().show_support_grid,
                               WT_BIND_EVENT_HANDLER_1( WtMenuPause::supporting_grid_changed ) ),
        m_immediate_error_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + 150 + (2*69- 20) ),
                               WtL10n_tr( "Show errors immediately" ),
                               STORAGE.get_settings().show_error_on_input,
                               WT_BIND_EVENT_HANDLER_1( WtMenuPause::show_error_changed ) ),
        m_pause_label( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (297 / 2), (ACTIVE_WINDOW_HEIGHT / 8) ),
                       WtDim( 297, 74 ),
                       "label_pause.bmp",
                       [](){} ),
        m_restart_handler( restart_handler ),
        m_quit_handler( quit_handler )
    {
        add_button( m_leave_btn );
        add_button( m_redo_btn );
        add_button( m_quit_btn );
        add_button( m_supporting_grid_btn );
        add_button( m_immediate_error_btn );
        add_button( m_pause_label );
    }

    ~WtMenuPause()
    {
    }
private: // no copy allowed
    WtMenuPause( const WtMenuPause& ); 
    WtMenuPause & operator = (const WtMenuPause &);


    /**************************
     *
     *************************/
    void restart_pressed()
    {
        if ( m_restart_handler ) m_restart_handler();
        leave();
    }

    /**************************
     *
     *************************/
    void quit_pressed()
    {
        if ( m_quit_handler ) m_quit_handler();
        leave();
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
     * signal
     *************************/
    void entered_view()
    {
        m_supporting_grid_btn.set_checked( STORAGE.get_settings().show_support_grid );
    }

private:
    WtButton            m_leave_btn;
    WtButton            m_redo_btn;
    WtButton            m_quit_btn;
    WtCheckboxButton    m_supporting_grid_btn;
    WtCheckboxButton m_immediate_error_btn;
    WtButton            m_pause_label;

    OnRestartDelegate   m_restart_handler;
    OnQuitDelegate      m_quit_handler;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
