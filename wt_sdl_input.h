/*!*****************************************************************************
 * @file wt_sdl_input.h
 * @brief policy implementation for sdl input handling
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
#ifndef _WT_SDL_INPUT_H_
#define _WT_SDL_INPUT_H_

#include "SDL.h"
#include "wt_sdl_config.h"
#include "wt_types.h"
#include "wt_backend_policy_if.h"

#define SDL_INPUT  INPUT( WtInputPolicySdl )
class WtInputPolicySdl : public WtInputPolicyIf
{
protected:

    WtInputPolicySdl()
    {
    }

    ~WtInputPolicySdl() {}
    /**************************
     *
     *************************/   
    WtInputEvent read_input()
    {
        WtInputEvent event;

        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            switch (sdl_event.type)
            {
                case SDL_QUIT:
                    event.is_key_event = true;
                    event.key = wt_control_QUIT;
                    break;
                case SDL_KEYDOWN:
                    event.is_key_event = true;
                    if ('a'==sdl_event.key.keysym.sym) event.key = wt_control_LEFT;
                    if ('d'==sdl_event.key.keysym.sym) event.key = wt_control_RIGHT;
                    if ('s'==sdl_event.key.keysym.sym) event.key = wt_control_DROP;
                    if ('p'==sdl_event.key.keysym.sym) event.key = wt_control_PAUSE;
                    if ('q'==sdl_event.key.keysym.sym) event.key = wt_control_QUIT;
                    if (SDLK_AC_BACK == sdl_event.key.keysym.sym) event.key = wt_control_QUIT;
                    break;
                case SDL_FINGERDOWN:
                    event.is_key_event = false;
                    event.pos.x = sdl_event.tfinger.x;
                    event.pos.y = sdl_event.tfinger.y;
                    break;
                case SDL_FINGERMOTION:
                    event.is_key_event = false;
                    event.is_motion_event = true;
                    event.pos.x = sdl_event.tfinger.x;
                    event.pos.y = sdl_event.tfinger.y;
                    event.d_pos.x = sdl_event.tfinger.dx;
                    event.d_pos.y = sdl_event.tfinger.dy;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    event.is_key_event = false;
                    event.pos.x = sdl_event.button.x;
                    event.pos.y = sdl_event.button.y;
                    break;
                default: 
                    SDL_PumpEvents();
                    SDL_FlushEvent(SDL_FINGERDOWN);
                    break;
            }
        }
        return event;
    }

    /**************************
     *
     *************************/   
    std::string get_key_map()
    {
        return std::string("a - LEFT\nd - RIGHT\ns - DROP\np - PAUSE\nq - QUIT\n");
    }


private:
    WtInputPolicySdl( const WtInputPolicySdl& ); 
    WtInputPolicySdl & operator = ( const WtInputPolicySdl& );
};


#endif /* _WT_SDL_INPUT_H */
