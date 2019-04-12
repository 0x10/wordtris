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

#include <utility>

#include "wt_utils.h"
#include "wt_player.h"
#include "wt_board.h"
#include "wt_animations.h"

#define INVALID_GAME_MODE static_cast<WtGameModeIf*>(0)

/**************************
 *
 *************************/
struct WtGameModeState
{
    WtGameModeState( bool go, const WtGridAnimation& a ) :
        game_over( go ),
        animation( a ) {}

    bool            game_over;
    WtGridAnimation animation;
};

/**************************
 *
 *************************/
class WtGameModeIf
{
public:
    WtGameModeIf( std::string id_string ) :
        m_current_diff( wt_difficulty_EASY ),
        m_id_string( id_string )
        {}

    /**************************
     * 
     *************************/
    static std::vector<std::pair<wt_difficulty, const char*> > get_available_difficulties()
    {
        std::vector<std::pair<wt_difficulty, const char*> > result =
                         { std::make_pair( wt_difficulty_EASY,   WtL10n_tr("easy") ), 
                           std::make_pair( wt_difficulty_MEDIUM, WtL10n_tr("normal") ),
                           std::make_pair( wt_difficulty_HARD,   WtL10n_tr("hard") ) };
        return result;
    }

    /**************************
     *
     *************************/
    const std::string get_id_string()
    {
        //shall not be translated
        return m_id_string;
    }

    /**************************
     * 
     *************************/
    virtual void init_game( WtBoard&, WtPlayer& ) {}

    /**************************
     *
     *************************/
    virtual std::string get_title()=0;

    /**************************
     *
     *************************/
    virtual WtGameModeState eval_board( WtBoard& board, WtPlayer& player )=0;

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
        return WtL10n_tr("no help provided");
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

    /**************************
     *
     *************************/
    virtual bool stone_blocked( WtBoard& board,
                                uint8_t  row,
                                uint8_t  col )
    {
        return board.cell_occupied( row, col );
    }
    
    /**************************
     *
     *************************/
    virtual void insert_stone_at( WtBoard& board,
                                  uint8_t  row,
                                  uint8_t  col,
                                  char     value )
    {
        board.set_cell( row, col, value );
    }


private:
    wt_difficulty     m_current_diff;
    const std::string m_id_string;
};

/*****************************************
 * demo implementation of game mode if
 *****************************************/
class WtGameModeDemo : public WtGameModeIf
{
public:
    WtGameModeDemo() :
        WtGameModeIf( "Demo" ),
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
    virtual WtGameModeState eval_board( WtBoard&, WtPlayer& )
    {
        WtGameModeState gs( false,
                             WtGridAnimation::no_animation() );
        return gs;
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
