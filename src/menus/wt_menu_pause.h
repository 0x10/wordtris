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
#include "menus/wt_menu_help.h"
#include "widgets/wt_checkbox_button.h"
#include "wt_game_mode_if.h"

class WtMenuPause : public WtViewIf
{
public:
    using OnQuitDelegate = std::function<void(void)>;
    using OnRestartDelegate = std::function<void(void)>;
private:
    static const size_t offset_x = (ACTIVE_WINDOW_WIDTH / 2);
    static const size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (69+20);
           const WtDim  m_standard_btn_size = WtDim( 328, 69 );
public:
    WtMenuPause( OnRestartDelegate restart_handler,
                 OnQuitDelegate    quit_handler ) :
        WtViewIf( "bg_menu_pause.bmp", false ),
        m_help(),
        m_leave_btn( WtCoord(offset_x - 158, offset_y-100), 
                     WtDim(100, 100),
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuPause::leave ) ),
        m_redo_btn( WtCoord(offset_x + 58, offset_y-100), 
                    WtDim(100, 100),
                    "redo_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::restart_pressed ) ),
        m_quit_btn( WtCoord(offset_x + 58, offset_y+50),
                    WtDim(100, 100),
                    "quit_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::quit_pressed ) ),
        m_help_btn( WtCoord(offset_x - 158, offset_y+50),
                    WtDim(100, 100),
                    "help_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::help_pressed ) ),
        m_supporting_grid_btn( WtCoord( offset_x-164, offset_y + (20+((69 + 20)*2)) ),
                               m_standard_btn_size,
                               WtL10n_tr( "Supporting Grid" ),
                               STORAGE.get_settings().show_support_grid,
                               WT_BIND_EVENT_HANDLER_1( WtMenuPause::supporting_grid_changed ) ),
        m_next_stone_btn( WtCoord( offset_x-164, offset_y + (20+((69 + 20)*3)) ),
                          m_standard_btn_size,
                          WtL10n_tr( "Show next stone" ),
                          STORAGE.get_settings().show_next_stone,
                          WT_BIND_EVENT_HANDLER_1( WtMenuPause::show_next_stone_changed ) ),
        m_restart_handler( restart_handler ),
        m_quit_handler( quit_handler )
    {
        add_button( m_leave_btn );
        add_button( m_redo_btn );
        add_button( m_quit_btn );
        add_button( m_help_btn );
        add_button( m_supporting_grid_btn );
        add_button( m_next_stone_btn );
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
    void help_pressed()
    {
        enter_child_menu( m_help );
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
     * signal
     *************************/
    void entered_view()
    {
        m_supporting_grid_btn.set_checked( STORAGE.get_settings().show_support_grid );
        m_next_stone_btn.set_checked( STORAGE.get_settings().show_next_stone );
    }

private:
    WtMenuHelp          m_help;

    WtButton            m_leave_btn;
    WtButton            m_redo_btn;
    WtButton            m_quit_btn;
    WtButton            m_help_btn;
    WtCheckboxButton    m_supporting_grid_btn;
    WtCheckboxButton    m_next_stone_btn;

    OnRestartDelegate   m_restart_handler;
    OnQuitDelegate      m_quit_handler;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
