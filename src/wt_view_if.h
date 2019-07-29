/*!*****************************************************************************
 * @file wt_view_if.h
 * @brief interface declaration for views
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
#ifndef _WT_VIEW_IF_H_
#define _WT_VIEW_IF_H_

#include "widgets/wt_button.h"
#include "widgets/wt_tristate_button.h"
#include "widgets/wt_horizontal_carousel.h"

#include "wt_input.h"
#include "wt_sfx.h"
#include "wt_drawing.h"
#include "wt_utils.h"


class WtViewIf
{
public:
    using OnKeyPressDelegate = std::function<void(wt_control)>;
    using OnUpdateOverlayDelegate = std::function<void(void)>;
public:
    WtViewIf( std::string bg_img="#172d4a", 
              bool fade=true, 
              WtTime::TimeType refresh_rate=WtTime::TimeType(0),
              OnKeyPressDelegate on_key_press_handler = nullptr,
              std::string bg_music="menu_bg_music.ogg" ) :
        m_shall_leave( false ),
        m_shall_exit( false ),
        m_bg( bg_img ),
        m_bg_music( bg_music ),
        m_buttons(),
        m_textboxes(),
        m_carousels(),
        m_fade( fade ),
        m_refresh_sleep_time( refresh_rate ),
        m_active_child( nullptr ),
        m_update_overlay( nullptr ),
        m_on_key_press_handler( on_key_press_handler )
    {
    }
    WtViewIf & operator = (const WtViewIf & rhs)
    {
        m_shall_leave = rhs.m_shall_leave;
        m_shall_exit = rhs.m_shall_exit;
        m_bg = rhs.m_bg;
        m_bg_music = rhs.m_bg_music;
        m_buttons.clear();
        m_textboxes.clear();
        m_carousels.clear();
        m_fade = rhs.m_fade;
        m_refresh_sleep_time = rhs.m_refresh_sleep_time;
        m_active_child = nullptr;
        m_update_overlay = rhs.m_update_overlay;
        m_on_key_press_handler = rhs.m_on_key_press_handler;
        return *this;
    }
    WtViewIf( const WtViewIf& rhs ) :
        m_shall_leave( rhs.m_shall_leave ),
        m_shall_exit( rhs.m_shall_exit ),
        m_bg( rhs.m_bg ),
        m_bg_music( rhs.m_bg_music ),
        m_buttons(),
        m_textboxes(),
        m_carousels(),
        m_fade( rhs.m_fade ),
        m_refresh_sleep_time( rhs.m_refresh_sleep_time ),
        m_active_child( nullptr ),
        m_update_overlay( nullptr ),
        m_on_key_press_handler( rhs.m_on_key_press_handler )
    {
    }
    virtual ~WtViewIf()
    {
    }

    /**************************
     *
     *************************/
    bool show( bool keep_music_running = false )
    {
        open_view( keep_music_running );

        while( !m_shall_leave )
        {
            WtTime::TimePoint before = WtTime::get_time();

            ACTIVE_INPUT.read();

            if ( ! m_shall_leave )
            {
                show_self();

                if ( m_refresh_sleep_time > WtTime::TimeType(0) )
                {
                    WtTime::TimePoint after = WtTime::get_time();
                    WtTime::TimeType remaining = m_refresh_sleep_time - WtTime::get_time_elapsed( before, after );
                    if ( remaining > WtTime::TimeType(0) )
                    {
                        WtTime::sleep( remaining );
                    }
                }
            }
        }

        close_view( keep_music_running );

        return m_shall_exit;
    }

    /**************************
     *
     *************************/
    void set_overlay_drawing( OnUpdateOverlayDelegate update_overlay )
    {
        m_update_overlay = update_overlay;
    }

protected:
    /**************************
     *
     *************************/
    void do_exit()
    {
        m_shall_exit = true;
        leave();
    }

    /**************************
     * signal
     *************************/
    virtual void entered_view()
    {
    }

    /**************************
     * signal
     *************************/
    virtual void left_view()
    {
    }

    /**************************
     * signal
     *************************/
    virtual void update_view()
    {
    }

    /**************************
     * signal
     *************************/
    virtual void draw_view()
    {
        ACTIVE_WINDOW.update();
    }

    /**************************
     *
     *************************/
    void on_key_press( wt_control key )
    {
        if ( nullptr != m_active_child )
        {
            m_active_child->on_key_press( key );
        }
        else
        {
            if ( m_on_key_press_handler )
            {
                m_on_key_press_handler(key);
            }
            else
            {
                if ( key == wt_control_BACK )
                {
                    leave();
                }
            }
        }
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
    void add_textbox( WtTextbox& textbox )
    {
        m_textboxes.push_back( &textbox );
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
        m_buttons.push_back( &button.item<0>() );
        m_buttons.push_back( &button.item<1>() );
        m_buttons.push_back( &button.item<2>() );
        m_buttons.push_back( &button.item<3>() );
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
    void open_view( bool keep_music_running=false )
    {
        m_shall_leave = false;
        ACTIVE_WINDOW.set_bg( get_bg_img() );
        ACTIVE_INPUT.register_on_quit_handler( WT_BIND_EVENT_HANDLER( WtViewIf::do_exit ) );

        if ( m_fade ) fade_in();

        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_INPUT.add_active_region( *(m_buttons[idx]) );
        }
        for(size_t idx=0;idx<m_carousels.size();idx++)
        {
            ACTIVE_INPUT.add_active_region( *(m_carousels[idx]) );
        }

        if ( ! keep_music_running )
            ACTIVE_SFX.play_background_music( get_bg_music() );

        entered_view();
    }

    /**************************
     *
     *************************/
    void close_view( bool keep_music_running=false )
    {
        if ( ! keep_music_running )
            ACTIVE_SFX.stop_background_music();

        for (size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_INPUT.remove_active_region( *(m_buttons[idx]) );
        }
        for(size_t idx=0;idx<m_carousels.size();idx++)
        {
            ACTIVE_INPUT.remove_active_region( *(m_carousels[idx]) );
        }

        if ( m_fade ) fade_out();

        left_view();
    }

    /**************************
     *
     *************************/
    void enter_child_menu( WtViewIf& sub_view )
    {
        bool keep_music_running = false;

        if ( sub_view.get_bg_music() == get_bg_music() )
        {
            keep_music_running = true;
        }

        close_view( keep_music_running );

        m_active_child = &sub_view;
        m_shall_exit = sub_view.show( keep_music_running );
        m_active_child = nullptr;

        if ( !m_shall_exit )
        {
            open_view( keep_music_running );
        }
        else
        {
            leave();
        }
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
    std::string get_bg_music()
    {
        return m_bg_music;
    }

    /**************************
     *
     *************************/
    void fade_in()
    {
        bool done = false;
        
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

            WtTime::sleep(WtTime::TimeType(12500));

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
        bool done = false;
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

            WtTime::sleep(WtTime::TimeType(12500));

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
        ACTIVE_WINDOW.clr();

        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_WINDOW.draw_button( *(m_buttons[idx]) );
        }

        for(size_t idx=0;idx<m_carousels.size();idx++)
        {
            for( size_t c_idx = 0; c_idx < m_carousels[idx]->size(); c_idx++ )
            {
                ACTIVE_WINDOW.draw_button( (*(m_carousels[idx]))[c_idx] );
            }
        }

        for(size_t idx=0;idx<m_textboxes.size();idx++)
        {
            ACTIVE_WINDOW.draw_textbox( *(m_textboxes[idx]) );
        }

        if ( m_update_overlay ) m_update_overlay();
        update_view();

        draw_view();
    }

private:
    bool                                    m_shall_leave;
    bool                                    m_shall_exit;
    std::string                             m_bg;
    std::string                             m_bg_music;
    std::vector< WtButton* >                m_buttons;
    std::vector< WtTextbox* >               m_textboxes;
    std::vector< WtHorizontalCarousel* >    m_carousels;
    bool                                    m_fade;
    WtTime::TimeType                        m_refresh_sleep_time;
    WtViewIf*                               m_active_child;
    OnUpdateOverlayDelegate                 m_update_overlay;
    OnKeyPressDelegate                      m_on_key_press_handler;
};


#endif /* _WT_VIEW_IF_H_ */
