/*!*****************************************************************************
 * @file wt_game_ctr.h
 * @brief main game logic control class
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
#ifndef _WT_GAME_CTR_H_
#define _WT_GAME_CTR_H_

#include <vector>
#include <unistd.h>

#include "global.h"

#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"
#include "wt_drawing.h"
#include "wt_input.h"
#include "wt_game_mode_if.h"
#include "wt_menu_if.h"
#include "wt_menu_settings.h"
#include "wt_storage.h"

#define GAME_CTR  WtGameCtr::instance()
class WtGameCtr : WtInputObserver, public WtSettingsChangeObserver
{
private:
    static constexpr const char* background_image = "bg.bmp";
// singleton definition
public:
    static WtGameCtr& instance()
    {
        static WtGameCtr _instance;
        return _instance;
    }
    ~WtGameCtr()
    {
    }
private:
    WtGameCtr() :
        // we store references of SINGLETON objects
        // as we make sure that migration from SINGLETON pattern
        // is as easy as possible; but for now we stay
        // with SINGLETON as we make sure that no objects
        // are mistakenly created/deleted
        m_player( PLAYER ),
        m_active( ACTIVE ),
        m_board( BOARD ),
        m_active_mode( GAME_MODE_CTR.default_mode() ),
        m_game_over( false ),
        m_shall_quit( false ),
        m_pause( false ),
        m_pause_menu( NULL ),
        m_left_btn( 0xF1, 
                                  WtCoord(0,100),
                                  WtDim( ACTIVE_WINDOW_WIDTH / 2, ACTIVE_WINDOW_HEIGHT/2+ACTIVE_WINDOW_HEIGHT/4 ), ""), 
        m_right_btn( 0xF2, 
                                 WtCoord(ACTIVE_WINDOW_WIDTH / 2, 100),
                                 WtDim( ACTIVE_WINDOW_WIDTH / 2, ACTIVE_WINDOW_HEIGHT/2+ACTIVE_WINDOW_HEIGHT/4 ), ""),
        m_drop_btn( 0xF3, 
                                 WtCoord( 0, ACTIVE_WINDOW_HEIGHT/2+ACTIVE_WINDOW_HEIGHT/4 ),
                                 WtDim( ACTIVE_WINDOW_WIDTH, ACTIVE_WINDOW_HEIGHT-(ACTIVE_WINDOW_HEIGHT/2+ACTIVE_WINDOW_HEIGHT/4)  ), ""),
        m_pause_btn( 0xF4, 
                                 WtCoord( 0, 0 ),
                                 WtDim( ACTIVE_WINDOW_WIDTH, 100 ), "")
    {
        if ( STORAGE.load() )
        {
            WtSettings settings = STORAGE.get_settings();
            set_mode( GAME_MODE_CTR.mode_from_string( settings.game_mode ) );
            WtL10n::set_language( settings.language );
            m_active_mode->set_difficulty( settings.difficulty );
        }

        ACTIVE_WINDOW.init();
    }
    WtGameCtr( const WtGameCtr& ); 
    WtGameCtr & operator = (const WtGameCtr &);

// api defintion
private:


    /**************************
     *
     *************************/
     void update_window()
     {
        /*
         * redraw board 
         */

        ACTIVE_WINDOW.clr();
        ACTIVE_WINDOW.draw_board( m_board );
        ACTIVE_WINDOW.draw_player_stat( m_player );
        ACTIVE_WINDOW.draw_active_letter( m_active );
        ACTIVE_WINDOW.draw_hint( m_active_mode->get_hint() );
        ACTIVE_WINDOW.draw_pause_button( WtCoord( ACTIVE_WINDOW_WIDTH - 42 - 50, 17 ) );
        ACTIVE_WINDOW.update();
    }

    /**************************
     *
     *************************/
    bool update_game()
    {
        bool game_over = false;
        if ( m_board.cell_occupied( m_active.current_row() - 1,
                                    m_active.current_column() ) )
        {
            /* commit ACTIVE to board */
            m_board.set_cell( m_active.current_row(), 
                              m_active.current_column(), 
                              m_active.current_value() );

            if ( m_active_mode->eval_board( m_board,
                                            m_player ) )
            {
                /* generate next stone */
                m_active.get_next( m_active_mode->next_letter() );

                game_over = m_board.cell_occupied( m_active.current_row(),
                                                   m_active.current_column() );
            }
            else
            {
                game_over = true;
            }
        }
        else
        {
            m_active.single_drop();
        }
        return game_over;
    }

    /**************************
     *
     *************************/
    virtual void notify_drop()
    {
        uint8_t new_row = m_active.current_row() - 1;
        while ( ! m_board.cell_occupied( new_row, m_active.current_column() ) )
        {
            new_row --;
        }
        new_row++;

        m_player.letter_dropped( m_active.current_row() - new_row );
        m_active.drop_at( new_row );
    }


    /**************************
     *
     *************************/
    virtual void notify_left()
    {
        if ( ! m_board.cell_occupied( m_active.current_row(),
                                      m_active.current_column() - 1 ) )
        {
            m_active.move_left();
        }
    }

    /**************************
     *
     *************************/
    virtual void notify_right()
    {
        if ( ! m_board.cell_occupied( m_active.current_row(),
                                      m_active.current_column() + 1 ) )
        {
            m_active.move_right(); 
        }
    }

    /**************************
     *
     *************************/
    virtual void notify_pause()
    {
        m_pause = !m_pause;
    }

    /**************************
     *
     *************************/
    virtual void notify_button_pressed( uint16_t id )
    {
        switch( id )
        {
            case 0xF1: notify_left(); break;
            case 0xF2: notify_right(); break;
            case 0xF3: notify_drop(); break;
            case 0xF4: notify_pause(); break;
            default: break;
        }
    }

    /**************************
     *
     *************************/
    void set_buttons()
    {
        ACTIVE_INPUT.add_button( m_left_btn );
        ACTIVE_INPUT.add_button( m_right_btn );
        ACTIVE_INPUT.add_button( m_drop_btn );
        ACTIVE_INPUT.add_button( m_pause_btn );

        ACTIVE_INPUT.listen( this );
    }
    /**************************
     *
     *************************/
    void unset_buttons()
    {
        ACTIVE_INPUT.ignore( this );

        ACTIVE_INPUT.remove_button( m_left_btn );
        ACTIVE_INPUT.remove_button( m_right_btn );
        ACTIVE_INPUT.remove_button( m_drop_btn );
        ACTIVE_INPUT.remove_button( m_pause_btn );
    }

    /**************************
     *
     *************************/
    void notify_language_changed( std::string lang_code )
    {
        std::cout << "new lanuage selected = "<< lang_code << std::endl;

        WtSettings settings = STORAGE.get_settings();
        settings.language = lang_code;
        STORAGE.store_settings( settings );
    }

    /**************************
     *
     *************************/
    void notify_game_mode_changed( WtGameModeIf* mode )
    {
        if ( NULL != mode )
        {
            set_mode( mode );

            WtSettings settings = STORAGE.get_settings();
            settings.game_mode = mode->get_id_string();
            STORAGE.store_settings( settings );
        }
    }

    /**************************
     *
     *************************/
    void notify_difficulty_changed( wt_difficulty diffi )
    {
        if ( INVALID_GAME_MODE != m_active_mode )
        {
            std::cout << "new diff selected = "<< diffi << std::endl;
            m_active_mode->set_difficulty( diffi );

            WtSettings settings = STORAGE.get_settings();
            settings.difficulty = diffi;
            STORAGE.store_settings( settings );
        }
    }

public:
    /**************************
     *
     *************************/
    void set_mode( WtGameModeIf* mode )
    {
        if ( ( INVALID_GAME_MODE != mode ) && ( INVALID_GAME_MODE != m_active_mode ) )
        {
            mode->set_difficulty( m_active_mode->get_difficulty() );
        }
        m_active_mode = mode;
    }

    /**************************
     *
     *************************/
    void set_pause_menu( WtMenuIf* menu )
    {
        m_pause_menu = menu;
    }

    /**************************
     *
     *************************/
    void quit()
    {
        m_shall_quit = true;
    }

    /**************************
     *
     *************************/
    void run()
    {
        uint8_t countdown = 48 - (m_player.get_current_level()*4);
        m_game_over = false;
        m_shall_quit = false;

        ACTIVE_WINDOW.set_bg( WtGameCtr::background_image );
        set_buttons();
        m_player.reset();
        m_board.init();

        m_active_mode->init_game( m_board, m_player );
        if ( INVALID_GAME_MODE == m_active_mode )
            m_game_over = true;
        else
            m_active.init( m_active_mode->next_letter() );

        while ( !m_game_over && !m_shall_quit )
        {
            ACTIVE_INPUT.read();

            if ( !m_pause )
            {
                if ( countdown == 0 )
                {
                    m_game_over = update_game();

                    countdown = 48 - (m_player.get_current_level()*4);
                }         

                update_window();
                countdown--;
            }
            else
            {
                if ( NULL != m_pause_menu )
                {
                    m_pause = false;
                    unset_buttons();
                    m_pause_menu->show();
                    ACTIVE_WINDOW.set_bg( WtGameCtr::background_image );
                    set_buttons();
                }
            }

            usleep(12500);
        }
        if ( m_game_over )
        {
            ACTIVE_WINDOW.clr();
            ACTIVE_WINDOW.draw_board( m_board );
            ACTIVE_WINDOW.draw_player_stat( m_player );
            ACTIVE_WINDOW.draw_active_letter( m_active );
            ACTIVE_WINDOW.draw_hint( m_active_mode->get_hint() );
            ACTIVE_WINDOW.draw_pause_button( WtCoord( ACTIVE_WINDOW_WIDTH - 42 - 50, 17 ) );


            ACTIVE_WINDOW.draw_message(WtL10n::tr("you lost! :P"));
            ACTIVE_WINDOW.update();
            sleep(5);
        }
        unset_buttons();
    }

private:
    WtPlayer&                   m_player;
    WtLetter&                   m_active;
    WtBoard&                    m_board;
    WtGameModeIf*               m_active_mode;
    bool                        m_game_over;
    bool                        m_shall_quit;
    bool                        m_pause;
    WtMenuIf*                   m_pause_menu;


    WtButton                    m_left_btn;
    WtButton                    m_right_btn;
    WtButton                    m_drop_btn;
    WtButton                    m_pause_btn;
};


#endif /* _WT_GAME_H_ */
