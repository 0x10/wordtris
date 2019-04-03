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
        GridText( std::string t, std::string f ) :
            text( t ),
            font( f )
            {}
        std::string text;
        std::string font; 
    };
    typedef WtAnimationStep<GridText> GridAnimationStep;

    /**************************
     *
     *************************/
    static const WtGridAnimation& no_animation()
    {
        static const WtGridAnimation empty(0,0);
        return empty;
    }

public:
    WtGridAnimation( uint8_t r, uint8_t c, bool horizontal=true ) :
        m_row(r),
        m_col(c),
        m_horizontal( horizontal )
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
    void move( uint8_t r, uint8_t c )
    {
        m_row = r;
        m_col = c;
    }

    /**************************
     *
     *************************/
    uint8_t row() const
    {
        return m_row;
    }

    /**************************
     *
     *************************/
    uint8_t column() const
    {
        return m_col;
    }

    /**************************
     *
     *************************/
    void set_horizontal()
    {
        m_horizontal = true;
    }

    /**************************
     *
     *************************/
    void set_vertical()
    {
        m_horizontal = false;
    }

    /**************************
     *
     *************************/
    bool is_horizontal() const
    {
        return m_horizontal;
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
    uint8_t m_row;
    uint8_t m_col;
    bool    m_horizontal;

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
