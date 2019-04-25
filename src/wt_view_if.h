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
#include "wt_drawing.h"
#include "wt_utils.h"


class WtViewIf
{
public:
    WtViewIf( std::string bg_img="bg.bmp", 
              bool fade=true, 
              WtTime::TimeType refresh_rate=WtTime::TimeType(0) ) :
        m_shall_leave( false ),
        m_shall_exit( false ),
        m_bg( bg_img ),
        m_buttons(),
        m_carousels(),
        m_fade( fade ),
        m_refresh_sleep_time( refresh_rate )
    {
    }
    virtual ~WtViewIf()
    {
    }

    /**************************
     *
     *************************/
    bool show()
    {
        open_view();

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

        close_view();

        return m_shall_exit;
    }

protected:
    /**************************
     *
     *************************/
    void exit()
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
     *
     *************************/
    void leave()
    {
        m_shall_leave = true;
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
    void open_view()
    {
        m_shall_leave = false;
        ACTIVE_WINDOW.set_bg( get_bg_img() );
        ACTIVE_INPUT.register_on_quit_handler( WT_BIND_EVENT_HANDLER( WtViewIf::exit ) );

        if ( m_fade ) fade_in();

        for(size_t idx=0;idx<m_buttons.size();idx++)
        {
            ACTIVE_INPUT.add_active_region( *(m_buttons[idx]) );
        }
        for(size_t idx=0;idx<m_carousels.size();idx++)
        {
            ACTIVE_INPUT.add_active_region( *(m_carousels[idx]) );
        }

        entered_view();
    }

    /**************************
     *
     *************************/
    void close_view()
    {
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
        close_view();

        m_shall_exit = sub_view.show();

        if ( !m_shall_exit )
        {
            open_view();
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

        update_view();

        ACTIVE_WINDOW.update();
    }

private:
    bool                                    m_shall_leave;
    bool                                    m_shall_exit;
    std::string                             m_bg;
    std::vector< WtButton* >                m_buttons;
    std::vector< WtHorizontalCarousel* >    m_carousels;
    bool                                    m_fade;
    WtTime::TimeType                        m_refresh_sleep_time;
};


#endif /* _WT_VIEW_IF_H_ */
