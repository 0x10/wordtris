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
    WtAnimationStep( animation_content& c, const size_t duration  ) :
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
    typedef struct {
        std::vector<char> text;
        std::string       font; 
    } GridText;
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
    GridAnimationStep& operator[]( const size_t idx)
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
    uint8_t get_row() const
    {
        return m_row;
    }

    /**************************
     *
     *************************/
    uint8_t get_column() const
    {
        return m_col;
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
