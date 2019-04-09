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

#include <unistd.h>

#include "wt_input.h"

#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"
#include "wt_drawing.h"
#include "wt_game_mode_if.h"
#include "wt_game_mode_ctr.h"
#include "wt_menu_if.h"
#include "wt_storage.h"
#include "wt_animations.h"

#include "wt_grid_touch_overlay.h"

#define GAME_CTR  WtGameCtr::instance()
class WtGameCtr : public WtSettingsChangeObserver
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
        m_player(),
        m_active(),
        m_board(),
        m_active_mode( GAME_MODE_CTR.default_mode() ),
        m_game_over( false ),
        m_shall_quit( false ),
        m_shall_restart( false ),
        m_pause( false ),
        m_pause_menu( NULL ),
        m_grid_touch_control( WtCoord( 0, 100 ),
                              WtDim( ACTIVE_WINDOW_WIDTH, ACTIVE_WINDOW_HEIGHT-100 ),
                              std::bind ( &WtGameCtr::notify_left, this ),
                              std::bind ( &WtGameCtr::notify_right, this ),
                              std::bind ( &WtGameCtr::notify_drop, this ) ),
        m_pause_btn( WtCoord( 393, 32 ),
                     WtDim( 64, 64 ),
                     "pause_btn.bmp",
                     std::bind ( &WtGameCtr::notify_pause, this ) )
    {
        WtSettings settings = STORAGE.get_settings();
        set_mode( GAME_MODE_CTR.mode_from_string( settings.game_mode ) );
        WtL10n::set_language( settings.language );
        if ( INVALID_GAME_MODE != m_active_mode )
           m_active_mode->set_difficulty( settings.difficulty );

        ACTIVE_WINDOW.init();
        ACTIVE_WINDOW.set_theme( settings.active_theme );

        ACTIVE_INPUT.register_key_press_delegate( std::bind( &WtGameCtr::on_key_press, this, std::placeholders::_1 ) );
    }
    WtGameCtr( const WtGameCtr& ); 
    WtGameCtr & operator = (const WtGameCtr &);

// api defintion
private:
    /**************************
     *
     *************************/
    void draw()
    {
        ACTIVE_WINDOW.draw_board( m_board );
        ACTIVE_WINDOW.draw_player_stat( m_player );
        ACTIVE_WINDOW.draw_active_letter( m_active );
        ACTIVE_WINDOW.draw_hint( m_active_mode->get_hint() );
        ACTIVE_WINDOW.draw_button( m_pause_btn );
    }

    /**************************
     *
     *************************/
    void update_window()
    {
        /*
         * redraw board 
         */

        ACTIVE_WINDOW.clr();
        draw();
        ACTIVE_WINDOW.update();
    }

    /**************************
     *
     *************************/
    void play_animation( const WtGridAnimation& animation )
    {
        for ( size_t a_idx = 0; a_idx < animation.size(); a_idx++ )
        {
            ACTIVE_WINDOW.clr();
            draw();

            ACTIVE_WINDOW.draw_at_grid( animation[a_idx].content.row,
                                        animation[a_idx].content.col,
                                        animation[a_idx].content.horizontal,
                                        animation[a_idx].content.text,
                                        animation[a_idx].content.font );

            ACTIVE_WINDOW.update();

            usleep( animation[a_idx].step_duration );
        }
    }

    /**************************
     *
     *************************/
    bool update_game()
    {
        bool game_over = false;
        if ( m_active_mode->stone_blocked( m_board,
                                           m_active.current_row() - 1,
                                           m_active.current_column() ) )
        {
            /* commit ACTIVE to board */
            m_active_mode->insert_stone_at( m_board, 
                                            m_active.current_row(), 
                                            m_active.current_column(), 
                                            m_active.current_value() );

            WtGameModeState eval_result = m_active_mode->eval_board( m_board,
                                                                     m_player );

            if ( !eval_result.animation.empty() )
            {
                play_animation( eval_result.animation );
            }

            if ( !eval_result.game_over )
            {
                /* generate next stone */
                m_active.get_next( m_active_mode->next_letter() );

                game_over = m_active_mode->stone_blocked( m_board,
                                                          m_active.current_row(),
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
    virtual void on_key_press( wt_control key )
    {
        switch( key )
        {
            default:
            case wt_control_INVALID:
                break;


            case wt_control_DROP:
                notify_drop();
                break;
            case wt_control_LEFT:
                notify_left();
                break;
            case wt_control_RIGHT:
                notify_right();
                break;
            case wt_control_QUIT:
                exit(0);
                break;
            case wt_control_PAUSE:
                notify_pause();
                break;
        }
    }


    /**************************
     *
     *************************/
    virtual void notify_drop()
    {
        uint8_t new_row = m_active.current_row() - 1;
        while ( ! m_active_mode->stone_blocked( m_board,
                                                new_row,
                                                m_active.current_column() ) )
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
        if ( ! m_active_mode->stone_blocked( m_board,
                                             m_active.current_row(),
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
        if ( ! m_active_mode->stone_blocked( m_board,
                                             m_active.current_row(),
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
    void set_buttons()
    {
        ACTIVE_INPUT.add_active_region( m_grid_touch_control );
        ACTIVE_INPUT.add_active_region( m_pause_btn );
    }
    /**************************
     *
     *************************/
    void unset_buttons()
    {
        ACTIVE_INPUT.remove_active_region( m_grid_touch_control );
        ACTIVE_INPUT.remove_active_region( m_pause_btn );
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


    /**************************
     *
     *************************/
    virtual void notify_theme_changed( std::string name )
    {
        ACTIVE_WINDOW.set_theme( name );

        std::cout << "new theme selected = "<< name << std::endl;
        WtSettings settings = STORAGE.get_settings();
        settings.active_theme = name;
        STORAGE.store_settings( settings );
    }

    /**************************
     *
     *************************/
    WtHighscores& update_highscores( WtPlayer& player, WtGameModeIf* mode, WtHighscores& scores )
    {
        // eval if player stat is within first 3 of game mode
        // if true add player at correct position

        return scores;
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
    void restart()
    {
        m_shall_quit = true;
        m_shall_restart = true;
    }

    /**************************
     *
     *************************/
    void run()
    {
        ACTIVE_WINDOW.set_bg( WtGameCtr::background_image );
        set_buttons();

        do
        {
            m_shall_restart = false;
            m_game_over = false;
            m_shall_quit = false;

            m_player.reset();
            m_board.init();

            if ( INVALID_GAME_MODE == m_active_mode )
            {
                std::cout << "invalid game mode\n";
                m_game_over = true;
            }
            else
            {
                m_active_mode->init_game( m_board, m_player );
                m_active.init( m_active_mode->next_letter() );
            }

            uint8_t countdown = 48 - (m_player.get_current_level()*4);
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
        }
        while ( m_shall_restart );

        STORAGE.store_highscores( update_highscores( m_player, m_active_mode, STORAGE.get_scores() ) );

        if ( m_game_over )
        {
            ACTIVE_WINDOW.set_bg( "bg_menu_pause.bmp" );
            ACTIVE_WINDOW.clr();
            ACTIVE_WINDOW.draw_player_stat( m_player );
//            ACTIVE_WINDOW.draw_board( m_board );
//            ACTIVE_WINDOW.draw_active_letter( m_active );
//            ACTIVE_WINDOW.draw_hint( m_active_mode->get_hint() );
 //           ACTIVE_WINDOW.draw_button( m_pause_btn );

            ACTIVE_WINDOW.draw_message(WtL10n_tr("you lost! :P"));
            ACTIVE_WINDOW.update();
            sleep(5);
        }
        unset_buttons();
    }

private:
    WtPlayer                    m_player;
    WtLetter                    m_active;
    WtBoard                     m_board;
    WtGameModeIf*               m_active_mode;
    bool                        m_game_over;
    bool                        m_shall_quit;
    bool                        m_shall_restart;
    bool                        m_pause;
    WtMenuIf*                   m_pause_menu;


    WtGridTouchOverlay          m_grid_touch_control;
    WtButton                    m_pause_btn;
};


#endif /* _WT_GAME_H_ */
