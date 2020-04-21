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
        WtGameModeState( bool go, WtGridAnimation* a ) :
            game_over( go ),
            animations()
        {
            animations.push_back( a );
        }
        /**************************
         * 
         *************************/
        void add_animation( WtGridAnimation* a )
        {
            animations.push_back( a );
        }

        bool                game_over;
        WtAnimationPlaylist animations;
    };

    /**************************
     *
     *************************/
    class WtGameModeIf
    {
    public:
        WtGameModeIf( std::string id_string ) :
            m_current_diff( wt_difficulty_EASY ),
            m_id_string( id_string ),
            m_cell_occupied_count(0)
            {}
        virtual ~WtGameModeIf() {}

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
        virtual void init_game( WtBoard&, WtPlayer&, std::string, std::string ) {}

        /**************************
         *
         *************************/
        virtual std::string get_title()=0;

        /**************************
         *
         *************************/
        virtual std::string get_name()=0;

        /**************************
         *
         *************************/
        virtual void eval_board( WtBoard&, WtPlayer&, WtGameModeState& )=0;
        /**************************
         *
         *************************/
        virtual void pre_eval_board( WtBoard& ) {}
        /**************************
         *
         *************************/
        virtual bool is_input_possible( WtBoard&, const char  ) { return true; }
        /**************************
         *
         *************************/
        virtual std::string get_current_game_state( WtBoard& ) { return "";}

        /**************************
         *
         *************************/
        virtual std::string get_current_game_orig() { return ""; }

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
        virtual void pause_time() {}

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
            std::cout << "new diff selected = "<< static_cast<size_t>(new_difficulty) << std::endl;
            m_current_diff = new_difficulty;
        }

        /**************************
         *
         *************************/
        virtual wt_difficulty get_difficulty() const
        {
            return m_current_diff;
        }

        /**************************
         *
         *************************/
        virtual std::string letter_after_next()
        {
            return std::string( 1, WtBoard::empty_cell );
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
            if ( ( value != WtBoard::empty_cell ) && ( value != ' ' ) )
            {
                stone_added();
            }
            else
            {
                stone_removed();
            }
            board.set_cell( row, col, value );
        }


        /**************************
         *
         *************************/
        uint16_t get_occupied_cell_count() const
        {
            return m_cell_occupied_count;    
        }

        /**************************
         *
         *************************/
        void clear_occupied_cell_count()
        {
            m_cell_occupied_count = 0;
        }

    protected:
        /**************************
         *
         *************************/
        void stone_added()
        {
            m_cell_occupied_count++;
        }


        /**************************
         *
         *************************/
        void stone_removed()
        {
            if ( m_cell_occupied_count > 0 )
                m_cell_occupied_count--;
        }

    private:
        wt_difficulty     m_current_diff;
        const std::string m_id_string;
        uint16_t          m_cell_occupied_count;
    };

    /*****************************************
     * demo implementation of game mode if
     *****************************************/
    class WtGameModeDemo : public WtGameModeIf
    {
    public:
        WtGameModeDemo( std::string title ) :
            WtGameModeIf( "LOCKED" ),
            m_title(title)
        {
        }
        ~WtGameModeDemo()
        {
        }

        /**************************
         * 
         *************************/
        virtual void init_game( WtBoard&, WtPlayer&, std::string, std::string )
        {

        }

        /**************************
         *
         *************************/
        virtual std::string get_title()
        {
            return "@dark/"+ m_title+ "_"+ WtL10n::get_language_code() +"_locked.bmp";
        }

        /**************************
         *
         *************************/
        virtual std::string get_name()
        {
            return "";
        }

        /**************************
         *
         *************************/
        virtual void eval_board( WtBoard&, WtPlayer&, WtGameModeState& )
        {
        }

        /**************************
         *
         *************************/
        virtual bool stone_blocked( WtBoard&,
                                    uint8_t,
                                    uint8_t )
        {
            return true;
        }

        /**************************
         *
         *************************/
        virtual char next_letter()
    {
        return ' '; 
    }

    /**************************
     *
     *************************/
    virtual std::string get_hint()
    {
        std::string s_hint( "Upgrade to Pro\nand enjoy more exciting game modes!" );
        return s_hint;
    }
private:
    WtGameModeDemo( const WtGameModeDemo& ); 
    WtGameModeDemo & operator = (const WtGameModeDemo &);



private:
    std::string m_title;
};


#endif /* _WT_GAME_MODE_IF_H_ */
