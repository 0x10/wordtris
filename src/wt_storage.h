/*!*****************************************************************************
 * @file wt_storage.h
 * @brief interface for storing and loading data
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
#ifndef _WT_STORAGE_H_
#define _WT_STORAGE_H_

#include <fstream>
#include <unistd.h>
#include "wt_storage_types.h"
#include "SDL.h"

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h> 
#include <android/log.h>

#define APPNAME "squabbel"
#endif

#define STORAGE  WtStorageCtr::instance()
class WtStorageCtr 
{
private:
    // changes affect devices with installed base
    struct _FileHeader
    {
        _FileHeader() :
            magic( m_header_magic )
        {}

        uint32_t magic;
    };

    // changes affect devices with installed base
    struct _FileData
    {
        _FileData() :
            settings(),
            highscores()
        {}

        WtSettings   settings;
        WtHighscores highscores;       
    };

    struct _PersistentFileStructure
    {
        _PersistentFileStructure() :
            header(),
            data()
        {}

        _FileHeader  header;
        _FileData    data;
    };

    typedef std::map< std::string, std::vector<uint8_t> > SimpleFileCache;
    // least significant word is fixed magic and most sig. word
    // may represent file structure
    static           const uint32_t m_header_magic = 0x0003ADDE;
    static constexpr const char*    m_fname        = "squabbel.dat";

// singleton definition
public:
    static WtStorageCtr& instance()
    {
        static WtStorageCtr _instance;
        return _instance;
    }
    ~WtStorageCtr()
    {
    }
private:
    WtStorageCtr() :
        m_storage_copy(),
        m_cwd(""),
        m_asset_cache()
#ifdef __ANDROID__
        ,
        m_assetManager(0),
        _global_asset_manager()
#endif /* __ANDROID__ */
    {
        m_storage_copy.header.magic = m_header_magic;
        m_storage_copy.data.settings = WtSettings();
        m_storage_copy.data.highscores.clear();

        char cwd_buf[512];
        const char* cwd;
#ifndef __ANDROID__
        std::cout << "cwd = "<<getcwd(cwd_buf, 512)<<std::endl;
        cwd = cwd_buf;
        std::cout << "cwd = " << std::string(cwd) << std::endl;
#else
        m_assetManager = getAssetManager();
        cwd = SDL_AndroidGetInternalStoragePath();
        std::cout << "a cwd = " << cwd << std::endl; 
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "%s", cwd );
#endif
        m_cwd = std::string( cwd );

        WtL10n::register_lang_change_obsever( WT_BIND_EVENT_HANDLER( WtStorageCtr::update_language ) );
    }
    WtStorageCtr( const WtStorageCtr& ); 
    WtStorageCtr & operator = (const WtStorageCtr &);

public:
    /**************************
     *
     *************************/
    void store_settings( WtSettings& settings )
    {
        m_storage_copy.data.settings = settings;

        write_to_file();
    }

    /**************************
     *
     *************************/
    void store_highscores( WtHighscores& scores )
    {
        m_storage_copy.data.highscores = scores;

        if ( ! write_to_file() )
        {
            std::cout << "write failed" << std::endl;
        }
        else
            std::cout << "write success" << std::endl;
    }

    /**************************
     *
     *************************/
    bool load()
    {
        bool read_successful = read_from_file();


        std::cout << "read was " << (read_successful ? "successful" : "not successful") << std::endl;

        if ( !read_successful )
        {
            //load default
        }

        std::cout << "settings.size() == " << m_storage_copy.data.settings.size() << std::endl;
        std::cout << "highscores.size() == " << m_storage_copy.data.highscores.size() << std::endl;
        std::cout << "settings.language == " << m_storage_copy.data.settings.language << std::endl;
        std::cout << "settings.game mode == " << m_storage_copy.data.settings.game_mode << std::endl;
        std::cout << "settings.diff == " << static_cast<size_t>(m_storage_copy.data.settings.difficulty) << std::endl;
        std::cout << "settings.active_theme == " << m_storage_copy.data.settings.active_theme << std::endl;
        // distribute
        //
        return read_successful;
    }

    /**************************
     *
     *************************/
    WtSettings& get_settings()
    {
        return m_storage_copy.data.settings;
    }

    /**************************
     *
     *************************/
    WtHighscores& get_scores()
    {
        return m_storage_copy.data.highscores;
    }


    /**************************
     *
     *************************/
    std::vector<uint8_t>& get_asset_file_buf( const std::string fname )
    {
        SimpleFileCache::iterator it = m_asset_cache.find(fname);
        if ( it != m_asset_cache.end() )
        {
            // load from cache
            return (*it).second;
        }
        else
        {
            std::cout << "read from assets folder: " << fname << std::endl;
            internal_load_from_assets( fname, m_asset_cache[fname] );
            return m_asset_cache[fname];
        }
    }

    /**************************
     *
     *************************/
    void update_language()
    {
        m_storage_copy.data.settings.language = WtL10n::get_language_code();
        write_to_file();
        std::cout << "new lanuage selected = "<< m_storage_copy.data.settings.language << std::endl;
    }

private:

    /**************************
     *
     *************************/
    bool write_to_file()
    {
        bool success = false;

        std::string fname = m_cwd;
        fname.append( "/" );
        fname.append( m_fname );

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "write to: %s", fname.c_str() );
#endif
        std::ofstream output_file( fname, std::ios::binary );
        if ( output_file.is_open() )
        {
            m_storage_copy.header.magic = m_header_magic;

            WtStorable::write_unsigned<uint32_t>( output_file, m_storage_copy.header.magic );

            m_storage_copy.data.settings.write( output_file );

            WtStorable::write_unsigned<size_t>( output_file, m_storage_copy.data.highscores.size() );
            for(size_t idx = 0; idx < m_storage_copy.data.highscores.size(); idx++ )
            {
                m_storage_copy.data.highscores[idx].write( output_file );
            }

            output_file.close();

            success = true;
        }

        return success;
    }


    /**************************
     *
     *************************/
    bool read_from_file()
    {
        bool success = false;

        std::string fname = m_cwd;
        fname.append( "/" );
        fname.append( m_fname );

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "read from: %s", fname.c_str() );
#endif
        std::ifstream input_file( fname, std::ios::binary );
        if ( input_file.is_open() )
        {
            bool was_eof = false;
            m_storage_copy.header.magic = WtStorable::read_unsigned<uint32_t>( input_file, was_eof );

            //if ( (! input_file.eof()) && (!input_file.fail()) ) in case latter wont work
            if ( !was_eof && ( static_cast<size_t>(input_file.gcount()) == sizeof(m_storage_copy.header) ) )
            {
                // check binary compatibility
                if ( m_storage_copy.header.magic == m_header_magic )
                {
                    std::cout << "read: header matches\n";

                    if ( m_storage_copy.data.settings.read( input_file ) )
                    {
                        m_storage_copy.data.highscores.clear();
                        was_eof = false;
                        size_t highscore_entries = WtStorable::read_unsigned<size_t>( input_file, was_eof );
                        if ( ! was_eof )
                        {
                            size_t idx = 0;
                            for ( ; idx < highscore_entries; idx++ )
                            {
                                WtScoreEntry tmp;
                                if ( tmp.read( input_file ) )
                                    m_storage_copy.data.highscores.push_back( tmp );
                                else
                                    break;
                            }

                            // check read size
                            if ( idx == highscore_entries )
                            {
                                success = true;
                            }
                        }
                        else
                        {
                            success = true;
                        }
                    }
                }
            }
        }

        return success;
    }

    /**************************
     *
     *************************/
    void internal_load_from_assets( const std::string fname, std::vector<uint8_t>& buffer )
#ifdef __ANDROID__
    {
        // Open your file
        AAsset* file = AAssetManager_open(m_assetManager, fname.c_str(), AASSET_MODE_BUFFER);

        // Get the file length
        size_t fileLength = AAsset_getLength(file);

        // Allocate memory to read your file
        buffer.resize(fileLength+1);

        // Read your file
        AAsset_read(file, buffer.data(), fileLength);
        // For safety you can add a 0 terminating character at the end of your file ...
        buffer[fileLength] = '\0';
        AAsset_close( file );
    }
#else
    {
        std::ifstream file( std::string("assets/").append(fname), std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer.resize(static_cast<size_t>(size));
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
        {
            std::cout << "error read file " << fname << std::endl;
        }
    }
#endif /* __ANDROID__ */


#ifdef __ANDROID__
    /**************************
     *
     *************************/
    AAssetManager* getAssetManager()
    {
        JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

        jobject activity = (jobject)SDL_AndroidGetActivity();

        jclass activity_class = env->GetObjectClass(activity);

        jmethodID activity_class_getAssets = env->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
        jobject asset_manager = env->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
        _global_asset_manager = env->NewGlobalRef(asset_manager);

        return AAssetManager_fromJava(env, _global_asset_manager);
    }
#endif /* __ANDROID__ */

private:
    _PersistentFileStructure m_storage_copy;
    std::string              m_cwd;

    SimpleFileCache          m_asset_cache;
#ifdef __ANDROID__
    AAssetManager*           m_assetManager;
    jobject                  _global_asset_manager;
#endif /* __ANDROID__ */
};


#endif /* _WT_STORAGE_H_ */
