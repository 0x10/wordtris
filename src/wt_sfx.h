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
        m_mute( false ),
        m_playing( false ),
        m_is_music( false ),
        m_active_file( "" )
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
        m_sfx_policy.stop_music();
        m_playing = false;
        m_is_music = false;
        m_active_file = "";
        m_sfx_policy.close();
    }

    /**************************
     *
     *************************/
    void toggle_mute( bool play_audio )
    {
        m_mute = !play_audio;

        if ( m_playing )
        {
            m_playing = false;
            m_sfx_policy.toggle_pause( m_mute );
        }
        else
        {
            if ( m_mute == false)
            {
                play_sfx( m_active_file, m_is_music );
            }
        }
    }

    /**************************
     *
     *************************/
    void play_background_music( const std::string mfile )
    {
        play_sfx( mfile, true );
    }

    /**************************
     *
     *************************/
    void stop_background_music()
    {
        m_playing = false;
        m_is_music = false;
        m_active_file = "";
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

    /**************************
     *
     *************************/
    void play_sound( std::string sfx_name )
    {
        play_sfx( sfx_name.append(".ogg"), false );
    }

    /**************************
     *
     *************************/
    void play_countdown1_sound()
    {
        play_sfx( "countdown_1.ogg", false );
    }

    /**************************
     *
     *************************/
    void play_countdown32_sound()
    {
        play_sfx( "countdown_32.ogg", false );
    }
private:
    /**************************
     *
     *************************/
    void play_sfx( const std::string filename, const bool is_music )
    {
        if ( ! filename.empty() )
        {
            if ( is_music )
            {
                if ( ! ( ( m_playing ) && ( m_active_file == filename ) ) )
                {
                    m_active_file = filename;
                    m_is_music = is_music;

                    if ( ! m_mute ) 
                    {
                        m_playing = true;
                        m_sfx_policy.play_music( filename );
                    }
                }
            }
            else
            {
                if ( m_playing )
                    m_sfx_policy.toggle_pause( true );

                if ( ! m_mute ) 
                    m_sfx_policy.play_effect( filename );

                if ( m_playing )
                    m_sfx_policy.toggle_pause( false );
            }
        }
    }
private:
    SfxPolicy   m_sfx_policy;
    bool        m_mute;
    bool        m_playing;
    bool        m_is_music;
    std::string m_active_file;
};


#endif /* _WT_SFX_H */
