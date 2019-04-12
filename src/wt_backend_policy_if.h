/*!*****************************************************************************
 * @file wt_backend_policy_if.h
 * @brief interface declaration for backend classes used for drawing/input
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
#ifndef _WT_BACKEND_POLICY_IF_H_
#define _WT_BACKEND_POLICY_IF_H_

#include "wt_board.h"
#include "wt_active_letter.h"

class WtDrawingPolicyIf
{
public:
    virtual ~WtDrawingPolicyIf(){}
    virtual void clr() =0;
    virtual void set_theme( std::string name )=0;
    virtual WtDim get_font_size() =0;
    virtual void draw_board( const WtBoard& board ) =0;
    virtual void draw_active_letter( const WtLetter& active ) =0;
    virtual void set_bg( const std::string bg_img ) =0;
    virtual void draw_text( const WtCoord     pos,
                            const std::string text ) =0;

    virtual void draw_image( const WtCoord     pos,
                             const WtDim       size,
                             const std::string image ) =0;
    virtual void update() =0;
};

#endif /* _WT_BACKEND_POLICY_IF_H */
