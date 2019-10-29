/*!*****************************************************************************
 * @file wt_wordlist.h
 * @brief implementation of word list helper
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
#ifndef _WT_WORDLIST_H_
#define _WT_WORDLIST_H_

#include "global.h"

#include "dea.h"

/**************************************
 *
 *************************************/
class WtWordList
{
public:
    typedef enum {
        eToLower,
        eToUpper,
        eNone,
    } EConvertChars;

    typedef std::map<std::string, std::vector<std::string> > SequenceCache;

    WtWordList( const std::string input_list_name="", EConvertChars conv=eNone ) :
        m_words(),
        m_list_fname(input_list_name),
        m_conv( conv ),
        m_cache_min_len(0),
        m_cache_max_len(0),
        m_list_cache(),
        m_contains_dea()
    {
        if ( ! input_list_name.empty() )
            load_from_list( input_list_name, conv );

        WtL10n::register_lang_change_obsever( WT_BIND_EVENT_HANDLER( WtWordList::language_changed ) );
    }
    ~WtWordList()
    {
    }

    /**************************************
     *
     *************************************/
    operator const std::vector<std::string>&()
    {
        return m_words;
    }

    /**************************************
     *
     *************************************/
    size_t size() const
    {
        return m_words.size();
    }

    /**************************************
     *
     *************************************/
    void load_from_list( const std::string input_list_name, EConvertChars conv=eNone )
    {
        if ( ! input_list_name.empty() )
        {
            std::vector<std::string>& input_list = m_words;
            m_list_fname = input_list_name;
            m_conv = conv;
            m_words.clear();
            load_list_from_file( input_list_name, WtL10n::get_language_code(), input_list, conv );
        }
        else
        {
            m_list_fname = "";
            m_conv = eNone;
            m_words.clear();
        }
    }

    /**************************************
     *
     *************************************/
    const std::vector<std::string>& get_word_list( const size_t min_len, const size_t max_len )
    {
        if ( ( m_cache_min_len == min_len ) && ( m_cache_max_len == max_len ) )
        {
            // nothing to do
            std::cout << "cache hit word list request << (" << min_len << ", " << max_len << ")\n";
        }
        else
        {
            m_list_cache.clear();
            for( size_t i = 0; i < m_words.size(); i++ )
            {
                if (( m_words[i].length() >= min_len ) && ( m_words[i].length() <= max_len ))
                {
                    m_list_cache.push_back( m_words[i] );
                }
            }
            std::cout << "cache miss word list request << ("<< m_list_cache.size()  << ")\n";
            m_cache_max_len = max_len;
            m_cache_min_len = min_len;
        }
        if ( m_list_cache.size() == 0 )
        {
            m_list_cache = m_words;
        }

        return m_list_cache;
     }

    /**************************************
     *
     *************************************/
    std::vector<std::string> get_contained_words( const std::string sequence, const size_t min_length )
    {
        std::vector<std::string> result_words;
        //std::cout << "search in " << sequence << std::endl;
        std::vector<ssize_t> result = m_contains_dea.find_in_string_multipass( sequence );
        std::vector<ssize_t>::iterator ip; 
        // Sorting the array 
        std::sort(result.begin(), result.end()); 
        // Now v becomes 1 1 2 2 3 3 3 3 7 7 8 10 

        // Using std::unique 
        ip = std::unique(result.begin(), result.end());
        // Now v becomes {1 2 3 7 8 10 * * * * * *} 
        // * means undefined 

        // Resizing the vector so as to remove the undefined terms 
        result.resize(static_cast<size_t>(std::distance(result.begin(), ip))); 

        for( ssize_t index : result )
        {
            if ( index >= 0 )
            {
                size_t uidx = static_cast<size_t>(index);
                //std::cout << "FOUND = " << m_words[index] << std::endl;
                if ( m_words[uidx].length() >= min_length )
                    result_words.push_back( m_words[uidx] );
            }
        }

        return result_words;
    }


private:
    /**************************************
     *
     *************************************/
    void language_changed()
    {
        load_from_list( m_list_fname, m_conv );
    }

    /**************************************
     *
     *************************************/
    void load_list_from_file( const std::string list_name, 
                              const std::string language, 
                              std::vector<std::string>& list, EConvertChars conv=eNone  )
    {
        list.clear();
        std::string fname( "wordlists/" );
        fname.append(language).append("/").append(list_name);
        std::cout << "try to load wordlist at \"" << fname << "\"" << std::endl;
        std::vector<uint8_t>& file_content = STORAGE.get_asset_file_buf( fname );
        // process....
        std::string str(file_content.begin(), file_content.end());
        std::stringstream ss(str);
        for (std::string each; std::getline(ss, each, '\n'); )
        {
            if ( !each.empty() )
            {
                //if ( std::find(list.begin(), list.end(), each) == list.end() )
                {
                    switch( conv )
                    {
                        case eToLower: std::transform(each.begin(), each.end(), each.begin(), ::tolower); break;
                        case eToUpper: std::transform(each.begin(), each.end(), each.begin(), ::toupper); break;
                        default: break;
                    }

                    list.push_back(each);
                    m_contains_dea.else_contains( each, list.size()-1 );
                }
            }
        }
    }

private:
    std::vector<std::string> m_words;
    std::string              m_list_fname;
    EConvertChars            m_conv;
    size_t                   m_cache_min_len;
    size_t                   m_cache_max_len;
    std::vector<std::string> m_list_cache;
    DeaImproved              m_contains_dea;
};

#endif /* _WT_WORDLIST_H_ */
