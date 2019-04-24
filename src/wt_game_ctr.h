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

#include "wt_input.h"
#include "wt_utils.h"
#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"
#include "wt_drawing.h"
#include "wt_game_mode_if.h"
#include "wt_game_mode_ctr.h"
#include "wt_menu_if.h"
#include "wt_storage.h"
#include "wt_animations.h"

#include "widgets/wt_grid_touch_overlay.h"

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
                              WT_BIND_EVENT_HANDLER( WtGameCtr::notify_left ),
                              WT_BIND_EVENT_HANDLER( WtGameCtr::notify_right ),
                              WT_BIND_EVENT_HANDLER( WtGameCtr::notify_drop ) ),
        m_pause_btn( WtCoord( 393, 32 ),
                     WtDim( 65, 65 ),
                     "pause_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtGameCtr::notify_pause ) ),
        m_pause_end_animation()
    {
        WtSettings settings = STORAGE.get_settings();
        set_mode( GAME_MODE_CTR.mode_from_string( settings.game_mode ) );
        WtL10n::set_language( settings.language );
        if ( INVALID_GAME_MODE != m_active_mode )
           m_active_mode->set_difficulty( settings.difficulty );

        ACTIVE_WINDOW.set_theme( settings.active_theme );

        ACTIVE_INPUT.register_key_press_delegate( WT_BIND_EVENT_HANDLER_1( WtGameCtr::on_key_press ) );

        construct_pause_animation( m_pause_end_animation );
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
        ACTIVE_WINDOW.draw_board( m_board, 
                                  m_active,
                                  STORAGE.get_settings().show_support_grid );
        ACTIVE_WINDOW.draw_player_stat( m_player );
        ACTIVE_WINDOW.draw_button( m_pause_btn );
        ACTIVE_WINDOW.draw_hint( m_active_mode->get_hint(), 
                                 m_active_mode->letter_after_next(), 
                                 STORAGE.get_settings().show_next_stone );
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

            for ( uint8_t r_idx = 0; r_idx < animation[a_idx].content.row_count; r_idx++ )
            {
                for ( uint8_t c_idx = 0; c_idx < animation[a_idx].content.col_count; c_idx++ )
                {
                    size_t cell_idx = static_cast<size_t>( (r_idx * animation[a_idx].content.col_count) + c_idx );
                    ACTIVE_WINDOW.draw_at_grid( animation[a_idx].content.row + r_idx,
                                                animation[a_idx].content.col + c_idx,
                                                animation[a_idx].content.cell_content[cell_idx],
                                                animation[a_idx].content.font );
                }
            }


            ACTIVE_WINDOW.update();

            WtTime::sleep( animation[a_idx].step_duration );
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
            std::cout << "new diff selected = "<< static_cast<size_t>(diffi) << std::endl;
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
    virtual void notify_supporting_grid_changed( bool show_grid )
    {
        std::cout << "grid " << ( show_grid ? "active" : "inactive" ) << std::endl;
        WtSettings settings = STORAGE.get_settings();
        settings.show_support_grid = show_grid;
        STORAGE.store_settings( settings );
    }


    /**************************
     *
     *************************/
    virtual void notify_show_next_stone_changed( bool show_stone )
    {
        std::cout << "preview " << ( show_stone ? "active" : "inactive" ) << std::endl;
        WtSettings settings = STORAGE.get_settings();
        settings.show_next_stone = show_stone;
        STORAGE.store_settings( settings );
    }


    /**************************
     *
     *************************/
    bool insert_entry( WtHighscores& scores, WtScoreEntry& entry )
    {
        bool entry_added = false;
        size_t game_mode_entries=0;
        for( size_t idx = 0; idx < scores.size(); idx++ )
        {
            if ( scores[idx].game_mode == entry.game_mode )
            {
                if ( scores[idx].score < entry.score )
                {
                    WtHighscores::iterator it = scores.begin();
                    std::advance( it, idx );
                    scores.insert( it, entry );
                    entry_added = true;
                    break;
                }
                game_mode_entries++;
            }
        }

        if ( ( !entry_added ) && ( game_mode_entries < 10 ) )
        {
            scores.push_back( entry );
            entry_added = true;
        }
        return entry_added;
    }


    /**************************
     *
     *************************/
    bool update_highscores( WtPlayer& player, WtGameModeIf* mode, WtHighscores& scores )
    {
        // eval if player stat is within first 3 of game mode
        // if true add player at correct position
        std::cout << "highscore entry: Lvl " << player.get_current_level() << " at " << player.get_points() << " within mode \"" << mode->get_title() << "\"" << std::endl;

        WtScoreEntry new_entry( mode->get_title(),
                                player.get_points(),
                                player.get_current_level() );

        return insert_entry( scores, new_entry );
    }

    /**************************
     *
     *************************/
    void construct_running_letter_animation( WtGridAnimation& animation,
                                             const uint8_t row_count, const uint8_t col_count,
                                             std::vector<char> letter_data,
                                             const uint8_t drop_until_row,
                                             const uint8_t col_offset,
                                             WtTime::TimeType speed_per_row )
    {
        const std::string font = "grid_inverse";
        for ( size_t draw_idx = 1;
              draw_idx < static_cast<size_t>(WtBoard::row_count-drop_until_row);
              draw_idx++ )
        {
            if ( draw_idx <= row_count )
            {
                size_t selected_row_start = (row_count - draw_idx) * col_count;
                std::vector<char>::iterator it = letter_data.begin();
                std::advance( it, selected_row_start );
                std::vector<char> row_cells(it, letter_data.end());

                WtGridAnimation::GridContent step_content( 1, col_offset, draw_idx, col_count, font, row_cells );
                WtGridAnimation::GridAnimationStep step_next( step_content, speed_per_row);
                animation.push_back( step_next );
            }
            else
            {
                WtGridAnimation::GridContent letter( draw_idx-row_count, col_offset, row_count, col_count,
                                                     font, letter_data );
                WtGridAnimation::GridAnimationStep step_next( letter, speed_per_row);
                animation.push_back( step_next );
            }
        }
    }

    /**************************
     *
     *************************/
    void construct_pause_animation( WtGridAnimation& pause_animation )
    {
        const char x = WtBoard::empty_cell;
        const uint8_t final_row = 5;
        const uint8_t row_count = 12;
        const std::string font = "grid_inverse";

        std::vector<char> cells_one{'1','1','1','1','1','1',
                                    '1','1','1','1','1','1',
                                     x , x ,'1','1', x , x,
                                     x , x ,'1','1', x , x,
                                     x , x ,'1','1', x , x,
                                     x , x ,'1','1', x , x,
                                     x , x ,'1','1', x , x,
                                     x , x ,'1','1', x , x,
                                     x , x ,'1','1', x , x,
                                     x , x ,'1','1', x , x,
                                    '1','1','1','1','1','1',
                                    '1','1','1','1','1','1'};
        std::vector<char> cells_two{'2','2','2','2','2','2','2','2',
                                    '2','2','2','2','2','2','2','2',
                                     x ,'2','2', x , x ,'2','2', x,
                                     x ,'2','2', x , x ,'2','2', x,
                                     x ,'2','2', x , x ,'2','2', x,
                                     x ,'2','2', x , x ,'2','2', x,
                                     x ,'2','2', x , x ,'2','2', x,
                                     x ,'2','2', x , x ,'2','2', x,
                                     x ,'2','2', x , x ,'2','2', x,
                                     x ,'2','2', x , x ,'2','2', x,
                                    '2','2','2','2','2','2','2','2',
                                    '2','2','2','2','2','2','2','2'};
        std::vector<char> cells_three{'3','3','3','3','3','3','3','3','3','3',
                                      '3','3','3','3','3','3','3','3','3','3',
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                       x ,'3','3', x ,'3','3', x ,'3','3', x,
                                      '3','3','3','3','3','3','3','3','3','3',
                                      '3','3','3','3','3','3','3','3','3','3' };

        const uint8_t col_count_3 = WtBoard::col_count;
        const uint8_t col_count_2 = WtBoard::col_count-2;
        const uint8_t col_count_1 = WtBoard::col_count-4;

        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_3,
                                            cells_three,
                                            final_row, 0,
                                            WtTime::TimeType(30000) );

        WtGridAnimation::GridContent three( final_row, 0, row_count, WtBoard::col_count,
                                            font, cells_three );
        WtGridAnimation::GridAnimationStep step_three( three, WtTime::TimeType(300000));
        pause_animation.push_back( step_three );



        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_2,
                                            cells_two,
                                            final_row, 1,
                                            WtTime::TimeType(30000) );

        WtGridAnimation::GridContent two( final_row, 1, row_count, WtBoard::col_count-2,
                                          font, cells_two );
        WtGridAnimation::GridAnimationStep step_two( two, WtTime::TimeType(300000));
        pause_animation.push_back( step_two );



        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_1,
                                            cells_one,
                                            final_row, 2,
                                            WtTime::TimeType(30000) );

        WtGridAnimation::GridContent one( final_row, 2, row_count, WtBoard::col_count-4,
                                          font, cells_one );
        WtGridAnimation::GridAnimationStep step_one( one, WtTime::TimeType(300000));

        pause_animation.push_back( step_one );
    }

    /**************************
     *
     *************************/
    uint8_t get_current_update_counter( WtPlayer& player )
    {
        return ( player.get_current_level() < 12 ?  48 - (player.get_current_level()*4) : 4 );
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
            m_pause = false;

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

            uint8_t countdown = get_current_update_counter( m_player );
            while ( !m_game_over && !m_shall_quit )
            {
                WtTime::TimePoint before = WtTime::get_time();
                ACTIVE_INPUT.read();

                if ( !m_pause )
                {
                    if ( countdown == 0 )
                    {
                        m_game_over = update_game();

                        countdown = get_current_update_counter( m_player );
                    }

                    update_window();

                    if ( countdown > 0 )
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
                        if ( ( ! m_shall_quit ) && ( ! m_shall_restart ) )
                            play_animation( m_pause_end_animation );
                        set_buttons();
                    }
                }
                WtTime::TimePoint after = WtTime::get_time();

                WtTime::TimeType remaining = WtTime::TimeType(12500) - WtTime::get_time_elapsed( before, after );
                if ( remaining > WtTime::TimeType(0) )
                {
                    WtTime::sleep( remaining );
                }
            }
        }
        while ( m_shall_restart );

        if (( m_game_over || m_shall_quit ) && m_player.get_points() > 0 )
        {
            bool new_highscore = update_highscores( m_player, m_active_mode, STORAGE.get_scores() );
            STORAGE.store_highscores( STORAGE.get_scores() );

            ACTIVE_WINDOW.set_bg( "bg_menu_pause.bmp" );
            ACTIVE_WINDOW.clr();
            ACTIVE_WINDOW.draw_player_stat( m_player );

            if ( new_highscore )
            {
                ACTIVE_WINDOW.draw_message(WtL10n_tr("wow! new highscore"));
            }
            else
            {
                ACTIVE_WINDOW.draw_message(WtL10n_tr("you lost! :P"));
            }
            ACTIVE_WINDOW.update();
            WtTime::sleep(WtTime::from_seconds(5));
        }
        unset_buttons();
    }

private:
    WtPlayer            m_player;
    WtLetter            m_active;
    WtBoard             m_board;
    WtGameModeIf*       m_active_mode;
    bool                m_game_over;
    bool                m_shall_quit;
    bool                m_shall_restart;
    bool                m_pause;
    WtMenuIf*           m_pause_menu;


    WtGridTouchOverlay  m_grid_touch_control;
    WtButton            m_pause_btn;

    WtGridAnimation     m_pause_end_animation; 
};


#endif /* _WT_GAME_H_ */
