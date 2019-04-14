/*!*****************************************************************************
 * @file wt_storage_types.h
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
#ifndef _WT_STORAGE_TYPES_H_
#define _WT_STORAGE_TYPES_H_

#include "wt_storable.h"

typedef enum wt_mode_difficulty_tag
{
    wt_difficulty_EASY,
    wt_difficulty_MEDIUM,
    wt_difficulty_HARD
} wt_difficulty;

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
    {}

    virtual ~WtSettings() {}
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
    virtual ~WtScoreEntry() {}

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

#endif /* _WT_STORAGE_TYPES_H */
