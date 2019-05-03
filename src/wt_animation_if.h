/*!*****************************************************************************
 * @file wt_animation_if.h
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
#ifndef _WT_ANIMATION_IF_H_
#define _WT_ANIMATION_IF_H_

#include "wt_view_if.h"

/**************************
 *
 *************************/
template<typename animation_content>
class WtAnimationStep
{
public:
    WtAnimationStep( animation_content c, const WtTime::TimeType duration, const size_t rep_count=0 ) :
        content( c ),
        step_duration( duration ),
        repeat_count( rep_count )
    {}

    ~WtAnimationStep() {}

public:
    animation_content content;
    WtTime::TimeType  step_duration;
    size_t            repeat_count;
};

/**************************
 *
 *************************/
template<typename animation_content>
class WtAnimationIf : public WtViewIf
{
public:
    using OnUpdateOverlayDelegate = std::function<void(void)>;
    typedef WtAnimationStep<animation_content> AnimationStep;

    /**************************
     *
     *************************/
    static const WtAnimationIf<animation_content>& no_animation()
    {
        static const WtAnimationIf<animation_content> empty;
        return empty;
    }

public:
    WtAnimationIf() :
        WtViewIf( "bg.bmp", false ),
        m_animation_steps(),
        m_update_overlay( nullptr ),
        m_a_idx( 0 ) {}
    virtual ~WtAnimationIf() {}

    /**************************
     *
     *************************/
    void push_back( const AnimationStep& step )
    {
        m_animation_steps.push_back( step );
    }

    /**************************
     *
     *************************/
    const AnimationStep& operator[]( const size_t idx) const
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
    void set_overlay_drawing( OnUpdateOverlayDelegate update_overlay )
    {
        m_update_overlay = update_overlay;
    }

    /**************************
     *
     *************************/
    void left_view()
    {
        m_a_idx = 0;
    }

    /**************************
     *
     *************************/
    virtual void draw_view()
    {
        // empty instance just to omit drawing
    }

    /**************************
     *
     *************************/
    virtual void update_view()
    {
        if ( m_update_overlay ) m_update_overlay();
        if ( m_a_idx < m_animation_steps.size() )
        {
            draw_animation_step( m_animation_steps[m_a_idx] );

            ACTIVE_WINDOW.update();
            WtTime::sleep( m_animation_steps[m_a_idx].step_duration );
            if ( m_animation_steps[m_a_idx].repeat_count == 0 )
                m_a_idx++;
            else
                m_animation_steps[m_a_idx].repeat_count--;
        }
        else
        {
            ACTIVE_WINDOW.update();
        }

        if ( m_a_idx >= m_animation_steps.size() )
        {
            m_a_idx = 0;
            leave();
        }
    }

private:
    /**************************
     *
     *************************/
    void draw_animation_step( const AnimationStep& step )
    {
        draw_content( step.content );
    }
protected:
    /**************************
     *
     *************************/
    virtual void draw_content( const animation_content& ) =0;

private:
    std::vector< AnimationStep > m_animation_steps;
    OnUpdateOverlayDelegate m_update_overlay;
    size_t m_a_idx;
};



#endif /* _WT_ANIMATION_IF_H_ */
