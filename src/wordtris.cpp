/*!*****************************************************************************
 * @file wordtris.cpp
 * @brief main unit/entry point for game
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

#include "global.h"

#include "wt_storage.h"

#include "wt_menu_ctr.h"


/******************************************************************************
 *
 *****************************************************************************/
int main( int, char** )
{
    {
        if ( ! STORAGE.load() )
        {
            // defaults should be used...
        }
        if ( ACTIVE_WINDOW.open() )
        {
//            if ( ACTIVE_SFX.open() )
            {
                WtL10n::clear_lang_change_observer();
                ACTIVE_INPUT.clear_all();
                WtSettings settings = STORAGE.get_settings();
                ACTIVE_WINDOW.set_theme( settings.active_theme );
                WtL10n::set_language( settings.language );
                //ACTIVE_SFX.toggle_mute( settings.enable_audio );

                ACTIVE_WINDOW.clr();
                //ACTIVE_WINDOW.draw_message(WtL10n_tr("Loading..."));
                ACTIVE_WINDOW.draw_button( WtButton( WtCoord( (ACTIVE_WINDOW_WIDTH/2)-(307/2), (ACTIVE_WINDOW_HEIGHT / 2) - (307/2)), 
                                                     WtDim(307,307), 
                                                     "splash_logo.bmp") );
                ACTIVE_WINDOW.update();

                WtTime::TimePoint before = WtTime::get_time();
                WtMenuCtr main_menu;
                WtTime::TimePoint after = WtTime::get_time();

                WtTime::TimeType elapsed = WtTime::get_time_elapsed( before, after );
                std::cout << "startup time was " << elapsed.count() << std::endl;
                if ( elapsed < WtTime::from_milliseconds(1000) )
                {
                    WtTime::sleep( WtTime::from_milliseconds(1000) - elapsed );
                }

                (void)main_menu.show();

           //     ACTIVE_SFX.close();
                ACTIVE_WINDOW.close();
            }
        }
    }
    return 0;
}


