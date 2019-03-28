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

#include "dea_wrapper.h"


/**************************************
 *
 *************************************/
class WtWord
{
public:
    WtWord( std::string w ) :
        m_word( w )
    {
        m_dea = DeaWrapper::construct_contains( w );
        
        std::cout << "=====" <<std::endl << "WtWord = " << w << std::endl;
        DeaWrapper::print( m_dea );
    }
    ~WtWord()
    {
        if ( NULL != m_dea )
        {
            DeaWrapper::free( m_dea );
        }
    }

public:
    /**************************************
     *
     *************************************/
    void search_update( const char symbol )
    {
        if ( ( NULL != m_dea )
            && ( NULL != m_dea->current_state ) )
        {
            DeaWrapper::process( m_dea, symbol );
        }
    }

    /**************************************
     *
     *************************************/
    bool search_found()
    {
        return ( ACCEPTING == m_dea->current_state->is_accepting );
    }

    /**************************************
     *
     *************************************/
    void search_reset()
    {
        if ( NULL != m_dea )
            DeaWrapper::init( m_dea );
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
    dea_t*      m_dea;
};

/**************************************
 *
 *************************************/
class WtWordList
{
public:
    WtWordList()
    {
    }
    WtWordList( const std::vector<std::string>& input_list )
    {
        for( size_t i = 0; i < input_list.size(); i++ )
        {
            WtWord* w = new WtWord( input_list[i] );
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
    void load_from_list( const std::vector<std::string>& input_list )
    {
        for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
            delete m_words[w_idx];
        m_words.clear();

        for( size_t i = 0; i < input_list.size(); i++ )
        {
            WtWord* w = new WtWord( input_list[i] );
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
                std::cout << idx << " update on " << m_words[w_idx]->as_string() << std::endl;
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
