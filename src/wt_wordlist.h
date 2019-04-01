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

#include "wt_wordlist_strings_en.h"
#include "wt_wordlist_strings_en_1k.h"

/**************************************
 *
 *************************************/
class WtWord
{


public:
    WtWord( std::string w ) :
        m_word( w ),
        m_dea( w )
    {
        //std::cout << "=====" <<std::endl << "WtWord = " << w << std::endl;
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

    static const std::vector<std::string>& get_wordlist_by_name( const std::string name )
    {
        if ( name == "short list" )
            return short_list;
        if ( name == "1k list" )
            return long_list;
        else
            return short_list;
    }
public:
    WtWordList()
    {
    }
    WtWordList( const std::vector<std::string>& input_list, EConvertChars conv=eNone )
    {
        for( size_t i = 0; i < input_list.size(); i++ )
        {
            std::string input = input_list[i];
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
    void load_from_list( const std::vector<std::string>& input_list, EConvertChars conv=eNone )
    {
        for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
            delete m_words[w_idx];
        m_words.clear();

        for( size_t i = 0; i < input_list.size(); i++ )
        {
            std::string input = input_list[i];
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

private:
    std::vector<WtWord*> m_words;
};


#endif /* _WT_WORDLIST_H_ */
