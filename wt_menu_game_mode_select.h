/*!*****************************************************************************
 * @file wt_menu_game_mode_select.h
 * @brief menu class for selection of game mode
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
#ifndef _WT_MENU_GAME_MODE_SELECT_H_
#define _WT_MENU_GAME_MODE_SELECT_H_

#include "wt_menu_if.h"
#include "wt_game_mode_ctr.h"

class WtMenuSelectMode : public WtMenuIf
{
public:
    WtMenuSelectMode() :
        WtMenuIf( 0x200, "bg_menu_settings.bmp" )
    {
        size_t offset_x = (ACTIVE_WINDOW_WIDTH - 328) / 2;
        size_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (ACTIVE_WINDOW_HEIGHT / 4 );

        std::vector<WtGameModeIf*>& available_modes = GAME_MODE_CTR.get_available_modes();

        size_t idx = 0;
        std::vector<std::string> modes;

        for( idx = 0; idx < available_modes.size(); idx++ )
        {
            modes.push_back( available_modes[idx]->get_title() );
        }

        add_list( WtCoord( offset_x, offset_y ),
                  WtDim( 328, 69 ),
                  "menu_btn.bmp",
                  modes );

        add_button( WtButton( idx, WtCoord( 105, 800 ), WtDim(100, 100), "back_btn.bmp" ) );
    }

    ~WtMenuSelectMode()
    {
    }
private: // no copy allowed
    WtMenuSelectMode( const WtMenuSelectMode& ); 
    WtMenuSelectMode & operator = (const WtMenuSelectMode &);


    /**************************
     *
     *************************/
    virtual void notify_button_pressed( uint16_t id )
    {
        uint8_t real_id = TO_BUTTON_ID( id );

        std::vector<WtGameModeIf*>& available_modes = GAME_MODE_CTR.get_available_modes();
        
        if ( real_id < available_modes.size() )
        {
            for( size_t idx = 0; idx < get_listener().size(); idx++ )
            {
                get_listener()[idx]->notify_game_mode_changed( available_modes[real_id] );
            }
        }

        leave();
    }
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
