/*!*****************************************************************************
 * @file wt_random.h
 * @brief helper class which provides random functions
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
#ifndef _WT_RANDOM_H_
#define _WT_RANDOM_H_

#include <time.h>
#include <cstdlib>
#include <chrono>
#include <random>
#include <string>

#define LETTER_COUNT  (26)
#define MAX_WEIGHT    (351)

class WtRandom
{
public:
    /******************************************************************************
     *
     *****************************************************************************/
    static size_t get_random_from_sequence( std::vector<size_t>& seq )
    {
        char result = 0;
        uint8_t buf;
        if ( WtRandom::getrandom( &buf, 1 ) <= 1 )
        {
            result = seq[(buf % seq.size() )];
        }
        return result;
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static char get_random_letter_of_weight_seq( const std::string word, std::discrete_distribution<int> dist  )
    {
        char result = '\0';
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        int random = dist( generator );

        result = word[(random % word.length())];

/*
        size_t max_weight = (word.length()+1) * (word.length()/2);

        ssize_t buf;
        if ( WtRandom::getrandom( (uint8_t*)&buf, sizeof(size_t) ) <= (ssize_t)sizeof(size_t) )
        {
            buf = (buf % max_weight);
            uint8_t i;
            for ( i = 0; i < word.length(); i++ )
            {
                buf = buf - (word.length() - i);
                if ( buf <= 0 )
                {
                    result = word.at(i);
                    break;
                }
            }
        }
*/
        return result;
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static char get_random_letter_of_word( std::string word )
    {
        char result = 0;
        uint8_t buf;
        if ( WtRandom::getrandom( &buf, 1 ) <= 1 )
        {
            result = word[(buf % word.length() )];
        }
        return result;
    }


    /******************************************************************************
     *
     *****************************************************************************/
    static char get_next_random_letter()
    {
//      return get_weighted_random_letter();
        return get_purely_random_letter();
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static uint8_t roll_dice( uint8_t dice )
    {
        uint8_t result = 0;
        uint8_t buf;
        if ( WtRandom::getrandom( &buf, 1 ) == 1 )
        {
            result = 1+(buf % dice);
        }
        return result;
    }
private:
    /******************************************************************************
     *
     *****************************************************************************/
    static char get_weighted_random_letter( void )
    {
        const char letters[LETTER_COUNT] = 
                     { 'e','t','a','o','i','n','s','r','h','d','l','u','c',
                       'm','f','y','w','g','p','b','v','k','x','q','j','z' };
        char result = 0;
        int16_t buf;
        if ( WtRandom::getrandom( (uint8_t*)&buf, 2 ) <= 2 )
        {
            buf = (buf % MAX_WEIGHT);
            uint8_t i;
            for ( i = 0; i < LETTER_COUNT; i++ )
            {
                buf = buf - (LETTER_COUNT - i);
                if ( buf <= 0 )
                {
                    result = letters[i];
                    break;
                }
            }
        }
        return result;
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static char get_purely_random_letter( void )
    {
        char result = 0;
        uint8_t buf;
        if ( WtRandom::getrandom( &buf, 1 ) <= 1 )
        {
            result = (char)((uint8_t)'a' + (buf % ((uint8_t)'z' - (uint8_t)'a') ) );
        }
        return result;
    }
public:
    /******************************************************************************
     * this wrapper may solve some issues on some systems...
     *****************************************************************************/
    static ssize_t getrandom( uint8_t* buf, size_t read )
    {
        struct timespec time;

        clock_gettime(CLOCK_MONOTONIC, &time);
        srand( time.tv_nsec );
        for ( size_t i=0; i<read; i++ )
            buf[i] = rand();
        return read;
    }
};

#endif /* _WT_RANDOM_H */
