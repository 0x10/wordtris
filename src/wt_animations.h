/*!*****************************************************************************
 * @file wt_animations.h
 * @brief base implementations and derived types for animations
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
#ifndef _WT_ANIMATIONS_H_
#define _WT_ANIMATIONS_H_

#include "wt_animation_if.h"
#include "wt_utils.h"
#include <vector>


struct WtAnimatableText {
    WtAnimatableText( WtCoord p,
                      std::string t,
                      std::string f ) :
    pos ( p ),
    text( t ),
    font( f ) {}
    WtCoord pos;
    std::string text;
    std::string font;
};


/**************************
 *
 *************************/
class WtTextAnimation : public WtAnimationIf<WtAnimatableText>
{
public:
    typedef WtAnimationStep<WtAnimatableText> TextAnimationStep;
public:
    WtTextAnimation() :
        WtAnimationIf<WtAnimatableText>()
    {
    }
    ~WtTextAnimation() {}

    /**************************
     *
     *************************/
    void draw_content( const WtAnimatableText& text )
    {
        ACTIVE_WINDOW.draw_text( text.pos, text.text );
    }
};


struct WtAnimatableGridContent {
    WtAnimatableGridContent( uint8_t r,
                 uint8_t c,
                 uint8_t r_count,
                 uint8_t c_count,
                 std::string f,
                 std::vector<char> cells ) :
        row( r ),
        col( c ),
        row_count( r_count ),
        col_count( c_count ),
        font( f ),
        cell_content( cells )
    {}

    uint8_t row;
    uint8_t col;
    uint8_t row_count;
    uint8_t col_count;
    std::string font;
    std::vector<char> cell_content;
};


/**************************
 *
 *************************/
class WtGridAnimation : public WtAnimationIf<WtAnimatableGridContent>
{
public:
    struct GridText {
        GridText( uint8_t r,
                  uint8_t c,
                  bool    horiz,
                  std::string t,
                  std::string f ) :
            text( t ),
            font( f ),
            row ( r ),
            col ( c ),
            horizontal( horiz )
            {}
        std::string text;
        std::string font;
        uint8_t row;
        uint8_t col;
        bool    horizontal;
    };
    typedef WtAnimationStep<WtAnimatableGridContent> GridAnimationStep;

    /**************************
     *
     *************************/
    static WtGridAnimation* no_animation()
    {
        return nullptr;
    }

public:
    WtGridAnimation() :
        WtAnimationIf<WtAnimatableGridContent>()
    {
    }
    ~WtGridAnimation() {}

    /**************************
     *
     *************************/
    static WtAnimatableGridContent fromGridText( GridText gt )
    {
        WtAnimatableGridContent gc( gt.row, gt.col,
                        ( gt.horizontal ? 1 : gt.text.length() ),
                        ( gt.horizontal ? gt.text.length() : 1 ),
                        gt.font, 
                        std::vector<char>(gt.text.begin(), gt.text.end()));
        return gc;
    }

private:
    /**************************
     *
     *************************/
    void draw_content( const WtAnimatableGridContent& step )
    {
        for ( uint8_t r_idx = 0; r_idx < step.row_count; r_idx++ )
        {
            for ( uint8_t c_idx = 0; c_idx < step.col_count; c_idx++ )
            {
                size_t cell_idx = static_cast<size_t>( (r_idx * step.col_count) + c_idx );
                ACTIVE_WINDOW.draw_at_grid( step.row + r_idx,
                                            step.col + c_idx,
                                            step.cell_content[cell_idx],
                                            step.font );
            }
        }
    }
};



#endif /* _WT_ANIMATIONS_H_ */
