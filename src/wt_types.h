/*!*****************************************************************************
 * @file wt_types.h
 * @brief some common type declarations needed within multiple units
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
#ifndef _WT_TYPES_H_
#define _WT_TYPES_H_

#include <cstring> //for strlen
#include <iostream>
#include <math.h>

typedef enum wt_mode_difficulty_tag
{
    wt_difficulty_EASY,
    wt_difficulty_MEDIUM,
    wt_difficulty_HARD
} wt_difficulty;


/**************************
 *
 *************************/
class WtDim
{
public:
    WtDim() :
        w(0),
        h(0) {}
    WtDim( ssize_t iW, ssize_t iH ) :
        w(iW),
        h(iH) {}
    bool operator!=(const WtDim& rhs) const
    {
        return ( w != rhs.w ) && ( h != rhs.h );
    }

    ssize_t w;
    ssize_t h;
};
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtDim &d);
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtDim &d)
{
    out << "(" << d.w << "," << d.h << ")";
    return out;
}

/**************************
 *
 *************************/
class WtCoord
{
public:    
    WtCoord() :
        x(0),
        y(0) {}
    WtCoord( ssize_t iX, ssize_t iY ) :
        x(iX),
        y(iY) {}
    WtCoord( const WtCoord& c ) :
        x(c.x),
        y(c.y) {}
    /**************************
     *
     *************************/
    WtCoord& operator=( const WtCoord& c )
    {
        x = c.x;
        y = c.y;
        return (*this);
    }
    /**************************
     *
     *************************/
    bool operator==(const WtCoord& rhs) const
    {
        return ( x == rhs.x ) && ( y == rhs.y );
    }
    /**************************
     *
     *************************/
    bool operator!=(const WtCoord& rhs) const
    {
        return ( x != rhs.x ) && ( y != rhs.y );
    }
    /**************************
     *
     *************************/
    WtCoord operator+( const WtCoord& rhs )
    {
        x = x + rhs.x;
        y = y + rhs.y;
        return (*this);
    }
    /**************************
     *
     *************************/
    bool near_point( WtCoord pos, size_t distance ) const
    {
        bool result = false;
        ssize_t dist = (( x - pos.x ) * ( x - pos.x )) + (( y - pos.y ) * ( y - pos.y ));
        size_t sqrt_dist = sqrt( dist );
        //std::cout << "dist = " << dist << "; sqrt_dist = " << sqrt_dist << std::endl;
        if ( sqrt_dist  <= distance )
            result = true;
        return result;
    }
    /**************************
     *
     *************************/
    bool in_region( WtCoord start_pos, WtDim size ) const
    {
        return (  ( x >= start_pos.x ) && ( x < start_pos.x+size.w )
               && ( y >= start_pos.y ) && ( y < start_pos.y+size.h ) );
    }
    /**************************
     *
     *************************/
    void moveX( const WtDim& dim )
    {
        x = x + dim.w;
    }

    ssize_t x;
    ssize_t y;
};
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtCoord &c);
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtCoord &c)
{
    out << "(" << c.x << "," << c.y << ")";
    return out;
}


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
    static std::string read_string( std::ifstream& inf )
    {
        char input[0xFF];
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
    static unsignedtype read_unsigned( std::ifstream& inf )
    {
        union {
            unsignedtype uint;
            char   bytes[sizeof(unsignedtype)];
        } size_t_conv;

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
    static bool read_boolean( std::ifstream& inf )
    {
        char bflag;
        inf.read( &bflag, 1 );

        return ( bflag == '1' );
    }
};


/**************************
 *
 *************************/
class WtSettings
{
public:
    WtSettings() :
        language( "en" ),
        game_mode( "" ),
        difficulty( wt_difficulty_EASY ),
        active_theme( "default" ),
        show_support_grid( false ),
        show_next_stone( false )
    {
    }
    ~WtSettings() {}
    std::string   language;
    std::string   game_mode;
    wt_difficulty difficulty;
    std::string   active_theme;
    bool          show_support_grid;
    bool          show_next_stone;

    /**************************
     *
     *************************/
    void write( std::ofstream& of )
    {
        WtStorable::write_string( of, language );
        WtStorable::write_string( of, game_mode );
        WtStorable::write_unsigned<wt_difficulty>( of, difficulty );
        WtStorable::write_string( of, active_theme );
        WtStorable::write_boolean( of, show_support_grid );
        WtStorable::write_boolean( of, show_next_stone );
    }

    /**************************
     *
     *************************/
    void read( std::ifstream& inf )
    {
        language = WtStorable::read_string( inf );
        game_mode = WtStorable::read_string( inf );
        difficulty = WtStorable::read_unsigned<wt_difficulty>( inf );
        active_theme = WtStorable::read_string( inf );
        show_support_grid = WtStorable::read_boolean( inf );
        show_next_stone = WtStorable::read_boolean( inf );
    }
};


/**************************
 *
 *************************/
class WtScoreEntry
{
public:
    WtScoreEntry( std::string mode="",
                  size_t      points=0,
                  size_t      lvl=0,
                  std::string pl="PLYR" ) :
        player(pl),
        game_mode(mode),
        score(points),
        level(lvl)
    {}

    std::string player;
    std::string game_mode;
    size_t      score;
    size_t      level;

    /**************************
     *
     *************************/
    void write( std::ofstream& of )
    {
        WtStorable::write_string( of, player );
        WtStorable::write_string( of, game_mode );
        WtStorable::write_unsigned<size_t>( of, score );
        WtStorable::write_unsigned<size_t>( of, level );
    }

    /**************************
     *
     *************************/
    bool read( std::ifstream& inf )
    {
        if (inf.eof()) return false;

        player = WtStorable::read_string( inf );

        if (inf.eof()) return false;

        game_mode = WtStorable::read_string( inf );

        if (inf.eof()) return false;
        
        score = WtStorable::read_unsigned<size_t>( inf );

        if (inf.eof()) return false;
        
        level = WtStorable::read_unsigned<size_t>( inf );

        return true;
    }
};

typedef std::vector<WtScoreEntry> WtHighscores;

#endif /* _WT_TYPES_H */
