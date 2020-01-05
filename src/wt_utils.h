/*!*****************************************************************************
 * @file wt_utils.h
 * @brief helper class which provides several common functions
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
#ifndef _WT_UTILS_H_
#define _WT_UTILS_H_

#include <time.h>
#include <cstdlib>
#include <chrono>
#include <random>
#include <string>
#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>

#define LETTER_COUNT  (26)
#define MAX_WEIGHT    (351)

/*******************************
 *
 ******************************/
class WtTime
{
public:
    using TimePoint = std::chrono::steady_clock::time_point;
    using TimeType  = std::chrono::microseconds;

    /******************************************************************************
     *
     *****************************************************************************/
    static TimeType from_seconds( const size_t s )
    {
        return std::chrono::duration_cast<TimeType>( std::chrono::seconds( s ) );
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static TimeType from_milliseconds( const size_t s )
    {
        return std::chrono::duration_cast<TimeType>( std::chrono::milliseconds( s ) );
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static TimePoint get_time()
    {
        return std::chrono::steady_clock::now();
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static TimeType get_time_elapsed( TimePoint start, TimePoint end )
    {
        return std::chrono::duration_cast<TimeType>( end - start );
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static struct timespec get_timespec()
    {
        struct timespec time;

        clock_gettime(CLOCK_MONOTONIC, &time);

        return time;
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static void sleep( TimeType t )
    {
        std::this_thread::sleep_for( t );
    }

    /******************************************************************************
     *
     *****************************************************************************/
    static std::string format_time( TimeType t )
    {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << std::chrono::duration_cast<std::chrono::minutes>(t).count() << ":";
        ss << std::setfill('0') << std::setw(2) << ( std::chrono::duration_cast<std::chrono::seconds>(t).count() % 60 );
        return ss.str();
    }
};

/*******************************
 *
 ******************************/
class WtRandom
{
public:
    /******************************************************************************
     *
     *****************************************************************************/
    template<typename itemtype>
    static itemtype get_random_from_sequence( std::vector<itemtype>& seq, size_t* idx=nullptr )
    {
        itemtype result;
        uint8_t buf;
        if ( WtRandom::getrandom( &buf, 1 ) <= 1 )
        {
            size_t index = buf % seq.size();
            result = seq[index];
            if ( idx != nullptr ) *idx = index;
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
        size_t random = static_cast<size_t>(dist( generator ));

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
#if 0
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
#endif
    /******************************************************************************
     *
     *****************************************************************************/
    static char get_purely_random_letter( void )
    {
        char result = 0;
        uint8_t buf;
        if ( WtRandom::getrandom( &buf, 1 ) <= 1 )
        {
            result = static_cast<char>(static_cast<uint8_t>('a') + (buf % (static_cast<uint8_t>('z') - static_cast<uint8_t>('a') ) ) );
        }
        return result;
    }
public:
    /******************************************************************************
     * this wrapper may solve some issues on some systems...
     *****************************************************************************/
    static size_t getrandom( uint8_t* buf, size_t read )
    {
        srand( WtTime::get_timespec().tv_nsec );
        for ( size_t i=0; i<read; i++ )
            buf[i] = rand();
        return read;
    }
};

#endif /* _WT_UTILS_H */
