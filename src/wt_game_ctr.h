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

#include "wt_view_if.h"
#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"
#include "wt_game_mode_if.h"
#include "wt_game_mode_ctr.h"
#include "wt_animation_builder.h"
#include "menus/wt_menu_pause.h"
#include "menus/wt_menu_score_summary.h"
//#include "menus/wt_menu_game_mode_intro.h"

#include "widgets/wt_grid_touch_overlay.h"


class WtGameCtr : public WtViewIf
{
private:
    typedef enum {
        GAME_STOPPED=0,
        GAME_STARTED,
        GAME_TO_START,
        GAME_ANIMATION_RUNNING,
        GAME_PAUSED,
        GAME_TO_QUIT
    } wt_game_state;
public:
    WtGameCtr() :
        WtViewIf( "#172d4a", false, WtTime::TimeType(12500), WT_BIND_EVENT_HANDLER_1( WtGameCtr::on_key_press ), "game_bg_music.ogg" ),
        m_player(),
        m_active(),
        m_board(),
        m_active_mode( GAME_MODE_CTR.default_mode() ),
        m_pause_menu( WT_BIND_EVENT_HANDLER( WtGameCtr::restart ), 
                      WT_BIND_EVENT_HANDLER( WtGameCtr::return_to_menu ) ),
        m_score_summary(),
        //m_game_mode_intro(),
        m_grid_touch_control( WtCoord( 0, 100 ),
                              WtDim( ACTIVE_WINDOW_WIDTH, ACTIVE_WINDOW_HEIGHT-100 ),
                              WT_BIND_EVENT_HANDLER( WtGameCtr::notify_left ),
                              WT_BIND_EVENT_HANDLER( WtGameCtr::notify_right ),
                              WT_BIND_EVENT_HANDLER( WtGameCtr::notify_drop ) ),
        m_pause_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-80)-24, 0 ),
                     WtDim( 80, 80 ),
                     "settings_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtGameCtr::enter_pause ) ),
        m_settings_bg( WtCoord( 0, 0 ),
                       WtDim( ACTIVE_WINDOW_WIDTH, 80 ),
                       "#112238",
                       [](){} ),
        m_pause_end_animation(""),
        m_current_update_counter(48),
        m_game_state( GAME_STOPPED ),
        m_hide_hint( false ),
        m_bomb_dropped(false)
    {

        WtGridAnimationBuilder::construct_pause_animation( m_pause_end_animation );

        add_button( m_settings_bg );
        add_button( m_pause_btn );
        

        if ( nullptr != m_active_mode )
            m_active_mode->set_difficulty( STORAGE.get_settings().difficulty );
    }

    ~WtGameCtr() {}


    /**************************
     *
     *************************/
    void set_game_mode( WtGameModeIf* mode )
    {
        if ( NULL != mode )
        {
            if ( NULL != m_active_mode )
            {
                mode->set_difficulty( m_active_mode->get_difficulty() );
            }
            else
            {
                mode->set_difficulty( STORAGE.get_settings().difficulty );
            }
            m_active_mode = mode;
//            m_game_mode_intro.set_game_mode( m_active_mode );
        }
    }


private:
    WtGameCtr( const WtGameCtr& ); 
    WtGameCtr & operator = (const WtGameCtr &);

    /**************************
     *
     *************************/
    void update_window()
    {
        ACTIVE_WINDOW.draw_board( m_board, 
                                  m_active,
                                  STORAGE.get_settings().show_support_grid );
        if ( STORAGE.get_settings().show_next_stone )
            ACTIVE_WINDOW.draw_hint( m_active_mode->letter_after_next() );

        ACTIVE_WINDOW.draw_player_stat( m_player );

        if ( m_active_mode->get_occupied_cell_count() < 5 )
        {
            if ( ! m_hide_hint )
            {
                WtCoord hint_pos = WtCoord( 130, 890 );
                if ( m_active_mode->get_id_string() != "WordtrisClassic" )
                {
                    hint_pos = WtCoord( 130, 400 );
                }
                ACTIVE_WINDOW.draw_help( hint_pos, m_active_mode->get_hint() );
            }
        }
        else
        {
            m_hide_hint = true;
        }

        if ( m_game_state == GAME_ANIMATION_RUNNING )
        {
            ACTIVE_WINDOW.draw_button( m_pause_btn );
            ACTIVE_WINDOW.draw_button( m_settings_bg );
        }
    }

    /**************************
     *
     *************************/
    void play_animation( WtViewIf* animation )
    {
        wt_game_state old_state = m_game_state;

        m_game_state = GAME_ANIMATION_RUNNING; 
        animation->set_overlay_drawing( WT_BIND_EVENT_HANDLER( WtGameCtr::update_window ) );
        enter_child_menu( *animation );
        m_game_state = old_state;
    }

    /**************************
     *
     *************************/
    bool update_game( bool& animation_played )
    {
        bool game_over = false;
        animation_played = false;
        if ( m_active_mode->stone_blocked( m_board,
                                           m_active.current_row() - 1,
                                           m_active.current_column() ) )
        {
            if (m_active.current_value() == '*')
            {
                if ( ! m_bomb_dropped )
                {
                    ACTIVE_SFX.play_sound("bomb");
                }
                m_bomb_dropped = false;
            }
            /* commit ACTIVE to board */
            m_active_mode->insert_stone_at( m_board, 
                                            m_active.current_row(), 
                                            m_active.current_column(), 
                                            m_active.current_value() );

            m_grid_touch_control.set_direction_seperator_pos( ACTIVE_WINDOW.grid_pos_to_coord( WtBoard::row_count - 1, 5 ).x );

            {
                WtGameModeState eval_result( false,
                                             nullptr );
                m_active_mode->eval_board( m_board, m_player, eval_result );

                if ( !eval_result.animations.empty() )
                {
                    for ( size_t a_idx = 0; a_idx < eval_result.animations.size(); a_idx++ )
                        play_animation( eval_result.animations[a_idx].animation );
                    animation_played = true;
                }
                game_over = eval_result.game_over;
            }

            if ( !game_over )
            {
                /* generate next stone */
                m_active.get_next( m_active_mode->next_letter() );


                game_over = m_active_mode->stone_blocked( m_board,
                                                          m_active.current_row(),
                                                          m_active.current_column() );
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
    void on_key_press( wt_control key )
    {
        switch( key )
        {
            default:
            case wt_control_QUIT:
            case wt_control_INVALID:
                // unhandled events are ignored
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
            case wt_control_PAUSE:
            case wt_control_BACK:
                enter_pause();
                break;
        }
    }

    /**************************
     *
     *************************/
    void notify_drop()
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
        if (m_active.current_value() == '*')
        {
            if ( ! m_bomb_dropped )
            {
                ACTIVE_SFX.play_sound("bomb");
                m_bomb_dropped = true;
            }
        }
    }

    /**************************
     *
     *************************/
    void notify_left()
    {
        if ( ! m_active_mode->stone_blocked( m_board,
                                             m_active.current_row(),
                                             m_active.current_column() - 1 ) )
        {
            m_active.move_left();
            m_grid_touch_control.set_direction_seperator_pos( ACTIVE_WINDOW.grid_pos_to_coord( m_active.current_row(), m_active.current_column() ).x );
        }
    }

    /**************************
     *
     *************************/
    void notify_right()
    {
        if ( ! m_active_mode->stone_blocked( m_board,
                                             m_active.current_row(),
                                             m_active.current_column() + 1 ) )
        {
            m_active.move_right(); 
            m_grid_touch_control.set_direction_seperator_pos( ACTIVE_WINDOW.grid_pos_to_coord( m_active.current_row(), m_active.current_column() ).x );
        }
    }

    /**************************
     *
     *************************/
    uint8_t get_current_update_counter( WtPlayer& player )
    {
        return ( player.get_current_level() < 12 ?  48 - (player.get_current_level()*4) : 4 );
    }


    /**************************
     *
     *************************/
    void return_to_menu()
    {
        m_game_state = GAME_TO_QUIT;
    }

    /**************************
     *
     *************************/
    void restart()
    {
        m_game_state = GAME_STOPPED;
    }

    /**************************
     *
     *************************/
    void enter_pause()
    {
        if ( m_game_state == GAME_STARTED )
        {
            m_game_state = GAME_PAUSED;
            enter_child_menu( m_pause_menu );
        }
    }

    /**************************
     *
     *************************/
    void leave_game()
    {
        m_hide_hint = false;
        m_game_state = GAME_STOPPED;
        if ( m_player.get_points() > 0 )
        {
            m_score_summary.update_highscores( m_player, m_active_mode, STORAGE.get_scores() );
            STORAGE.store_highscores( STORAGE.get_scores() );

            enter_child_menu( m_score_summary );
        }
        leave();
    }

    /**************************
     *
     *************************/
    void init_game()
    {
        m_player.reset();
        m_board.init();
        if ( INVALID_GAME_MODE == m_active_mode )
        {
            std::cout << "invalid game mode\n";
            m_game_state = GAME_TO_QUIT;
        }
        else
        {
            m_active_mode->init_game( m_board, m_player );
            m_active.init( m_active_mode->next_letter() );
            m_current_update_counter = get_current_update_counter( m_player );
            m_game_state = GAME_TO_START;
            m_grid_touch_control.set_direction_seperator_pos( ACTIVE_WINDOW.grid_pos_to_coord( m_active.current_row(), m_active.current_column() ).x );

        }
    }

    /**************************
     *
     *************************/
    virtual void update_view()
    {
        switch( m_game_state )
        {
            default: break;

            case GAME_STOPPED:
                init_game();
                break;

            case GAME_TO_QUIT:
                leave_game();
                break;

            case GAME_PAUSED:
                m_game_state = GAME_STARTED;
                play_animation( &m_pause_end_animation );
                break;

            case GAME_TO_START:
                //enter_child_menu( m_game_mode_intro );
                m_game_state = GAME_STARTED;
                break;


            case GAME_STARTED:
                {
                    bool was_animation_played = false;
                    if ( m_current_update_counter == 0 )
                    {
                        bool game_over = update_game( was_animation_played );

                        if ( game_over )
                        {
                            ACTIVE_SFX.play_gameover_sound();
                            leave_game();
                        }

                        m_current_update_counter = get_current_update_counter( m_player );
                    }

                    if ( ! was_animation_played )
                    {
                        update_window();
                    }

                    if ( m_current_update_counter > 0 )
                        m_current_update_counter--;
                }
                break;
        }
   }

    /**************************
     * signal
     *************************/
    virtual void entered_view()
    {
        ACTIVE_INPUT.add_active_region( m_grid_touch_control );
    }

    /**************************
     * signal
     *************************/
    virtual void left_view()
    {
        ACTIVE_INPUT.remove_active_region( m_grid_touch_control );
    }


private:
    WtPlayer            m_player;
    WtLetter            m_active;
    WtBoard             m_board;
    WtGameModeIf*       m_active_mode;

    WtMenuPause         m_pause_menu;
    WtMenuScoreSummary  m_score_summary;
    //WtMenuGameModeIntro m_game_mode_intro;

    WtGridTouchOverlay  m_grid_touch_control;
    WtButton            m_pause_btn;
    WtButton            m_settings_bg;

    WtGridAnimation     m_pause_end_animation; 

    uint8_t             m_current_update_counter;
    wt_game_state       m_game_state;
    bool                m_hide_hint;
    bool                m_bomb_dropped;
};


#endif /* _WT_GAME_H_ */
