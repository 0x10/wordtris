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

#include <vector>

/**************************
 *
 *************************/
template<typename animation_content>
class WtAnimationStep
{
public:
    WtAnimationStep( animation_content c, const size_t duration  ) :
        content( c ),
        step_duration( duration )
    {}

    ~WtAnimationStep() {}

public:
    animation_content content;
    size_t            step_duration;
};

/**************************
 *
 *************************/
class WtGridAnimation 
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
    typedef WtAnimationStep<GridText> GridAnimationStep;

    /**************************
     *
     *************************/
    static const WtGridAnimation& no_animation()
    {
        static const WtGridAnimation empty;
        return empty;
    }

public:
    WtGridAnimation()
     {
     }
    ~WtGridAnimation() {}

    /**************************
     *
     *************************/
    void push_back( const GridAnimationStep& step )
    {
        m_animation_steps.push_back( step );
    }

    /**************************
     *
     *************************/
    const GridAnimationStep& operator[]( const size_t idx) const
    {
        return m_animation_steps[idx];
    }

    /**************************
     *
     *************************/
    bool empty() const
    {
        return m_animation_steps.empty();
    }

    /**************************
     *
     *************************/
    size_t size() const
    {
        return m_animation_steps.size();
    }

private:
    std::vector< GridAnimationStep > m_animation_steps;
};

/**************************
 *
 *************************
class WtImageAnimation
{
private:
    WtCoord m_pos;
    std::vector< WtAnimationStep <std::string> > m_animation_steps;
};
*/

#endif /* _WT_ANIMATIONS_H_ */
