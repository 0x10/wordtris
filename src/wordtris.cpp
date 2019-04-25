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

#include "wt_game_ctr.h"
#include "wt_menu_ctr.h"

/******************************************************************************
 *
 *****************************************************************************/
void on_quit();



/******************************************************************************
 *
 *****************************************************************************/
void on_quit()
{
    MENU_CTR.quit();
    GAME_CTR.quit();
}

/******************************************************************************
 *
 *****************************************************************************/
int main( int, char** )
{
    ACTIVE_INPUT.register_on_quit_handler( std::bind( on_quit ) );

    if ( ! STORAGE.load() )
    {
        // defaults should be used...
    }

    GAME_CTR.set_pause_menu( MENU_CTR.get_pause_menu() );

    MENU_CTR.listen( &GAME_CTR );

    while( !ACTIVE_INPUT.shall_be_quit() )
    {
        if ( !ACTIVE_INPUT.shall_be_quit() )
            MENU_CTR.show();
        
        if ( !ACTIVE_INPUT.shall_be_quit() )
            GAME_CTR.run();
    }

    return 0;
}


