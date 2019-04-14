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
        m_on_key_press()
    {
    }
    WtInput( const WtInput& ); 
    WtInput & operator = (const WtInput &);

// api defintion
public:
    using OnKeyPressDelegate = std::function<void(wt_control)>;

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

private:
    std::vector< WtClickableIf* > m_active_regions;
    InputPolicy                   m_input_policy;  
    OnKeyPressDelegate            m_on_key_press;
};


#endif /* _WT_INPUT_H */
