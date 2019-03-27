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

#include "wt_button.h"

#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"

/**************************************
 * base class for each input observer
 **************************************/
class WtInputObserver
{
public:
    virtual void notify_drop() {}
    virtual void notify_left() {}
    virtual void notify_right(){}
    virtual void notify_pause(){}
    virtual void notify_quit(){ exit(0); }
    virtual void notify_button_pressed( uint16_t ) {}
    virtual void notify_motion( WtCoord, WtCoord ) {}
};

/**************************************
 * input handling class
 **************************************/
#define INPUT( Policy )  WtInput<Policy>::instance()
template<typename InputPolicy>
class WtInput : private InputPolicy
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
        InputPolicy()
    {
        m_active_buttons.clear();
    }
    WtInput( const WtInput& ); 
    WtInput & operator = (const WtInput &);

// api defintion
public:
    /**************************
     *
     *************************/
    void listen( WtInputObserver* observer )
    {
        m_input_listener.push_back( observer );
    }

    /**************************
     *
     *************************/
    void ignore( WtInputObserver* observer )
    {
        for (size_t i=0;i<m_input_listener.size();i++)
        {
            if ( m_input_listener[i] == observer )
            {
                m_input_listener.erase( m_input_listener.begin()+i );
                break;
            }
        }
    }

    /**************************
     *
     *************************/
    void add_button( WtButton& button )
    {
        m_active_buttons.push_back( button );
        //std::cout << "new button("<<id<<") @ ("<<pos.x<<","<<pos.y<<") with ("<<size.w<<","<<size.h<<")"<<std::endl;
    }

    /**************************
     *
     *************************/
    void remove_button( WtButton& button )
    {
        for ( size_t i = 0; i < m_active_buttons.size(); i++ )
        {
            if ( m_active_buttons[i].id() == button.id() )
            {
                //std::cout << "del button("<<id<<")"<<std::endl;
                m_active_buttons.erase( m_active_buttons.begin()+i );
            }
        }
    }

    /**************************
     *
     *************************/
    void read()
    {
        WtInputEvent ev = InputPolicy::read_input();

        if ( ev.is_key_event )
        {
            wt_control ch = ev.key;
            switch (ch)
            {
                case wt_control_DROP:
                    for (size_t i = 0; i < m_input_listener.size(); i++)
                        m_input_listener[i]->notify_drop();
                    break;
                case wt_control_LEFT: 
                    for (size_t i = 0; i < m_input_listener.size(); i++)
                        m_input_listener[i]->notify_left();
                    break;
                case wt_control_RIGHT:
                    for (size_t i = 0; i < m_input_listener.size(); i++)
                        m_input_listener[i]->notify_right();
                    break;
                case wt_control_QUIT:
                    for (size_t i = 0; i < m_input_listener.size(); i++)
                        m_input_listener[i]->notify_quit();
                    break;
                case wt_control_PAUSE:
                    for (size_t i = 0; i < m_input_listener.size(); i++)
                        m_input_listener[i]->notify_pause();
                    break;
                default:
                    break;
            }
        }
        else
        {
            if ( !ev.is_motion_event )
            {
                // eval button
                for ( size_t i = 0; i < m_active_buttons.size(); i++ )
                {
                    if ( ( ev.pos.x >= m_active_buttons[i].position().x )
                      && ( ev.pos.x < (m_active_buttons[i].position().x + m_active_buttons[i].width()) )
                      && ( ev.pos.y >= m_active_buttons[i].position().y )
                      && ( ev.pos.y < (m_active_buttons[i].position().y + m_active_buttons[i].height()) ) )
                    {
                        for (size_t list_idx = 0; list_idx < m_input_listener.size(); list_idx++)
                        {
                            m_input_listener[list_idx]->notify_button_pressed( m_active_buttons[i].id() );
                        }

                        break;// no button stacking..
                    }
                }
            }
            else
            {
                for (size_t list_idx = 0; list_idx < m_input_listener.size(); list_idx++)
                {
                    m_input_listener[list_idx]->notify_motion( ev.pos, ev.d_pos );
                }
            }
        }
    }

    /**************************
      *
      *************************/   
    std::string get_input_help()
    {
        return InputPolicy::get_key_map();
    }

private:
    std::vector<WtInputObserver*> m_input_listener;
    std::vector<WtButton>         m_active_buttons;
};


#endif /* _WT_INPUT_H */
