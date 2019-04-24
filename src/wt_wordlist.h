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

    WtWordList( const std::string input_list_name="", EConvertChars conv=eNone ) :
        m_words(),
        m_list_fname(input_list_name),
        m_conv( conv ),
        m_list()
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
        return m_list;
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
            std::vector<std::string>& input_list = m_list;
            m_list_fname = input_list_name;
            m_conv = conv;
            m_words.clear();
            load_list_from_file( input_list_name, WtL10n::get_language_code(), input_list );

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

                    m_words.push_back( WtWord( input ) );
                }
            }
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
    std::vector<std::string> get_contained_words( const std::string sequence, const size_t min_length )
    {
        std::vector<std::string> found_words;
        for( size_t idx = 0; idx<sequence.length(); idx++ )
        {
            for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
            {
//                std::cout << idx << " update on " << m_words[w_idx].as_string() << std::endl;
                m_words[w_idx].search_update( sequence[idx] );
            }
        }
        for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
        {
            if ( m_words[w_idx].search_found() && ( m_words[w_idx].as_string().length() >= min_length ) )
            {
                found_words.push_back( m_words[w_idx].as_string() );
            }
            m_words[w_idx].search_reset();
        }
        return found_words;
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
                              std::vector<std::string>& list )
    {
        list.clear();
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
    std::vector<WtWord>      m_words;
    std::string              m_list_fname;
    EConvertChars            m_conv;
    std::vector<std::string> m_list;
};


#endif /* _WT_WORDLIST_H_ */
