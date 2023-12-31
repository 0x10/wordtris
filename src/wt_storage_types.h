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
        language( "de" ),
        game_mode( "" ),
        difficulty( wt_difficulty_EASY ),
        active_theme( "default" ),
        show_support_grid( true ),
        show_next_stone( true ),
        enable_audio( true )
    {}

    virtual ~WtSettings() {}
    std::string   language;
    std::string   game_mode;
    wt_difficulty difficulty;
    std::string   active_theme;
    bool          show_support_grid;
    bool          show_next_stone;
    bool          enable_audio;

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
        WtStorable::write_boolean( of, enable_audio );
    }

    /**************************
     *
     *************************/
    bool read( std::ifstream& inf )
    {
        bool was_eof = false;

        language = WtStorable::read_string( inf, was_eof );
        game_mode = WtStorable::read_string( inf, was_eof );
        difficulty = WtStorable::read_unsigned<wt_difficulty>( inf, was_eof );
        active_theme = WtStorable::read_string( inf, was_eof );
        show_support_grid = WtStorable::read_boolean( inf, was_eof );
        show_next_stone = WtStorable::read_boolean( inf, was_eof );
        enable_audio = WtStorable::read_boolean( inf, was_eof );

        return ( ! was_eof );
    }

    /**************************
     *
     *************************/
    size_t size() const
    {
        size_t size = 0;
        size += WtStorable::get_storage_size( language );
        size += WtStorable::get_storage_size( game_mode );
        size += WtStorable::get_storage_size( difficulty );
        size += WtStorable::get_storage_size( active_theme );
        size += WtStorable::get_storage_size( show_next_stone );
        size += WtStorable::get_storage_size( show_support_grid );
        size += WtStorable::get_storage_size( enable_audio );
        return size;
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
        bool was_eof = false;

        player = WtStorable::read_string( inf, was_eof );
        game_mode = WtStorable::read_string( inf, was_eof );
        score = WtStorable::read_unsigned<size_t>( inf, was_eof );
        level = WtStorable::read_unsigned<size_t>( inf, was_eof );

        return ( ! was_eof );
    }
};

typedef std::vector<WtScoreEntry> WtHighscores;

/**************************
 *
 *************************/
class WtAchievement
{
public:
    WtAchievement( std::string mode = "",
                   std::string achievement_name = "",
                   bool        reached = false ) :
        game_mode( mode ),
        achievement( achievement_name ),
        acomplished( reached )
    {}
    virtual ~WtAchievement() {}

    std::string game_mode;
    std::string achievement;
    bool        acomplished;

    /**************************
     *
     *************************/
    void write( std::ofstream& of )
    {
        WtStorable::write_string( of, game_mode );
        WtStorable::write_string( of, achievement );
        WtStorable::write_boolean( of, acomplished );
    }

    /**************************
     *
     *************************/
    bool read( std::ifstream& inf )
    {
        bool was_eof = false;

        game_mode = WtStorable::read_string( inf, was_eof );
        achievement = WtStorable::read_string( inf, was_eof );
        acomplished = WtStorable::read_boolean( inf, was_eof );

        return ( ! was_eof );
    }
};

typedef std::vector<WtAchievement> WtAchievements;

#endif /* _WT_STORAGE_TYPES_H */
