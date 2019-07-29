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
        WtViewIf( "#122339", false ),
        //m_help(),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     WtDim(138, 124),
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuPause::leave ),
                     WtL10n_tr( "B A C K"),
                     WtCoord( -20, 25 ),
                     WtFont( "#498fe1", "text_big" ) ),
        m_redo_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 4) - (138 / 2), (ACTIVE_WINDOW_HEIGHT / 2)+100 ), 
                    WtDim(138, 124), 
                    "redo_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::restart_pressed ),
                    WtL10n_tr("RESTART"),
                    WtCoord( -20, 40 ),
                    WtFont( "#498fe1", "text_big" ) ),
        m_quit_btn( WtCoord(ACTIVE_WINDOW_WIDTH - ((ACTIVE_WINDOW_WIDTH / 4) + (138 / 2)), (ACTIVE_WINDOW_HEIGHT / 2)+100),
                    WtDim(138, 124), 
                    "quit_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::quit_pressed ),
                    WtL10n_tr("EXIT GAME"),
                    WtCoord( -20, 40 ),
                    WtFont( "#498fe1", "text_big" ) ),
        m_help_btn( WtCoord(offset_x - 158, offset_y+50),
                    WtDim(100, 100),
                    "help_btn.bmp",
                    WT_BIND_EVENT_HANDLER( WtMenuPause::help_pressed ) ),
        m_seperator0( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + 150 ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b" ),
        m_supporting_grid_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + 150 + (69-20) ),
                               WtL10n_tr( "Supporting Grid" ),
                               STORAGE.get_settings().show_support_grid,
                               WT_BIND_EVENT_HANDLER_1( WtMenuPause::supporting_grid_changed ) ),
        m_next_stone_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + 150 + ((69*2)) ),
                          WtL10n_tr( "Show next stone" ),
                          STORAGE.get_settings().show_next_stone,
                          WT_BIND_EVENT_HANDLER_1( WtMenuPause::show_next_stone_changed ) ),
        m_enable_audio_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) + ((m_standard_btn_size.w / 2)-100), offset_y + 150 + ((69*3)+20) ),
                            WtL10n_tr( "Play music and sounds" ),
                            STORAGE.get_settings().enable_audio,
                            WT_BIND_EVENT_HANDLER_1( WtMenuPause::enable_audio_changed ) ),
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
        //add_button( m_seperator0 );
        //add_button( m_help_btn );
        add_button( m_supporting_grid_btn );
        add_button( m_next_stone_btn );
        add_button( m_enable_audio_btn );
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
    void help_pressed()
    {
        //enter_child_menu( m_help );
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

private:
    //WtMenuHelp          m_help;

    WtButton            m_leave_btn;
    WtButton            m_redo_btn;
    WtButton            m_quit_btn;
    WtButton            m_help_btn;
    WtButton            m_seperator0;
    WtCheckboxButton    m_supporting_grid_btn;
    WtCheckboxButton    m_next_stone_btn;
    WtCheckboxButton    m_enable_audio_btn;
    WtButton            m_pause_label;

    OnRestartDelegate   m_restart_handler;
    OnQuitDelegate      m_quit_handler;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
