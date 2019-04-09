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

typedef enum wt_mode_difficulty_tag
{
    wt_difficulty_EASY,
    wt_difficulty_MEDIUM,
    wt_difficulty_HARD
} wt_difficulty;


typedef enum wt_controls_tag
{
    wt_control_DROP,
    wt_control_LEFT,
    wt_control_RIGHT,
    wt_control_QUIT,
    wt_control_PAUSE,
    wt_control_INVALID
} wt_control;

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

    ssize_t w;
    ssize_t h;
};
std::ostream & operator << (std::ostream &out, const WtDim &d)
{
    out << "(" << d.w << "," << d.h << ")";
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
    WtCoord& operator=( const WtCoord& c )
    {
        x = c.x;
        y = c.y;
        return (*this);
    }
    bool operator==(const WtCoord& rhs) const
    {
        return ( x == rhs.x ) && ( y == rhs.y );
    }
    bool operator!=(const WtCoord& rhs) const
    {
        return ( x != rhs.x ) && ( y != rhs.y );
    }

    WtCoord& operator+( const WtCoord& rhs )
    {
        x = x + rhs.x;
        y = y + rhs.y;
        return (*this);
    }


    bool in_region( WtCoord start_pos, WtDim size ) const
    {
        return (  ( x >= start_pos.x ) && ( x < start_pos.x+size.w )
               && ( y >= start_pos.y ) && ( y < start_pos.y+size.h ) );
    }

    void moveX( const WtDim& dim )
    {
        x = x + dim.w;
    }

    ssize_t x;
    ssize_t y;
};
std::ostream & operator << (std::ostream &out, const WtCoord &c)
{
    out << "(" << c.x << "," << c.y << ")";
}

/**************************
 *
 *************************/
class WtInputEvent
{
public:
    WtInputEvent() :
        is_key_event( true ),
        is_motion_event( false ),
        is_press_event( false ),
        key( wt_control_INVALID ),
        pos(),
        d_pos()
    {}

    bool       is_key_event;
    bool       is_motion_event;
    bool       is_press_event;
    wt_control key;
    WtCoord    pos;
    WtCoord    d_pos;
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
        active_theme( "default" )
    {
    }
    ~WtSettings() {}
    std::string   language;
    std::string   game_mode;
    wt_difficulty difficulty;
    std::string   active_theme;

    /**************************
     *
     *************************/
    void write( std::ofstream& of )
    {
        const char* str = language.c_str();
        of.write( str, strlen(str)+1 );// incl. NULL

        str = game_mode.c_str();
        of.write( str, strlen(str)+1 );// incl. NULL

        uint8_t diff = difficulty;
        of.write( (char*)&diff, 1 );

        str = active_theme.c_str();
        of.write( str, strlen(str)+1 );// incl. NULL
    }

    /**************************
     *
     *************************/
    void read( std::ifstream& inf )
    {
        char input[0xFF];
        inf.getline( input, 0xFF, '\0' );
        language = std::string(input);

        inf.getline( input, 0xFF, '\0' );
        game_mode = std::string(input);

        uint8_t diff;
        inf.read( (char*)&diff, 1 );
        difficulty = (wt_difficulty)diff;

        inf.getline( input, 0xFF, '\0' );
        active_theme= std::string(input);
    }    
};


/**************************
 *
 *************************/
class WtScoreEntry
{
public:
    std::string player;
    std::string game_mode;
    size_t      score;

    /**************************
     *
     *************************/
    void write( std::ofstream& of )
    {
        const char* str = player.c_str();
        of.write( str, strlen(str)+1 ); // incl. NULL

        str = game_mode.c_str();
        of.write( str, strlen(str)+1 );// incl. NULL

        of.write( (char*)&score, sizeof(score) );
    }

    /**************************
     *
     *************************/
    bool read( std::ifstream& inf )
    {
        char input[0xFF];

        if (inf.eof()) return false;

        inf.getline( input, 0xFF, '\0' );
        player = std::string( input );

        if (inf.eof()) return false;

        inf.getline( input, 0xFF, '\0' );
        game_mode = std::string( input );

        if (inf.eof()) return false;

        inf.read( (char*)&score, sizeof(score) );

        return true;
    }
};

typedef std::vector<WtScoreEntry> WtHighscores;

#endif /* _WT_TYPES_H */
