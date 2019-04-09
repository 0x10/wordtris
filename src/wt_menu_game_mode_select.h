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
        WtMenuIf( 0x200, "bg_menu_settings.bmp" ),
        m_leave_btn( WtCoord( 105, 800 ), 
                     WtDim(100, 100), 
                     "back_btn.bmp",
                     std::bind ( &WtMenuSelectMode::leave, this ) )
    {
        ssize_t offset_x = (ACTIVE_WINDOW_WIDTH - 328) / 2;
        ssize_t offset_y = (ACTIVE_WINDOW_HEIGHT / 2) - (ACTIVE_WINDOW_HEIGHT / 4 );

        std::vector<WtGameModeIf*>& available_modes = GAME_MODE_CTR.get_available_modes();

        m_game_mode_list.clear();
        for( size_t idx = 0; idx < available_modes.size(); idx++ )
        {
            WtGameModeIf* current_mode = available_modes[idx];

            std::function<void(void)> cb = [this, idx]() {
                                                            std::cout << "button " << idx << " pressed\n";
                                                            for( size_t l_idx = 0; l_idx < get_listener().size(); l_idx++ )
                                                            {
                                                                get_listener()[l_idx]->notify_game_mode_changed( GAME_MODE_CTR.get_available_modes()[idx] );
                                                            }
                                                            leave();
                                                         };
            WtButton* mode_btn = new WtButton( WtCoord(offset_x, offset_y+(idx*(69+20))),
                                               WtDim(328, 69),
                                               "menu_btn.bmp",
                                               cb,
                                               current_mode->get_title() );
            m_game_mode_list.push_back( mode_btn );
            add_button( *mode_btn );
        }
        add_button( m_leave_btn );
    }

    ~WtMenuSelectMode()
    {
        for ( size_t idx = 0; idx < m_game_mode_list.size(); idx++ ) 
            delete m_game_mode_list[idx];
    }
private: // no copy allowed
    WtMenuSelectMode( const WtMenuSelectMode& ); 
    WtMenuSelectMode & operator = (const WtMenuSelectMode &);

private:
    WtButton m_leave_btn;

    std::vector<WtButton*> m_game_mode_list;
};

#endif /* _WT_MENU_GAME_MODE_SELECT_H_ */
