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
    WtAnimationStep( animation_content c, const WtTime::TimeType duration, const size_t rep_count=0, const std::string initsfx="") :
        content( c ),
        step_duration( duration ),
        repeat_count( rep_count ),
        initial_sfx(initsfx)
    {}

    ~WtAnimationStep() {}

public:
    animation_content content;
    WtTime::TimeType  step_duration;
    size_t            repeat_count;
    std::string       initial_sfx;
};

/**************************
 *
 *************************/
template<typename animation_content>
class WtAnimationIf : public WtViewIf
{
public:
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
    WtAnimationIf( std::string bg_music="game_bg_music.ogg" ) :
        WtViewIf( "#172d4a", false,
                  WtTime::TimeType(0),
                  nullptr,
                  bg_music ),
        m_animation_steps(),
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
        if ( m_a_idx < m_animation_steps.size() )
        {
            if ( m_animation_steps[m_a_idx].initial_sfx != "" )
            {
                ACTIVE_SFX.play_sound(m_animation_steps[m_a_idx].initial_sfx);
            }
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
    size_t m_a_idx;
};


/**************************
 *
 *************************/
class WtAnimationPlaylist
{
public:
    struct WtAnimationPlaylistEntry
    {
        size_t    title_idx;
        WtViewIf* animation;
    };
    /**************************
     *
     *************************/
    static WtAnimationPlaylistEntry new_entry( const size_t idx, WtViewIf* view )
    {
        WtAnimationPlaylistEntry e;
        e.title_idx = idx;
        e.animation = view;
        return e;
    }
public:
    WtAnimationPlaylist() :
        m_playlist()
    {}
    WtAnimationPlaylist( const WtAnimationPlaylist& rhs ) :
        m_playlist( rhs.m_playlist )
    {}

    ~WtAnimationPlaylist()
    {
        for ( size_t idx = 0; idx < m_playlist.size(); idx++ )
        {
            std::cout << "destroy animation...\n";
            delete m_playlist[idx].animation;
            m_playlist[idx].animation = nullptr;
        }
        m_playlist.clear();
    }

    /**************************
     *
     *************************/
    void push_back( WtViewIf* view )
    {
        if ( nullptr != view )
        {
            if ( m_playlist.size() == 0 )
            {
                m_playlist.push_back( new_entry( 0, view ) );
            }
            else
            {
                m_playlist.push_back( new_entry( m_playlist.back().title_idx + 1 , view ) );
            }
        }
    }

    /**************************
     *
     *************************/
    void push_back( WtAnimationPlaylistEntry& animation )
    {
        m_playlist.push_back( animation );
    }

    /**************************
     *
     *************************/
    const WtAnimationPlaylistEntry& operator[]( const size_t idx ) const
    {
        return m_playlist[idx];
    }

    /**************************
     *
     *************************/
    size_t size() const
    {
        return m_playlist.size();
    }

    /**************************
     *
     *************************/
    bool empty() const
    {
        return m_playlist.empty();
    }
private:
    WtAnimationPlaylist & operator = (const WtAnimationPlaylist &);


private:
    std::vector<WtAnimationPlaylistEntry> m_playlist;
};

#endif /* _WT_ANIMATION_IF_H_ */
