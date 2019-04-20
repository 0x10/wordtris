/*!*****************************************************************************
 * @file wt_settings.h
 * @brief settings helper class
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
#ifndef _WT_SETTINGS_H_
#define _WT_SETTINGS_H_

#include "wt_game_mode_if.h"

class WtSettingsChangeObserver
{
public:
    virtual ~WtSettingsChangeObserver(){}
    virtual void notify_difficulty_changed( wt_difficulty diffi )=0;
    virtual void notify_game_mode_changed( WtGameModeIf* mode )=0;
    virtual void notify_theme_changed( std::string name )=0;
    virtual void notify_supporting_grid_changed( bool show_grid )=0;
    virtual void notify_show_next_stone_changed( bool show_stone )=0;
};



#endif /* _WT_SETTINGS_H_ */
