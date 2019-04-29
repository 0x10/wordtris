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
    if ( ! STORAGE.load() )
    {
        // defaults should be used...
    }
    WtSettings settings = STORAGE.get_settings();
    ACTIVE_WINDOW.set_theme( settings.active_theme );
    WtL10n::set_language( settings.language );

    ACTIVE_WINDOW.clr();
    ACTIVE_WINDOW.draw_message(WtL10n_tr("Loading..."));
    ACTIVE_WINDOW.update();

    {
        WtTime::TimePoint before = WtTime::get_time();
        WtMenuCtr main_menu;
        WtTime::TimePoint after = WtTime::get_time();

        WtTime::TimeType elapsed = WtTime::get_time_elapsed( before, after );
        if ( elapsed < WtTime::from_seconds(1) )
        {
            WtTime::sleep( WtTime::from_seconds(1) - elapsed );
        }

        (void)main_menu.show();
    }
    return 0;
}


