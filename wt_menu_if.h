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

#include "wt_backend_select.h"
#include "wt_settings_observer_if.h"
#include "wt_input.h"
#include "wt_button.h"

#define MENU_BUTTON_ID( id ) (this->get_id() + id)
#define TO_BUTTON_ID( id ) (id - this->get_id())

class WtMenuIf : WtInputObserver
{
public:
    WtMenuIf( std::string bg_img="bg_menu.bmp" ) :
        m_menu_id(0xFF00),
        m_shall_leave( false ),
        m_bg( bg_img )
    {
    }
    WtMenuIf( uint16_t menu_id, std::string bg_img="bg_menu.bmp") :
        m_menu_id( menu_id ),
        m_shall_leave( false ),
        m_bg( bg_img )
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

            usleep(12500);
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
    const uint16_t get_id() const
    {
        return m_menu_id;
    }

    /**************************
     *
     *************************/
    void add_button( WtButton button )
    {
        bool already_exists = false;

        button.set_id( MENU_BUTTON_ID( button.id() ) );

        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            if ( button.id() == m_buttons[idx].id() )
            {
                already_exists = true;
                break;
            }
        }

        if (! already_exists)
        {
            m_buttons.push_back( button );
        }
    }

    /**************************
      *
      *************************/   
    void add_list( const WtCoord&                  pos,
                   const WtDim&                    size,
                   const std::string               btn_image,
                   const std::vector<std::string>& texts )
    {
        WtCoord working_pos = pos;
        for ( size_t idx = 0; idx < texts.size(); idx++ )
        {
            add_button( WtButton( idx, working_pos, size, btn_image, texts[idx] ) );
            working_pos.y += (size.h + 20);
        }
    }

#if 0
    /**************************
     *
     *************************/
    void remove_button( const uint8_t id )
    {
        ACTIVE_INPUT.remove_button( MENU_BUTTON_ID( id ) );

        for (size_t i=0;i<m_buttons.size();i++)
        {
            if ( m_buttons[i].id() == id )
            {
                m_buttons.erase( m_buttons.begin()+i );
                break;
            }
        }
    }
#endif

    /**************************
     *
     *************************/
    void open_menu()
    {
        m_shall_leave = false;

        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_INPUT.add_button( m_buttons[idx] );
        }
        ACTIVE_INPUT.listen( this );

        ACTIVE_WINDOW.set_bg( get_bg_img() );

#if 0
        ACTIVE_WINDOW.clr();

        show_self();

        ACTIVE_WINDOW.update();
#endif
    }

    /**************************
     *
     *************************/
    void close_menu()
    {
        ACTIVE_INPUT.ignore( this );

        for (size_t i=0;i<m_buttons.size();i++)
        {
            ACTIVE_INPUT.remove_button( m_buttons[i] );
        }

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
    void show_self()
    {
        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_WINDOW.draw_button( m_buttons[idx] );
        }
    }

private:
    uint16_t                                m_menu_id;
    bool                                    m_shall_leave;
    std::string                             m_bg;
    std::vector<WtButton>                   m_buttons;
    std::vector<WtSettingsChangeObserver*>  m_change_listener;
};


#endif /* _WT_MENU_IF_H_ */
