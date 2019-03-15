/*!*****************************************************************************
 * @file wt_game_mode_if.h
 * @brief declaration of game mode interface and one example implemenatation
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
#ifndef _WT_GAME_MODE_IF_H_
#define _WT_GAME_MODE_IF_H_

#include <string>
#include <vector>
#include <utility>

#include "global.h"

#include "wt_random.h"
#include "wt_player.h"
#include "wt_board.h"

#define INVALID_GAME_MODE (WtGameModeIf*)(0)



class WtGameModeIf
{
public:
    /**************************
     * 
     *************************/
    static std::vector<std::pair<wt_difficulty, std::string> > get_available_difficulties()
    {
        std::vector<std::pair<wt_difficulty, std::string> > result =
                         { std::make_pair( wt_difficulty_EASY,   WtL10n::tr("easy") ), 
                           std::make_pair( wt_difficulty_MEDIUM, WtL10n::tr("normal") ),
                           std::make_pair( wt_difficulty_HARD,   WtL10n::tr("hard") ) };
        return result;
    }

    /**************************
     * 
     *************************/
    virtual void init_game( WtBoard& board, WtPlayer& player ) {}

    /**************************
     *
     *************************/
    virtual std::string get_title()=0;

    /**************************
     *
     *************************/
    virtual std::string get_id_string()=0;

    /**************************
     * return false if game over
     *************************/
    virtual bool eval_board( WtBoard& board, WtPlayer& player )=0;

    /**************************
     *
     *************************/
    virtual char next_letter()=0;

    /**************************
     *
     *************************/
    virtual std::string get_hint()=0;

    /**************************
     *
     *************************/
    virtual std::string get_help()
    {
        return WtL10n::tr("no help provided");
    }

    /**************************
     *
     *************************/
    void set_difficulty( wt_difficulty new_difficulty )
    {
        m_current_diff = new_difficulty;
    }

    /**************************
     *
     *************************/
    virtual wt_difficulty get_difficulty()
    {
        return m_current_diff;
    }

private:
    wt_difficulty m_current_diff;
};

class WtGameModeDemo : public WtGameModeIf
{
public:
    WtGameModeDemo() :
        m_active_wordbase( "12340()()()()#@abcdEFGK" )
    {
    }
    ~WtGameModeDemo()
    {
    }

    /**************************
     * 
     *************************/
    virtual void init_game( WtBoard& board, WtPlayer& player )
    {
        for( size_t r_idx = 0; r_idx < WtBoard::row_count/2+1; r_idx++ )
            for( size_t c_idx = 0; c_idx < WtBoard::col_count; c_idx++ )
            {
                uint8_t dice = WtRandom::roll_dice(6);
                if ( dice > 5 )
                    board.set_cell( r_idx, c_idx, WtRandom::get_random_letter_of_word( m_active_wordbase ) );
            }

        player.set_level_offset(9);
    }

    /**************************
     *
     *************************/
    virtual std::string get_title()
    {
        return "Demo";
    }

    /**************************
     *
     *************************/
    virtual std::string get_id_string()
    {
        //shall not be translated
        return "Demo";
    }

    /**************************
     * return false if game over
     *************************/
    virtual bool eval_board( WtBoard& board, WtPlayer& player )
    {
        return true;
    }

    /**************************
     *
     *************************/
    virtual char next_letter()
    {
        return WtRandom::get_random_letter_of_word( m_active_wordbase ); 
    }

    /**************************
     *
     *************************/
    virtual std::string get_hint()
    {
        std::string s_hint( "dEmO MoDe" );
        return s_hint;
    }
private:
    WtGameModeDemo( const WtGameModeDemo& ); 
    WtGameModeDemo & operator = (const WtGameModeDemo &);



private:
    std::string m_active_wordbase;
};


#endif /* _WT_GAME_MODE_IF_H_ */
