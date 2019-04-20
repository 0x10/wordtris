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

#include "wt_menu_if.h"
#include "wt_menu_help.h"
#include "wt_game_mode_if.h"
#include "wt_game_ctr.h"

class WtMenuPause : public WtMenuIf
{
private:
    static const size_t offset_x = (ACTIVE_WINDOW_WIDTH / 2);
    static const size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (69+20);
           const WtDim  m_standard_btn_size = WtDim( 328, 69 );
public:
    WtMenuPause() :
        WtMenuIf( 0x200, "bg_menu_pause.bmp", false ),
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
                          WT_BIND_EVENT_HANDLER_1( WtMenuPause::show_next_stone_changed ) )
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
        GAME_CTR.restart();
        leave();
    }

    /**************************
     *
     *************************/
    void quit_pressed()
    {
        GAME_CTR.quit();
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
    WtMenuHelp m_help;

    WtButton m_leave_btn;
    WtButton m_redo_btn;
    WtButton m_quit_btn;
    WtButton m_help_btn;
    WtCheckboxButton m_supporting_grid_btn;
    WtCheckboxButton m_next_stone_btn;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
