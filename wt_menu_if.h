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

#define MENU_BUTTON_ID( id ) (this->get_id() + id)
#define TO_BUTTON_ID( id ) (id - this->get_id())

class WtMenuIf : WtInputObserver
{
private:
    static constexpr const char* background_image = "bg_menu.bmp";
public:
    WtMenuIf() :
        m_menu_id(0xFF00),
        m_shall_leave( false )
    {
    }
    WtMenuIf( uint16_t menu_id ) :
        m_menu_id( menu_id ),
        m_shall_leave( false )
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
     *
     *************************/
    virtual void show_self() = 0;

    /**************************
     *
     *************************/
    virtual void menu_left()
    {

    }

    /**************************
     *
     *************************/
    virtual void menu_update()
    {

    }

    /**************************
     *
     *************************/
    virtual std::string get_bg_img()
    {
        return WtMenuIf::background_image;
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
    void add_button( const uint8_t  id,
                     const WtCoord& pos,
                     const WtDim&   size,
                     std::string    label, 
                     bool           labelIsImage )
    {
        bool already_exists = false;
        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            if ( MENU_BUTTON_ID( id ) == m_buttons[idx] )
            {
                already_exists = true;
                break;
            }
        }

        if (! already_exists)
        {
            m_buttons.push_back( MENU_BUTTON_ID( id ) );

            ACTIVE_INPUT.add_button( MENU_BUTTON_ID( id ), pos, size );
        }

        if ( ! labelIsImage )
            ACTIVE_WINDOW.draw_button( pos, label );
    }

    /**************************
      *
      *************************/   
    void add_list( const WtCoord&                  pos,
                   const std::vector<std::string>& texts )
    {
        WtCoord working_pos = pos;
        for ( size_t idx = 0; idx < texts.size(); idx++ )
        {
            add_button( idx, working_pos, WtDim( 500, 80 ), texts[idx], false );
            working_pos.y += (80 + 20);
        }
    }

    /**************************
     *
     *************************/
    void remove_button( const uint8_t id )
    {
        ACTIVE_INPUT.remove_button( MENU_BUTTON_ID( id ) );

        for (size_t i=0;i<m_buttons.size();i++)
        {
            if ( m_buttons[i] == id )
            {
                m_buttons.erase( m_buttons.begin()+i );
                break;
            }
        }
    }

    /**************************
     *
     *************************/
    void open_menu()
    {
        m_shall_leave = false;

        ACTIVE_INPUT.listen( this );

        ACTIVE_WINDOW.set_bg( get_bg_img() );

        ACTIVE_WINDOW.clr();

        show_self();

        ACTIVE_WINDOW.update();
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
        m_buttons.clear();

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
    uint16_t                                m_menu_id;
    bool                                    m_shall_leave;
    std::vector<uint16_t>                   m_buttons;
    std::vector<WtSettingsChangeObserver*>  m_change_listener;
};


#endif /* _WT_MENU_IF_H_ */
