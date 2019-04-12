/*!*****************************************************************************
 * @file wt_input_types.h
 * @brief some common type declarations needed within multiple units
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
#ifndef _WT_INPUT_TYPES_H_
#define _WT_INPUT_TYPES_H_


typedef enum wt_controls_tag
{
    wt_control_DROP,
    wt_control_LEFT,
    wt_control_RIGHT,
    wt_control_QUIT,
    wt_control_PAUSE,
    wt_control_INVALID
} wt_control;


/**************************
 *
 *************************/
class WtInputEvent
{
public:
    WtInputEvent() :
        is_key_event( true ),
        is_motion_event( false ),
        is_press_event( false ),
        call_again( false ),
        key( wt_control_INVALID ),
        pos(),
        d_pos()
    {}

    bool       is_key_event;
    bool       is_motion_event;
    bool       is_press_event;
    bool       call_again;
    wt_control key;
    WtCoord    pos;
    WtCoord    d_pos;
};


#endif /* _WT_INPUT_TYPES_H */
