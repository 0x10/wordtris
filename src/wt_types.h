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
    WtDim( size_t iW, size_t iH ) :
        w(iW),
        h(iH) {}

    size_t w;
    size_t h;
};


/**************************
 *
 *************************/
class WtCoord
{
public:    
    WtCoord() :
        x(0),
        y(0) {}
    WtCoord( size_t iX, size_t iY ) :
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

    void moveX( const WtDim& dim )
    {
        x = x + dim.w;
    }

    size_t x;
    size_t y;
};

/**************************
 *
 *************************/
class WtInputEvent
{
public:
    WtInputEvent() :
        is_key_event( true ),
        is_motion_event( false ),
        key( wt_control_INVALID ),
        pos()
    {}

    bool       is_key_event;
    bool       is_motion_event;
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
    std::string   language;
    std::string   game_mode;
    wt_difficulty difficulty;

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
