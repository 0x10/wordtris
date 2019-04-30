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
#include "wt_input_types.h"

#define SDL_INPUT  INPUT( WtInputPolicySdl )
class WtInputPolicySdl
{
public:

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
        event.call_again = SDL_PollEvent(&sdl_event);
        if (event.call_again)
        {
            switch (sdl_event.type)
            {
                case SDL_QUIT:
                    event.is_key_event = false;
                    event.is_system_event = true;
                    event.key = wt_control_QUIT;
                    break;
                case SDL_APP_WILLENTERBACKGROUND:
                    event.is_key_event = true;
                    event.key = wt_control_PAUSE;
                    break;
                case SDL_KEYUP:
                case SDL_WINDOWEVENT:
                case SDL_TEXTINPUT:
                    break;
                case SDL_KEYDOWN:
                    event.is_key_event = true;
                    if ('a'==sdl_event.key.keysym.sym) event.key = wt_control_LEFT;
                    if ('d'==sdl_event.key.keysym.sym) event.key = wt_control_RIGHT;
                    if ('s'==sdl_event.key.keysym.sym) event.key = wt_control_DROP;
                    if ('p'==sdl_event.key.keysym.sym) event.key = wt_control_PAUSE;
                    if (SDLK_AC_BACK == sdl_event.key.keysym.sym) event.key = wt_control_BACK;
                    if ('b' == sdl_event.key.keysym.sym) event.key = wt_control_BACK;
                    if ('q'==sdl_event.key.keysym.sym)
                    {
                        event.is_key_event = false;
                        event.is_system_event = true;
                        event.key = wt_control_QUIT;
                    }
                    break;

                case SDL_FINGERDOWN:
                    event.is_key_event = false;
                    event.is_press_event = true;
                    event.is_motion_event = false;
                    event.pos.x = sdl_event.tfinger.x;
                    event.pos.y = sdl_event.tfinger.y;
                    break;
                case SDL_FINGERMOTION:
                    event.is_key_event = false;
                    event.is_motion_event = true;
                    event.is_press_event = false;
                    event.pos.x = sdl_event.tfinger.x;
                    event.pos.y = sdl_event.tfinger.y;
                    event.d_pos.x = sdl_event.tfinger.dx;
                    event.d_pos.y = sdl_event.tfinger.dy;
                    break;
                case SDL_FINGERUP:
                    event.is_key_event = false;
                    event.is_press_event = false;
                    event.is_motion_event = false;
                    event.pos.x = sdl_event.tfinger.x;
                    event.pos.y = sdl_event.tfinger.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    event.is_key_event = false;
                    event.is_press_event = true;
                    event.is_motion_event = false;
                    event.pos.x = sdl_event.button.x;
                    event.pos.y = sdl_event.button.y;
                    break;
                case SDL_MOUSEMOTION:
                    event.is_key_event = false;
                    event.is_motion_event = true;
                    event.is_press_event = false;
                    event.pos.x = sdl_event.motion.x;
                    event.pos.y = sdl_event.motion.y;
                    event.d_pos.x = sdl_event.motion.xrel;
                    event.d_pos.y = sdl_event.motion.yrel;
                    break;
                case SDL_MOUSEBUTTONUP:
                    event.is_key_event = false;
                    event.is_press_event = false;
                    event.is_motion_event = false;
                    event.pos.x = sdl_event.button.x;
                    event.pos.y = sdl_event.button.y;
                    break;                    



                default:
                    std::cout << "unknown event = " << sdl_event.type << std::endl;
                    /*SDL_PumpEvents();
                    SDL_FlushEvent( SDL_FINGERDOWN );
                    SDL_FlushEvent( SDL_FINGERUP );
                    SDL_FlushEvent( SDL_FINGERMOTION );*/
                    break;
            }
        }
        return event;
    }

private:
    WtInputPolicySdl( const WtInputPolicySdl& ); 
    WtInputPolicySdl & operator = ( const WtInputPolicySdl& );
};


#endif /* _WT_SDL_INPUT_H */
