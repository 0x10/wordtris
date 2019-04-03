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
#include "wt_types.h"
#include "SDL.h"

#ifdef __ANDROID__
#include <android/log.h>

#define APPNAME "wordtris"
#endif

#define STORAGE  WtStorageCtr::instance()
class WtStorageCtr 
{
private:
    // changes affect devices with installed base
    typedef struct
    {
        uint32_t magic;
    } _FileHeader;

    // changes affect devices with installed base
    typedef struct
    {
        WtSettings   settings;
        WtHighscores highscores;       
    } _FileData;

    typedef struct
    {
        _FileHeader  header;
        _FileData    data;
    } _PersistentFileStructure;

    typedef std::map< std::string, std::vector<uint8_t> > SimpleFileCache;
    // least significant word is fixed magic and most sig. word
    // may represent file structure
    static           const uint32_t m_header_magic = 0x0001ADDE;
    static constexpr const char*    m_fname        = "wordtris.dat";

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
    WtStorageCtr()
    {
        m_storage_copy.header.magic = m_header_magic;
        m_storage_copy.data.settings = WtSettings();
        m_storage_copy.data.highscores.clear();

#ifndef __ANDROID__
        std::cout << "cwd = "<<getcwd(m_cwd_buf, 512)<<std::endl;
        m_cwd = m_cwd_buf;
        std::cout << "cwd = " << std::string(m_cwd) << std::endl;
#else
        m_cwd = SDL_AndroidGetInternalStoragePath();
        std::cout << "a cwd = " << m_cwd << std::endl; 
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "%s", m_cwd );
#endif
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

        std::cout << "highscores.size() == " << m_storage_copy.data.highscores.size() << std::endl;
        std::cout << "settings.language == " << m_storage_copy.data.settings.language << std::endl;
        std::cout << "settings.game mode == " << m_storage_copy.data.settings.game_mode << std::endl;
        std::cout << "settings.diff == " << m_storage_copy.data.settings.difficulty << std::endl;
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

private:

    /**************************
     *
     *************************/
    bool write_to_file()
    {
        bool success = false;

        std::string fname = std::string( m_cwd );
        fname.append( "/" );
        fname.append( m_fname );

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "write to: %s", fname.c_str() );
#endif
        std::ofstream output_file( fname, std::ios::binary );
        if ( output_file.is_open() )
        {
            m_storage_copy.header.magic = m_header_magic;

            output_file.write( (char*)&m_storage_copy.header, sizeof(m_storage_copy.header));

            m_storage_copy.data.settings.write( output_file );
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

        std::string fname = std::string( m_cwd );
        fname.append( "/" );
        fname.append( m_fname );

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "read from: %s", fname.c_str() );
#endif
        std::ifstream input_file( fname, std::ios::binary );
        if ( input_file.is_open() )
        {
            input_file.read( (char*)&m_storage_copy.header, sizeof(m_storage_copy.header) );

            //if ( (! input_file.eof()) && (!input_file.fail()) ) in case latter wont work
            if ( input_file.gcount() == sizeof(m_storage_copy.header) )
            {
                // check binary compatibility
                if ( m_storage_copy.header.magic == m_header_magic )
                {
                    std::cout << "read: header matches\n";

                    m_storage_copy.data.settings.read( input_file );

                    m_storage_copy.data.highscores.clear();
                    while( !input_file.eof() )
                    {
                        WtScoreEntry tmp;
                        if ( tmp.read( input_file ) )
                            m_storage_copy.data.highscores.push_back( tmp );
                    }

                    // check read size
                    // todo..
                    {
                        success = true;
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
        AAsset* file = AAssetManager_open(assetManager, fname, AASSET_MODE_BUFFER);
        // Get the file length
        size_t fileLength = AAsset_getLength(file);

        // Allocate memory to read your file
        buffer.resize(fileLength+1);

        // Read your file
        AAsset_read(file, buffer.data(), fileLength);
        // For safety you can add a 0 terminating character at the end of your file ...
        buffer[fileLength] = '\0';
    }
#else
    {
        std::ifstream file( std::string("assets/").append(fname), std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer.resize(size);
        if (!file.read((char*)buffer.data(), size))
        {
            std::cout << "error read file " << fname << std::endl;
        }
    }
#endif /* __ANDROID__ */


private:
    _PersistentFileStructure m_storage_copy;
    char                     m_cwd_buf[512];
    const char*              m_cwd;

    SimpleFileCache          m_asset_cache;
};


#endif /* _WT_STORAGE_H_ */
