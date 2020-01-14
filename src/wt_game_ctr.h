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
#include "wt_storage.h"
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
#include "widgets/wt_numpad.h"


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
        WtViewIf( "#202020", 0, 0, WtTime::TimeType(0/*12500*/), WT_BIND_EVENT_HANDLER_1( WtGameCtr::on_key_press ) ),
        m_player(),
        m_active(),
        m_board(),
        m_active_mode( GAME_MODE_CTR.default_mode() ),
        m_pause_menu( WT_BIND_EVENT_HANDLER( WtGameCtr::restart ), 
                      WT_BIND_EVENT_HANDLER( WtGameCtr::return_to_menu ) ),
        m_score_summary(),
        //m_game_mode_intro(),
        m_grid_touch_control( ( STORAGE.get_settings().gridsize == 9 ? WtCoord( 18, 100 ) : WtCoord( 208, 177 ) )/*WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - static_cast<ssize_t>((76u * (STORAGE.get_settings().gridsize) ) / 2), 100 )*/,
                              ( STORAGE.get_settings().gridsize == 9 ? WtDim( 686, 686 ) : WtDim( 307, 307 ) )/*WtDim( static_cast<ssize_t>(STORAGE.get_settings().gridsize) * 76, static_cast<ssize_t>(STORAGE.get_settings().gridsize) * 76 )*/,
                              WtDim( 77, 77 ),
                              WT_BIND_EVENT_HANDLER_1( WtGameCtr::notify_click ) ),
        m_numpad( WtCoord( 0, ACTIVE_WINDOW_HEIGHT-(4*100+50)/*(STORAGE.get_settings().gridsize * 76) + 130*/  ), 
                  WtDim(ACTIVE_WINDOW_WIDTH, (4*100+50) /*ACTIVE_WINDOW_HEIGHT-(STORAGE.get_settings().gridsize * 76)-100*/  ), 
                  WT_BIND_EVENT_HANDLER_1( WtGameCtr::notify_num_click ) ),
        m_pause_btn( WtCoord( (ACTIVE_WINDOW_WIDTH-65)-24, (80-65)/2 ),
                     WtDim( 65, 65 ),
                     "settings_logo.bmp",
                     WT_BIND_EVENT_HANDLER( WtGameCtr::enter_pause ) ),
        m_settings_bg( WtCoord( 0, 0 ),
                       WtDim( ACTIVE_WINDOW_WIDTH, 80 ),
                       "#0e0e0e",
                       [](){} ),
        m_grid_bg( ( STORAGE.get_settings().gridsize == 9 ? WtCoord( 17, 99 ) : WtCoord( 208, 176 ) ),
                   ( STORAGE.get_settings().gridsize == 9 ? WtDim( 686, 686 ) : WtDim( 307, 307 ) ),
                   ( STORAGE.get_settings().gridsize == 9 ? "sudoku9x9_grid.bmp" : "sudoku4x4_grid.bmp" ),
                   [](){} ),
       // m_pause_end_animation(""),
        m_current_time( WtCoord( 50, 30 ),
                        WtDim( 10, 10 ),
                        "#0e0e0e",
                        [](){},
                        WtTime::format_time( WtTime::from_seconds(0)) ),

        m_current_update_counter(4),
        m_game_state( GAME_STOPPED ),
        m_hide_hint( false )
    {

        add_button( m_settings_bg );
        add_button( m_grid_bg );
        add_button( m_pause_btn );
        add_button( m_current_time );
        add_numpad( m_numpad );

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

        if ( m_game_state == GAME_ANIMATION_RUNNING )
        {
            ACTIVE_WINDOW.draw_button( m_settings_bg );
            ACTIVE_WINDOW.draw_button( m_pause_btn );
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

        WtGameModeState eval_result( false,
                                     nullptr );
        m_active_mode->eval_board( m_board, m_player, eval_result );
        game_over = eval_result.game_over;
        m_current_time.set_label( WtTime::format_time( m_player.get_current_time() ) );
        

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
    void notify_click( WtCoord pos )
    {
        m_active.set_pos( pos.y, pos.x );
    }

    /**************************
     *
     *************************/
    void notify_num_click( size_t number )
    {
        if ( number <= 9 )
        {
            if ( ! m_active_mode->stone_blocked( m_board, m_active.current_row(), m_active.current_column() ) )
            {
                std::cout << "num pad clicked at " << number << std::endl;
                if ( number == 0 )
                {
                    if ( m_numpad.is_edit_active() )
                    {
                        m_board.clear_notes( m_active.current_row(),
                                             m_active.current_column() );
                    }
                    else
                    {
                        m_board.set_cell( m_active.current_row(),
                                          m_active.current_column(),
                                          WtBoard::empty_cell );
                    }
                }
                else
                {
                    if ( m_numpad.is_edit_active() )
                    {
                        m_board.set_note( m_active.current_row(),
                                          m_active.current_column(),
                                          number );
                    }
                    else
                    {
                        m_board.set_cell( m_active.current_row(),
                                          m_active.current_column(),
                                          number + 0x30 );
                    }
                }
            }
        }
        else
        {
            switch( number )
            {
                case 10:
                    std::cout << "edit pressed\n";
                    break;
                case 11:
                    std::cout << "more pressed\n";
                    break;
                case 12:
                    std::cout << "undo pressed\n";
                    m_board.undo();
                    break;
                case 13:
                    std::cout << "save pressed\n";
                    m_board.save();
                    m_numpad.restore_possible( true );
                    break;
                case 14:
                    std::cout << "restore pressed\n";
                    m_board.restore();
                    break;
                default: break;
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
            m_active.move_right( m_board.col_count() ); 
        }
    }

    /**************************
     *
     *************************/
    uint8_t get_current_update_counter( WtPlayer& /*player*/ )
    {
        return 4;//( player.get_current_level() < 12 ?  48 - (player.get_current_level()*4) : 4 );
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
            m_active_mode->pause_time();
            enter_child_menu( m_pause_menu );
        }
    }

    /**************************
     *
     *************************/
    void leave_game()
    {
        m_hide_hint = false;
        m_active_mode->clear_occupied_cell_count();
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
                //play_animation( &m_pause_end_animation );
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
        if ( m_active_mode->get_id_string() == "Sudoku4" )
        {
            m_grid_touch_control.set_position( WtCoord( 208, 176 ) );
            m_grid_touch_control.set_size( WtDim( 307, 307 ) );
            m_grid_bg.set_image( "sudoku4x4_grid.bmp" );
            m_grid_bg.set_size( WtDim( 307, 307 ) );
            m_grid_bg.set_position( WtCoord( 208, 176 ) );
            m_numpad.setup_4x4_layout();

        }
        else
        {
            m_grid_touch_control.set_position( WtCoord( 17, 99 ) );
            m_grid_touch_control.set_size( WtDim( 686, 686 ) );
            m_grid_bg.set_image( "sudoku9x9_grid.bmp" );
            m_grid_bg.set_size( WtDim( 686, 686 ) );
            m_grid_bg.set_position( WtCoord( 17, 99 ) );
            m_numpad.setup_9x9_layout();
        }
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
    WtNumPad            m_numpad;
    WtButton            m_pause_btn;
    WtButton            m_settings_bg;
    WtButton            m_grid_bg;
    WtButton            m_current_time;

//    WtGridAnimation     m_pause_end_animation; 

    uint8_t             m_current_update_counter;
    wt_game_state       m_game_state;
    bool                m_hide_hint;
};


#endif /* _WT_GAME_H_ */
