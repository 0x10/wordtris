/*******************************************************************************
 * @file contains.c
 * @brief application of dea unit which implements a contains search logic
 *        using the dea lib
 *
 * @author Christian Kranz
 *
 * @copyright Copyright 2018 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz.
 ******************************************************************************/

#include "dea_wrapper.h"
#include <string>
#include <vector>
#include <iostream>


/**************************************
 *
 *************************************/
class Word
{
public:
    Word( std::string w ) :
        m_word( w )
    {
        m_dea = DeaWrapper::construct_contains( w );
    }
    ~Word()
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
class WordList
{
public:
    WordList( std::vector<std::string>& input_list )
    {
        for( size_t i = 0; i < input_list.size(); i++ )
        {
            Word* w = new Word( input_list[i] );
            m_words.push_back(w);
        }
    }
    ~WordList()
    {
        for( size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
            delete m_words[w_idx];
        m_words.clear();
    }

public:

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
    std::vector<Word*> m_words;
};


int main( int argc, const char* argv[] )
{

    if ( argc > 1 )
    {
        std::vector<std::string> list = { "hallo", "traum",
                                          "tot", "baum",
                                          "heu", "heute" };
    
        WordList myList = WordList( list );
        std::vector<std::string> found_list = myList.get_contained_words( argv[ 1 ] );
        for( size_t i = 0; i < found_list.size(); i++ )
            std::cout << "("<<i<<") "<< found_list[i] << std::endl;

    }


    return 0;
}
