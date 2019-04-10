/*!*****************************************************************************
 * @file wt_menu_if.h
 * @brief interface declaration for menu views
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
#ifndef _WT_MENU_IF_H_
#define _WT_MENU_IF_H_

#include "wt_button.h"
#include "wt_tristate_button.h"
#include "wt_horizontal_carousel.h"

#include "wt_settings_observer_if.h"
#include "wt_input.h"

#define MENU_BUTTON_ID( id )    (this->get_id() | id)
#define TO_BUTTON_ID( id )      (0x00FF & id)
#define INVALID_BUTTON_ID       (0xFF)

class WtMenuIf
{
public:
    WtMenuIf() :
        m_menu_id(0xFF00),
        m_shall_leave( false ),
        m_bg( "bg_menu.bmp" ),
        m_fade( true )
    {
    }
    WtMenuIf( uint16_t menu_id, std::string bg_img="bg_menu.bmp", bool fade=true ) :
        m_menu_id( menu_id ),
        m_shall_leave( false ),
        m_bg( bg_img ),
        m_fade( fade )
    {
    }
    ~WtMenuIf()
    {
    }

    /**************************
     *
     *************************/
    virtual void listen( WtSettingsChangeObserver* listener )
    {
        m_change_listener.push_back( listener );
    }

    /**************************
     *
     *************************/
    void show()
    {
        open_menu();

        while( !m_shall_leave )
        {
            ACTIVE_INPUT.read();

            ACTIVE_WINDOW.clr();

            show_self();

            menu_update();

            ACTIVE_WINDOW.update();
        }

        close_menu();
    }


protected:
    /**************************
     * signal
     *************************/
    virtual void menu_left()
    {

    }

    /**************************
     * signal
     *************************/
    virtual void menu_update()
    {

    }


    /**************************
     *
     *************************/
    void leave()
    {
        m_shall_leave = true;
    }

    /**************************
     *
     *************************/
    uint16_t get_id() const
    {
        return m_menu_id;
    }

    /**************************
     *
     *************************/
    void add_button( WtButton& button )
    {
        m_buttons.push_back( &button );
    }

    /**************************
     *
     *************************/
    void add_tristate_button( WtTriStateButton& button )
    {
        m_tristate_buttons.push_back( &button );
    }

    /**************************
     *
     *************************/
    void add_horizontal_carousel( WtHorizontalCarousel& carousel )
    {
        m_carousels.push_back( &carousel );
    }

    /**************************
     *
     *************************/
    void open_menu()
    {
        m_shall_leave = false;
        ACTIVE_WINDOW.set_bg( get_bg_img() );

        fade_in();

        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_INPUT.add_active_region( *(m_buttons[idx]) );
        }
        for(size_t idx=0;idx<m_tristate_buttons.size();idx++)
        {
            ACTIVE_INPUT.add_active_region( *(m_tristate_buttons[idx]) );
        }
        for(size_t idx=0;idx<m_carousels.size();idx++)
        {
            ACTIVE_INPUT.add_active_region( *(m_carousels[idx]) );
        }
    }

    /**************************
     *
     *************************/
    void close_menu()
    {
        for (size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_INPUT.remove_active_region( *(m_buttons[idx]) );
        }
        for (size_t idx=0;idx<m_tristate_buttons.size();idx++)
        {
            ACTIVE_INPUT.remove_active_region( *(m_tristate_buttons[idx]) );
        }
        for(size_t idx=0;idx<m_carousels.size();idx++)
        {
            ACTIVE_INPUT.remove_active_region( *(m_carousels[idx]) );
        }

        fade_out();

        menu_left();
    }

    /**************************
     *
     *************************/
    void enter_child_menu( WtMenuIf& sub_menu )
    {
        close_menu();

        sub_menu.show();

        open_menu();
    }

    /**************************
     *
     *************************/
    std::vector<WtSettingsChangeObserver*>& get_listener()
    {
        return m_change_listener;
    }

private:
    /**************************
     *
     *************************/
    std::string get_bg_img()
    {
        return m_bg;
    }

    /**************************
     *
     *************************/
    void fade_in()
    {
        bool done = !m_fade;
        
        std::vector<WtButton> button_fading;

        for (size_t idx=0;idx<m_buttons.size();idx++)
        {
            button_fading.push_back( *m_buttons[idx] );
        }

        for(size_t idx=0;idx<button_fading.size();idx++)
        {
            button_fading[idx].set_x( button_fading[idx].x()-800 );
        }
        
        while( !done )
        {
            ACTIVE_WINDOW.clr();

            for(size_t idx=0;idx<button_fading.size();idx++)
            {
                ACTIVE_WINDOW.draw_button( button_fading[idx] );
            }

            ACTIVE_WINDOW.update();

            usleep(12500);

            done = true;
            for(size_t idx=0;idx<button_fading.size();idx++)
            {
                ssize_t dest_x = m_buttons[idx]->x();

                button_fading[idx].set_x( button_fading[idx].x() + 80);
                if ( button_fading[idx].x() != dest_x )
                    done = false;
            }
        }
    }

    /**************************
     *
     *************************/
    void fade_out()
    {
        bool done = !m_fade;
        std::vector<WtButton> button_fading;

        for (size_t idx=0;idx<m_buttons.size();idx++)
        {
            button_fading.push_back( *m_buttons[idx] );
        }

        while( !done )
        {
            ACTIVE_WINDOW.clr();

            for(size_t idx=0;idx<button_fading.size();idx++)
            {
                ACTIVE_WINDOW.draw_button( button_fading[idx] );
            }

            ACTIVE_WINDOW.update();

            usleep(12500);

            done = true;
            for(size_t idx=0;idx<button_fading.size();idx++)
            {
                ssize_t dest_x = m_buttons[idx]->x() - 800;

                button_fading[idx].set_x( button_fading[idx].x() - 80);
                if ( button_fading[idx].x() != dest_x )
                    done = false;
            }
        }
    }

    /**************************
     *
     *************************/
    void show_self()
    {
        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_WINDOW.draw_button( *(m_buttons[idx]) );
        }
        for(size_t idx=0;idx<m_tristate_buttons.size();idx++)
        {
            ACTIVE_WINDOW.draw_image( m_tristate_buttons[idx]->position(),
                                      m_tristate_buttons[idx]->size(),
                                      m_tristate_buttons[idx]->background_image() );
            ACTIVE_WINDOW.draw_button( m_tristate_buttons[idx]->item<0>() );
            ACTIVE_WINDOW.draw_button( m_tristate_buttons[idx]->item<1>() );
            ACTIVE_WINDOW.draw_button( m_tristate_buttons[idx]->item<2>() );
        }
        for(size_t idx=0;idx<m_carousels.size();idx++)
        {
            for( size_t c_idx = 0; c_idx < m_carousels[idx]->size(); c_idx++ )
            {
                ACTIVE_WINDOW.draw_button( (*(m_carousels[idx]))[c_idx] );
            }
        }
    }

private:
    const uint16_t                          m_menu_id;
    bool                                    m_shall_leave;
    std::string                             m_bg;
    std::vector< WtButton* >                m_buttons;
    std::vector< WtTriStateButton* >        m_tristate_buttons;
    std::vector< WtHorizontalCarousel* >    m_carousels;
    std::vector<WtSettingsChangeObserver*>  m_change_listener;
    bool                                    m_fade;
};


#endif /* _WT_MENU_IF_H_ */
