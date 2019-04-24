/*!*****************************************************************************
 * @file wt_storable.h
 * @brief static only class for stream r/w op wrapper
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
#ifndef _WT_STORABLE_H_
#define _WT_STORABLE_H_

#include <cstring> //for strlen
#include <iostream>

/**************************
 *
 *************************/
class WtStorable
{
public:
    /**************************
     *
     *************************/
    static void write_string( std::ofstream& of, const std::string& string )
    {
        const char* str = string.c_str();
        of.write( str, static_cast<std::streamsize>(strlen(str) + 1) );// incl. NULL
    }

    /**************************
     *
     *************************/
    static std::string read_string( std::ifstream& inf, bool& was_eof )
    {
        char input[0xFF] = {0};

        was_eof = ( was_eof || inf.eof() );
        if ( ! was_eof )
            inf.getline( input, 0xFF, '\0' );

        return std::string(input);
    }

    /**************************
     *
     *************************/
    template<typename unsignedtype>
    static void write_unsigned( std::ofstream& of, const unsignedtype val )
    {
        union {
            unsignedtype uint;
            char   bytes[sizeof(unsignedtype)];
        } size_t_conv;

        size_t_conv.uint = val;

        of.write( size_t_conv.bytes, sizeof(unsignedtype) );
    }

    /**************************
     *
     *************************/
    template<typename unsignedtype>
    static unsignedtype read_unsigned( std::ifstream& inf, bool& was_eof )
    {
        union {
            unsignedtype uint;
            char   bytes[sizeof(unsignedtype)];
        } size_t_conv;

        was_eof = ( was_eof || inf.eof() );
        if ( ! was_eof )
            inf.read( size_t_conv.bytes, sizeof(unsignedtype) );

        return size_t_conv.uint;
    }


    /**************************
     *
     *************************/
    static void write_boolean( std::ofstream& of, const bool& flag )
    {
        char bflag = '0';

        if ( flag )
        {
            bflag = '1';
        }

        of.write( &bflag, 1 );
    }

    /**************************
     *
     *************************/
    static bool read_boolean( std::ifstream& inf, bool& was_eof )
    {
        char bflag='0';

        was_eof = ( was_eof || inf.eof() );
        if ( ! was_eof )
            inf.read( &bflag, 1 );

        return ( bflag == '1' );
    }
};



#endif /* _WT_STORABLE_H */
