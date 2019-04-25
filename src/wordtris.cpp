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

    (void)MENU_CTR.show();

    return 0;
}


