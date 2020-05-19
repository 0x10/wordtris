/*!*****************************************************************************
 * @file wt_input.h
 * @brief generic input class which is configurable by policies (e.g. sdl)
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
#ifndef _WT_INPUT_H_
#define _WT_INPUT_H_

#include "wt_clickable_if.h"
#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"
#include "widgets/wt_button.h"

/**************************************
 * input handling class
 **************************************/
#define INPUT( Policy )  WtInput<Policy>::instance()
template<typename InputPolicy>
class WtInput
{
// singleton definition
public:
    static WtInput& instance()
    {
        static WtInput _instance;
        return _instance;
    }
    ~WtInput() {}
private:
    WtInput() :
        m_active_regions(),
        m_input_policy(),
        m_on_key_press(),
        m_shall_be_quit(false),
        m_on_quit_handler()
    {
    }
    WtInput( const WtInput& ); 
    WtInput & operator = (const WtInput &);

// api defintion
public:
    using OnKeyPressDelegate = std::function<void(wt_control)>;
    using OnQuitDelegate = std::function<void(void)>;

    /**************************
     *
     *************************/
   void clear_all()
   {
        m_on_key_press = nullptr;
        m_on_quit_handler = nullptr;
        m_active_regions.clear();
   }

    /**************************
     *
     *************************/
    void register_key_press_delegate( const OnKeyPressDelegate& on_key_press  )
    {
        m_on_key_press = on_key_press;
    }

    /**************************
     *
     *************************/
    void register_on_quit_handler( const OnQuitDelegate& on_quit_handler  )
    {
        m_on_quit_handler = on_quit_handler;
    }

    /**************************
     *
     *************************/
    void add_active_region( WtClickableIf& clickable_region )
    {
        clickable_region.set_id ( m_active_regions.size() );
        m_active_regions.push_back( &clickable_region );
    }

    /**************************
     *
     *************************/
    void remove_active_region( WtClickableIf& clickable_region )
    {
        for ( size_t idx = 0; idx < m_active_regions.size(); idx++ )
        {
            if ( m_active_regions[idx]->id() == clickable_region.id() )
            {
                std::vector<WtClickableIf*>::iterator it = m_active_regions.begin();
                std::advance(it, idx);
                m_active_regions.erase( it );
                clickable_region.set_id( m_active_regions.max_size() );
                break;
            }
        }
    }

    /**************************
     *
     *************************/
    void read()
    {
        
        WtInputEvent ev = m_input_policy.read_input();
        while ( ev.call_again )
        {
            if ( ev.is_key_event )
            {
                wt_control ch = ev.key;
                if ( m_on_key_press ) m_on_key_press( ch );
            }
            else if ( ev.is_system_event )
            {
                if ( ev.key == wt_control_QUIT )
                {
                    m_shall_be_quit = true;
                    if ( m_on_quit_handler ) m_on_quit_handler();
                }
                if ( ev.key == wt_control_SUSPEND )
                {
                    WtInputEvent ev_next = m_input_policy.read_input();
                    while( ! (ev_next.is_system_event && ev_next.key == wt_control_RESUME ) )
                    {
                        ev_next = m_input_policy.read_input();
                    }
                }
            }
            else
            {
                // eval button
                for ( size_t i = 0; i < m_active_regions.size(); i++ )
                {
                    if ( !ev.is_motion_event )
                    {
                        if ( ev.is_press_event )
                        {
                            m_active_regions[i]->trigger_press( ev.pos );
                        }
                        else
                        {
                            m_active_regions[i]->trigger_release( ev.pos );
                        }
                    }
                    else
                        m_active_regions[i]->trigger_motion( ev.pos, ev.d_pos );

                }
            }

            ev = m_input_policy.read_input();
        }
    }

    /**************************
     *
     *************************/
    bool shall_be_quit() const
    {
        return m_shall_be_quit;
    }

private:
    std::vector< WtClickableIf* > m_active_regions;
    InputPolicy                   m_input_policy;  
    OnKeyPressDelegate            m_on_key_press;
    bool                          m_shall_be_quit;
    OnQuitDelegate                m_on_quit_handler;
};


#endif /* _WT_INPUT_H */
