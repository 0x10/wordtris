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

#include "wt_view_if.h"
#include "wt_utils.h"
#include <vector>

/**************************
 *
 *************************/
template<typename animation_content>
class WtAnimationStep
{
public:
    WtAnimationStep( animation_content c, const WtTime::TimeType duration  ) :
        content( c ),
        step_duration( duration )
    {}

    ~WtAnimationStep() {}

public:
    animation_content content;
    WtTime::TimeType  step_duration;
};

/**************************
 *
 *************************/
class WtGridAnimation : public WtViewIf
{
public:
    using OnUpdateOverlayDelegate = std::function<void(void)>;
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
    struct GridContent {
        GridContent( uint8_t r,
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
    typedef WtAnimationStep<GridContent> GridAnimationStep;

    /**************************
     *
     *************************/
    static const WtGridAnimation& no_animation()
    {
        static const WtGridAnimation empty;
        return empty;
    }

public:
    WtGridAnimation() :
        WtViewIf( "bg.bmp", false ),
        m_animation_steps(),
        m_update_overlay( nullptr ),
        m_a_idx( 0 )
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
    static GridContent fromGridText( GridText gt )
    {
        GridContent gc( gt.row, gt.col,
                        ( gt.horizontal ? 1 : gt.text.length() ),
                        ( gt.horizontal ? gt.text.length() : 1 ),
                        gt.font, 
                        std::vector<char>(gt.text.begin(), gt.text.end()));
        return gc;
    }

    /**************************
     *
     *************************/
    void set_overlay_drawing( OnUpdateOverlayDelegate update_overlay )
    {
        m_update_overlay = update_overlay;
    }

private:
    /**************************
     *
     *************************/
    void draw_animation_step( const GridAnimationStep& step )
    {
        for ( uint8_t r_idx = 0; r_idx < step.content.row_count; r_idx++ )
        {
            for ( uint8_t c_idx = 0; c_idx < step.content.col_count; c_idx++ )
            {
                size_t cell_idx = static_cast<size_t>( (r_idx * step.content.col_count) + c_idx );
                ACTIVE_WINDOW.draw_at_grid( step.content.row + r_idx,
                                            step.content.col + c_idx,
                                            step.content.cell_content[cell_idx],
                                            step.content.font );
            }
        }
    }

    /**************************
     *
     *************************/
    virtual void update_view()
    {
        if ( m_a_idx < m_animation_steps.size() )
        {
            if ( m_update_overlay ) m_update_overlay();

            draw_animation_step( m_animation_steps[m_a_idx] );

            ACTIVE_WINDOW.update();
            WtTime::sleep( m_animation_steps[m_a_idx].step_duration );
            m_a_idx++;
        }

        if ( m_a_idx >= m_animation_steps.size() )
        {
            m_a_idx = 0;
            leave();
        }
    }
private:
    std::vector< GridAnimationStep > m_animation_steps;
    OnUpdateOverlayDelegate m_update_overlay;
    size_t m_a_idx;
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
