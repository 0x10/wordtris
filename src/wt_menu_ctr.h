/*!*****************************************************************************
 * @file wt_menu_ctr.h
 * @brief main menu control class
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
#ifndef _WT_MENU_CTR_H_
#define _WT_MENU_CTR_H_

#include "wt_menu_if.h"
#include "wt_menu_settings.h"
#include "wt_menu_highscores.h"
#include "wt_menu_pause.h"

#define MENU_CTR  WtMenuCtr::instance()
class WtMenuCtr : public WtMenuIf
{
// singleton definition
public:
    static WtMenuCtr& instance()
    {
        static WtMenuCtr _instance;
        return _instance;
    }
    ~WtMenuCtr()
    {
    }
private:
    WtMenuCtr() :
        WtMenuIf( 0x100, "bg_menu_settings.bmp" ),
        m_drag_start_pos(0,0),
        m_was_drag(false),
        m_drag_button_id(0)
    {
        add_button( WtButton( 1, WtCoord( 170, 193/*493*/ ), WtDim( 200, 200 ), "start_btn.bmp" ) );
        add_button( WtButton( 2, WtCoord( 105, 800 ), WtDim( 100, 100 ), "score_btn.bmp" ) );
        add_button( WtButton( 3, WtCoord( 332, 800 ), WtDim( 100, 100 ), "settings_btn.bmp" ) );

        add_button( WtButton( 4, WtCoord( 105, 493 ), WtDim( 328, 200 ), "list_item_active.bmp" ) );
        m_settings.listen( m_pause_menu.get_help_listener() );

    }
    WtMenuCtr( const WtMenuCtr& ); 
    WtMenuCtr & operator = (const WtMenuCtr &);

public:
    /**************************
     *
     *************************/
    WtMenuIf* get_pause_menu()
    {
        return &m_pause_menu;
    }

    /**************************
     *
     *************************/
    virtual void listen( WtSettingsChangeObserver* listener )
    {
        WtMenuIf::listen( listener );
        m_settings.listen( listener );
    }
private:
    /**************************
     *
     *************************/
    virtual void notify_button_pressed( uint16_t id )
    {
        switch( TO_BUTTON_ID( id ) )
        {
            case 1:
                leave();
                break;
            case 2:
                enter_child_menu( m_scores );
                break;
            case 3:
                enter_child_menu( m_settings );
                break;
            default: /*ignore*/ break;
        }
    }
    /**************************
     *
     *************************/
    virtual void notify_motion( WtCoord pos, 
                                WtCoord d_pos, 
                                bool is_drag )
    {
        if ( is_drag )
        {
            if ( !m_was_drag )
            {
               // drag started 
               m_drag_start_pos = pos;
               WtButton* list = get_button(4);
               if ( m_drag_start_pos.in_region( list->position(), list->size() )  )
               {
                   m_drag_button_id = TO_BUTTON_ID(list->id());
                   std::cout << "start btn drag = " << m_drag_button_id << std::endl;
               }
            }


            if ( m_drag_button_id != 0 )
            {
                WtButton* drag_btn = get_button(m_drag_button_id);
                drag_btn->set_x( drag_btn->x() + d_pos.x );

                std::cout << "continue btn drag: "<< m_drag_button_id<< "@" << drag_btn->x() << std::endl;
            }

            m_was_drag = true;
        }
        else
        {
            if ( m_was_drag )
            {
               // drag stopped
                
            }
            m_was_drag = false;
            m_drag_button_id = 0;
        }

     /*   std::cout << (is_drag ? "drag" : "motion") << " at: " << 
                        "(" << (int)pos.x << "," << (int)pos.y << ");"
                        "(" << (int)d_pos.x << "," << (int)d_pos.y << ");"
                    << std::endl;*/
    }

private:
    WtCoord          m_drag_start_pos;
    bool             m_was_drag;
    uint16_t         m_drag_button_id;

    WtMenuSettings   m_settings;
    WtMenuPause      m_pause_menu;
    WtMenuHighscores m_scores;
};

#endif /* _WT_MENU_CTR_H_ */
