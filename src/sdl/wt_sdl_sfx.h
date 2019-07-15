/*!*****************************************************************************
 * @file wt_sdl_sfx.h
 * @brief sdl wrapper class for sound effects and music
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
#ifndef _WT_SDL_SFX_H_
#define _WT_SDL_SFX_H_

#include "SDL.h"
#include "SDL_mixer.h" 

#include "wt_sdl_utils.h"
#include "wt_sdl_config.h"

#define SDL_SFX SFX( WtSfxPolicySdl )
class WtSfxPolicySdl
{
private:
    WtSfxPolicySdl( const WtSfxPolicySdl& );
    WtSfxPolicySdl& operator=( const WtSfxPolicySdl& );

    typedef std::map<std::string, Mix_Music*> SDL_MusicCache;
    typedef std::map<std::string, Mix_Chunk*> SDL_EffectCache;
public:
    WtSfxPolicySdl() :
        m_music_cache(),
        m_effect_cache()
    {
    }
    ~WtSfxPolicySdl() {}

    /**************************
     *
     *************************/
    bool open()
    {
        bool success = true;

        //Initialize SDL_mixer
        if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, AUDIO_S16, 2, 4096 ) < 0 )
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
            success = false;
        }

        Mix_VolumeMusic( MIX_MAX_VOLUME );

        return success;
    }

    /**************************
     *
     *************************/
    void close()
    {
        Mix_HaltMusic();

        clear_sfx_cache();

        Mix_Quit();
    }

    /**************************
     *
     *************************/
    void play_music( const std::string filename )
    {
        // look up in cache and play
        Mix_Music* mus = NULL;
        SDL_MusicCache::const_iterator it = m_music_cache.find( filename );
        if ( it != m_music_cache.end() )
        {
            mus = (*it).second;
        }
        else
        {
            if ( ! filename.empty() )
            {

                std::string path = std::string(SDL_ASSETS);
                path.append("sfx/").append( filename );
                std::cout << "load song from file.. " << path << std::endl;
                m_music_cache[filename] = Mix_LoadMUS( path.c_str() );

                mus = m_music_cache[filename];
            }
        }

        if ( NULL != mus )
        {
            Mix_PlayMusic( mus, -1 );
        }
        else
        {
            std::cout << "error loading music: " << Mix_GetError() << std::endl;
        }
    }

    /**************************
     *
     *************************/
    void toggle_pause( bool pause_active )
    {
        if ( Mix_PlayingMusic() != 0 )
        {
            if ( pause_active )
            {
                Mix_PauseMusic();
            }
            else
            {
                if ( Mix_PausedMusic() != 0 )
                {
                    Mix_ResumeMusic();
                }
            }
        }
    }


    /**************************
     *
     *************************/
    void stop_music()
    {
        Mix_HaltMusic();
    }

    /**************************
     *
     *************************/
    void play_effect( const std::string filename )
    {
        // look up in cache and play
        Mix_Chunk* chunk = NULL;
        SDL_EffectCache::const_iterator it = m_effect_cache.find( filename );
        if ( it != m_effect_cache.end() )
        {
            chunk = (*it).second;
        }
        else
        {
            if ( ! filename.empty() )
            {
                std::cout << "load song from file.. " << filename << std::endl;

                std::string path = std::string(SDL_ASSETS);
                path.append("sfx/").append( filename );
                m_effect_cache[filename] = Mix_LoadWAV( path.c_str() );

                chunk = m_effect_cache[filename];
            }
        }

        if ( NULL != chunk )
        {
            Mix_PlayChannel( -1, chunk, 0 );
        }
    }

private:
    /**************************
     *
     *************************/
    void clear_sfx_cache()
    {
        {
            SDL_MusicCache::iterator it;
            for ( it = m_music_cache.begin(); it != m_music_cache.end(); it++ )
            {
                if ( NULL != it->second ) {
                    Mix_FreeMusic( it->second );
                    it->second = NULL;
                }
            }
            m_music_cache.clear();
        }

        {
            SDL_EffectCache::iterator it;
            for ( it = m_effect_cache.begin(); it != m_effect_cache.end(); it++ )
            {
                if ( NULL != it->second ) {
                    Mix_FreeChunk( it->second );
                    it->second = NULL;
                }
            }
            m_effect_cache.clear();
        }
    }

private:
    SDL_MusicCache  m_music_cache;
    SDL_EffectCache m_effect_cache;
};

#endif /* _WT_SDL_SFX_H */
