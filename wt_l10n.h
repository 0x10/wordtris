/*!*****************************************************************************
 * @file wt_l10n.h
 * @brief l10n helper
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
#ifndef _WT_L10N_H_
#define _WT_L10N_H_

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


//#define READ_TEXTS_FROM_XML
#define READ_TEXTS_FROM_HEADER

#ifdef READ_TEXTS_FROM_HEADER
#include "wt_l10n_translations.h"
#endif /* READ_TEXTS_FROM_HEADER */


#ifdef READ_TEXTS_FROM_XML
// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
#endif /* READ_TEXTS_FROM_XML */

class WtL10n
{
private:
    typedef std::map< std::string, std::vector<std::string> > LocaleMap;
public:
    /**************************
     *
     *************************/
    static std::vector<std::string>& available_languages()
    {
        return WtL10n::instance().get_languages();
    }

    /**************************
     *
     *************************/
    static void set_language( std::string lang_code )
    {
        WtL10n::instance().set_lang( lang_code );
    }

    /**************************
     *
     *************************/
    static std::string get_language_code()
    {
        return WtL10n::instance().available_languages()[WtL10n::instance().active_language_idx()];
    }

    /**************************
     *
     *************************/
    static std::string get_next_language_code()
    {
        std::vector<std::string>& langs = WtL10n::instance().available_languages();
        return langs[(WtL10n::instance().active_language_idx() + 1) % langs.size()];
    }

    /**************************
     *
     *************************/
    static std::string tr( const std::string s )
    {
        std::string result = s;
        LocaleMap& translations = WtL10n::instance().get_translations();
        LocaleMap::const_iterator it = translations.find(s);

        if ( it != translations.end() )
        {
            result = (*it).second[WtL10n::instance().active_language_idx()];
            if ( result.empty() )
            {
                result = s;
            }
        }
        else
        {
            result = s;
        }

        return result;
    }

    ~WtL10n() {}
private:
    static WtL10n& instance()
    {
        static WtL10n locale;
        return locale;
    }
    WtL10n() :
        m_active_language_idx(0)
    {
        m_languages.push_back("en");
        m_languages.push_back("de");
        m_languages.push_back("fr");

        read_translations( m_translations );
    }

    /**************************
     *
     *************************/
    size_t active_language_idx()
    {
        return m_active_language_idx;
    }

    /**************************
     *
     *************************/
    LocaleMap& get_translations()
    {
        return m_translations;
    }

    /**************************
     *
     *************************/
    void set_lang( std::string lang_code )
    {
        for( size_t l_idx = 0; l_idx < m_languages.size(); l_idx++ )
        {
            if ( m_languages[l_idx] == lang_code )
            {
                m_active_language_idx = l_idx;
                break;
            }
        }
    }

    /**************************
     *
     *************************/
    std::vector<std::string>& get_languages()
    {
        return m_languages;
    }

#ifdef READ_TEXTS_FROM_XML
    /**************************
     *
     *************************/
    void read_xml( std::vector<std::string>& tr_tu )
    {
        //TODO wont work; see https://en.wikibooks.org/wiki/OpenGL_Programming/Android_GLUT_Wrapper#Accessing_assets
        std::ifstream file( "l10n_texts.xml" );

        if ( file )
        {
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            // operations on the buffer...
            std::string xml_hdr;
            std::getline( buffer, xml_hdr );
            if ( xml_hdr == "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" )
            {
                std::string root;
                std::getline( buffer, root );
                if ( root == "<translations>" )
                {
                    std::string tu;
                    bool tu_started = false;

                    while( !buffer.eof() )
                    {
                        std::string line;
                        std::getline(buffer, line);
//                        std::cout << "# " << line << std::endl;
                        trim(line);

                        if ( line == "<tu>" )
                            tu_started = true;
                        else if ( line == "</tu>" )
                        {
                            tu.append(line);
                            tr_tu.push_back(tu);
                            tu = "";
                            tu_started = false;
                        }
                        else
                        {}

                        if ( tu_started )
                            tu.append(line);
                    }
                }
            }
        }
    }

    /**************************
     * <tu><src>you lost! :P</src><l code="en">You lost!</l><l code="de">Du hast verloren!</l><l code="fr">Tu as perdu!</l>
     *************************/
    void process_tu( std::string tu, LocaleMap& locale )
    {
        size_t src_start_pos = tu.find( "<src>" ) + std::string("<src>").length();
        size_t src_end_pos = tu.find( "</src>" );

        std::string src = tu.substr( src_start_pos, src_end_pos - src_start_pos );
        //std::cout << ">> " << src << std::endl;

        size_t l_start_pos = src_end_pos + std::string("</src>").length();
        size_t l_end_pos = tu.find( "</l>", l_start_pos );
        while ( l_end_pos != std::string::npos )
        {
            std::string l = tu.substr( l_start_pos, l_end_pos - l_start_pos );

            std::string l_code = l.substr( l.find( "\"")+1, 2 );
            std::string l_text = l.substr( l.find( ">" )+1 );
          //  std::cout << ">>>  (" << l_code << ") "  << l_text << std::endl;

            locale[src].push_back(l_text);

            l_start_pos = l_end_pos + std::string("</l>").length();
            l_end_pos = tu.find( "</l>", l_start_pos );
        }
    }
#endif /* READ_TEXTS_FROM_XML */

    /**************************
     *
     *************************/
    void read_translations( LocaleMap& locale )
    {
#ifdef READ_TEXTS_FROM_XML
        std::vector<std::string> tr_tu;
        read_xml( tr_tu );

        for(size_t i=0;i<tr_tu.size();i++)
        {
           // std::cout << "> " << tr_tu[i] << std::endl;

            process_tu( tr_tu[i], locale );
        }
#endif /* READ_TEXTS_FROM_XML */

#ifdef READ_TEXTS_FROM_HEADER
        for(size_t i=0;i<wt_translations.size();i++)
        {
            locale[wt_translations[i].src] = wt_translations[i].texts;
        }
#endif /* READ_TEXTS_FROM_HEADER */        
    }

private:
    size_t                   m_active_language_idx;
    std::vector<std::string> m_languages;
    LocaleMap                m_translations;
};

#endif /* _WT_L10N_H_ */
