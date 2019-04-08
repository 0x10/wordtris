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
        m_drag_button_id(0),
        m_start_btn( WtCoord( 170, 193/*493*/ ), 
                     WtDim( 200, 200 ),
                     "start_btn.bmp",
                     std::bind ( &WtMenuCtr::leave, this ) ),
        m_score_btn( WtCoord( 105, 800 ), 
                     WtDim( 100, 100 ), 
                     "score_btn.bmp",
                     std::bind ( &WtMenuCtr::enter_score_menu, this ) ),
        m_setting_btn( WtCoord( 332, 800 ),
                       WtDim( 100, 100 ),
                       "settings_btn.bmp",
                       std::bind ( &WtMenuCtr::enter_settings_menu, this ) )
    {
        add_button( m_start_btn );
        add_button( m_score_btn );
        add_button( m_setting_btn );

        //add_button( WtButton( 4, WtCoord( 105, 493 ), WtDim( 328, 200 ), "list_item_active.bmp" ) );
       #if 0 
        std::vector<WtGameModeIf*>& available_modes = GAME_MODE_CTR.get_available_modes();
        std::vector< std::pair<uint16_t, std::string> > labeled_ids;
        
        for( size_t idx = 0; idx < available_modes.size(); idx++ )
        {
            labeled_ids.push_back( std::make_pair( (uint16_t)(4+idx), available_modes[idx]->get_title() ) );
        }

        add_horizontal_carousel( labeled_ids,
                                 WtCoord( 0, 493), WtDim( ACTIVE_WINDOW_WIDTH, 200 ),
                                 4 );
#endif
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
    void enter_score_menu()
    {
        std::cout << "enter score menu\n";
        enter_child_menu( m_scores );
    }

    /**************************
     *
     *************************/
    void enter_settings_menu()
    {
        enter_child_menu( m_settings );
    }

#if 0
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
               if ( m_drag_start_pos.in_region( WtCoord(0,493), WtDim( ACTIVE_WINDOW_WIDTH, 200 ) )  )
               {
                   m_drag_button_id = 4;
                   std::cout << "start btn drag = " << m_drag_button_id << std::endl;
               }
            }


            if ( m_drag_button_id != 0 )
            {
                // move to modify_carousel routine...
                size_t mode_count = GAME_MODE_CTR.get_available_modes().size();
                for (size_t m = 0; m<mode_count; m++)
                {
                    WtButton* drag_btn = get_button(m_drag_button_id+m);
                    if ( drag_btn != NULL )
                    {
                        drag_btn->set_x( drag_btn->x() + d_pos.x );
                        if ( ( drag_btn->x() < 0 ) || ( drag_btn->x()+drag_btn->width() > ACTIVE_WINDOW_WIDTH ) )
                           drag_btn->set_image("list_item_inactive.bmp"); 
                        else
                           drag_btn->set_image("list_item_active.bmp"); 
                    }

//                    std::cout << "continue btn drag: "<< m_drag_button_id<< "@" << drag_btn->x() << std::endl;
                }


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
#endif
private:
    WtCoord          m_drag_start_pos;
    bool             m_was_drag;
    uint16_t         m_drag_button_id;

    WtMenuSettings   m_settings;
    WtMenuPause      m_pause_menu;
    WtMenuHighscores m_scores;

    WtButton         m_start_btn;
    WtButton         m_score_btn;
    WtButton         m_setting_btn;
};

#endif /* _WT_MENU_CTR_H_ */
