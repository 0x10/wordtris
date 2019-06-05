/*!*****************************************************************************
 * @file wt_sfx.h
 * @brief generic input class which is configurable by policies (e.g. sdl)
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
#ifndef _WT_SFX_H_
#define _WT_SFX_H_

/**************************************
 * input handling class
 **************************************/
#define SFX( Policy )  WtSfx<Policy>::instance()
template<typename SfxPolicy>
class WtSfx
{
// singleton definition
public:
    static WtSfx& instance()
    {
        static WtSfx _instance;
        return _instance;
    }
    ~WtSfx() {}
private:
    WtSfx() :
        m_sfx_policy(),
        m_mute( false )
    {
    }
    WtSfx( const WtSfx& ); 
    WtSfx & operator = (const WtSfx &);

// api defintion
public:
    /**************************
     *
     *************************/
    bool open()
    {
        return m_sfx_policy.open();
    }

    /**************************
     *
     *************************/
    void close()
    {
        m_sfx_policy.close();
    }

    /**************************
     *
     *************************/
    void toggle_mute()
    {
        m_mute = !m_mute;
        if ( m_mute )
        {
            m_sfx_policy.stop_music();
        }
    }

    /**************************
     *
     *************************/
    void play_background_music()
    {
        play_sfx( "game_bg_music.wav", true );
    }

    /**************************
     *
     *************************/
    void stop_background_music()
    {
        m_sfx_policy.stop_music();
    }

    /**************************
     *
     *************************/
    void play_drop_sound()
    {
        play_sfx( "", false );
    }

    /**************************
     *
     *************************/
    void play_bomb_sound()
    {
        play_sfx( "", false );
    }

    /**************************
     *
     *************************/
    void play_solved_sound()
    {
        play_sfx( "", false );
    }

    /**************************
     *
     *************************/
    void play_levelup_sound()
    {
        play_sfx( "", false );
    }

    /**************************
     *
     *************************/
    void play_gameover_sound()
    {
        play_sfx( "", false );
    }

private:
    /**************************
     *
     *************************/
    void play_sfx( const std::string filename, const bool is_music )
    {
        if ( ! m_mute )
        {
            if ( is_music )
            {
                m_sfx_policy.play_music( filename );
            }
            else
            {
                m_sfx_policy.play_effect( filename );
            }
        }
    }
private:
    SfxPolicy m_sfx_policy;
    bool      m_mute;
};


#endif /* _WT_SFX_H */
