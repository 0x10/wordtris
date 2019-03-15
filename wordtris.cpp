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

#include "wt_game_mode_ctr.h"
#include "wt_game_ctr.h"
#include "wt_menu_ctr.h"

/******************************************************************************
 *
 *****************************************************************************/
int main( int argc, char** argv )
{
    MENU_CTR.listen( &GAME_CTR );

    GAME_CTR.set_pause_menu( MENU_CTR.get_pause_menu() );

    while( 1 )
    {
        MENU_CTR.show();

        GAME_CTR.run();
    }

    return 0;
}


