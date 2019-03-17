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

#define MENU_BUTTON_ID( id )    (this->get_id() | id)
#define TO_BUTTON_ID( id )      (0x00FF & id)
#define INVALID_BUTTON_ID       (0xFF)

class WtMenuIf : WtInputObserver
{
private:
    const std::string m_tri_state_frame = "tri_state_btn.bmp";
    const std::string m_tri_state_selected_img[3] = { "tri_state_btn_select0.bmp", "tri_state_btn_select1.bmp", "tri_state_btn_select2.bmp" };
    const std::string m_tri_state_unselected_img = "tri_state_btn_select_none.bmp";
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
    void add_tri_state_button( const uint16_t    ids[3], 
                               const std::string label[3],
                               const WtCoord&    frame_pos,
                               const WtDim&      frame_size,
                               const WtCoord&    state_pos,
                               const WtDim&      state_size,
                               const size_t      selected )
    {
        // Add frame which isnt a button at all but can be set
        // using invalid id...
        m_buttons.push_back( WtButton( INVALID_BUTTON_ID,
                                       frame_pos, frame_size,
                                       m_tri_state_frame ) );

        WtCoord working_state_pos = state_pos;

        m_buttons.push_back( WtButton( MENU_BUTTON_ID( ids[0] ),
                                       working_state_pos, state_size,
                                       (selected == ids[0] ? m_tri_state_selected_img[0] : m_tri_state_unselected_img ),
                                       label[0] ) );
        working_state_pos.moveX( state_size );

        m_buttons.push_back( WtButton( MENU_BUTTON_ID( ids[1] ),
                                       working_state_pos, state_size,
                                       (selected == ids[1] ? m_tri_state_selected_img[1] : m_tri_state_unselected_img ),
                                       label[1] ) );
        working_state_pos.moveX( state_size );

        m_buttons.push_back( WtButton( MENU_BUTTON_ID( ids[2] ),
                                       working_state_pos, state_size,
                                       (selected == ids[2] ? m_tri_state_selected_img[2] : m_tri_state_unselected_img ),
                                       label[2] ) );
        working_state_pos.moveX( state_size );                                      
    }

    /**************************
      *
      *************************/
    void modify_tri_state_button( const uint16_t first_id, 
                                  const size_t selected )
    {
        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            if( m_buttons[idx].id() == MENU_BUTTON_ID( first_id ) )
            {
                m_buttons[idx].set_image(   (selected == first_id ? m_tri_state_selected_img[0] : m_tri_state_unselected_img ) );
                m_buttons[idx+1].set_image( (selected == first_id+1u ? m_tri_state_selected_img[1] : m_tri_state_unselected_img ) );
                m_buttons[idx+2].set_image( (selected == first_id+2u ? m_tri_state_selected_img[2] : m_tri_state_unselected_img ) );

                break;
            }
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

    /**************************
     *
     *************************/
    void open_menu()
    {
        m_shall_leave = false;

        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            if ( m_buttons[idx].id() != INVALID_BUTTON_ID )
            {
                ACTIVE_INPUT.add_button( m_buttons[idx] );
            }
        }
        ACTIVE_INPUT.listen( this );

        ACTIVE_WINDOW.set_bg( get_bg_img() );
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
