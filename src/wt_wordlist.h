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

#include "wt_l10n.h"
#include "dea.h"

/**************************************
 *
 *************************************/
class WtWord
{
public:
    WtWord( const std::string w ) :
        m_word( w ),
        m_dea( w )
    {
      //  std::cout << "=====" <<std::endl << "WtWord = " << w << std::endl;
       // m_dea.print();
    }
    ~WtWord()
    {
    }

public:
    /**************************************
     *
     *************************************/
    void search_update( const char symbol )
    {
        m_dea.process_symbol( symbol );
    }

    /**************************************
     *
     *************************************/
    bool search_found()
    {
        return m_dea.is_current_state_accepting();
    }

    /**************************************
     *
     *************************************/
    void search_reset()
    {
        m_dea.init();
    }

    /**************************************
     *
     *************************************/
    std::string as_string()
    {
        return m_word;
    }

private:
    std::string m_word;
    TDea        m_dea;
};

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


public:
    WtWordList( const std::string input_list_name="", EConvertChars conv=eNone ) :
        m_words(),
        m_list_short(),
        m_list_medium(),
        m_list_large(),
        m_list_xlarge()
    {
        load_lists();

        if ( ! input_list_name.empty() )
        {
            const std::vector<std::string>& input_list = get_wordlist_by_name( input_list_name );
            for( size_t i = 0; i < input_list.size(); i++ )
            {
                std::string input = input_list[i];
                if ( !input.empty() )
                {
                    switch( conv )
                    {
                        case eToLower: std::transform(input.begin(), input.end(), input.begin(), ::tolower); break;
                        case eToUpper: std::transform(input.begin(), input.end(), input.begin(), ::toupper); break;
                        default: break;
                    }

                    WtWord* w = new WtWord( input );
                    m_words.push_back(w);
                }
            }
        }
    }
    ~WtWordList()
    {
        for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
            delete m_words[w_idx];
        m_words.clear();
    }

public:
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
        const std::vector<std::string>& input_list = get_wordlist_by_name( input_list_name );

        for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
            delete m_words[w_idx];
        m_words.clear();

        for( size_t i = 0; i < input_list.size(); i++ )
        {
            std::string input = input_list[i];
            if ( !input.empty() )
            {
                switch( conv )
                {
                    case eToLower: std::transform(input.begin(), input.end(), input.begin(), ::tolower); break;
                    case eToUpper: std::transform(input.begin(), input.end(), input.begin(), ::toupper); break;
                    default: break;
                }

                WtWord* w = new WtWord( input );
                m_words.push_back(w);
            }
        }
    }

    /**************************************
     *
     *************************************/
    std::vector<std::string> get_contained_words( std::string sequence )
    {
        std::vector<std::string> found_words;
        for( size_t idx = 0; idx<sequence.length(); idx++ )
        {
            for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
            {
//                std::cout << idx << " update on " << m_words[w_idx]->as_string() << std::endl;
                m_words[w_idx]->search_update( sequence[idx] );
            }
        }
        for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
        {
            if ( m_words[w_idx]->search_found() )
            {
                found_words.push_back( m_words[w_idx]->as_string() );
            }
            m_words[w_idx]->search_reset();
        }
        return found_words;
    }

    /**************************************
     *
     *************************************/
    const std::vector<std::string>& get_wordlist_by_name( const std::string name )
    {
        if ( name == "short list" )
            return m_list_short;
        if ( name == "1k list" )
            return m_list_medium;
        if ( name == "5k list" )
            return m_list_large;
        if ( name == "20k list" )
            return m_list_xlarge;
        else
            return m_list_short;
    }

private:
    /**************************************
     *
     *************************************/
    void load_lists()
    {
        load_list_from_file( "short.txt", WtL10n::get_language_code(), m_list_short );
        load_list_from_file( "medium.txt", WtL10n::get_language_code(), m_list_medium );
        load_list_from_file( "large.txt", WtL10n::get_language_code(), m_list_large );
        load_list_from_file( "xlarge.txt", WtL10n::get_language_code(), m_list_xlarge );
    }

    /**************************************
     *
     *************************************/
    void load_list_from_file( const std::string list_name, 
                              const std::string language, 
                              std::vector<std::string>& list )
    {
        std::string fname( "wordlists/" );
        fname.append(language).append("/").append(list_name);
        std::cout << "try to load wordlist at \"" << fname << "\"" << std::endl;
        std::vector<uint8_t>& file_content = STORAGE.get_asset_file_buf( fname );
        // process....
        std::string str(file_content.begin(), file_content.end());
        std::stringstream ss(str);
        for (std::string each; std::getline(ss, each, '\n'); list.push_back(each));
    }

private:
    std::vector<WtWord*>     m_words;
    std::vector<std::string> m_list_short;
    std::vector<std::string> m_list_medium;
    std::vector<std::string> m_list_large;
    std::vector<std::string> m_list_xlarge;
};


#endif /* _WT_WORDLIST_H_ */
