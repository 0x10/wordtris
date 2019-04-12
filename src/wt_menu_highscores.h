/*!*****************************************************************************
 * @file wt_menu_highscores.h
 * @brief menu class for game scores
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
#ifndef _WT_MENU_HIGHSCORES_H_
#define _WT_MENU_HIGHSCORES_H_

#include "wt_menu_if.h"

class WtMenuHighscores : public WtMenuIf
{
public:
    WtMenuHighscores() :
        WtMenuIf( 0x400, "bg_menu_settings.bmp" ),
        m_leave_btn( WtCoord( 105, 800 ), 
                     WtDim(100, 100), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuHighscores::leave ) )
    {
        add_button( m_leave_btn );
    }
    ~WtMenuHighscores() {}
private: // no copy allowed
    WtMenuHighscores( const WtMenuHighscores& ); 
    WtMenuHighscores & operator = (const WtMenuHighscores &);

private:
    WtButton m_leave_btn;
};

#endif /* _WT_MENU_HIGHSCORES_H_ */
