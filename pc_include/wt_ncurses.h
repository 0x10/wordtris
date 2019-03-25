/*!*****************************************************************************
 * @file wt_ncurses.h
 * @brief ncurses based backend implementation for input/drawing
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
#ifndef _WT_NCURSES_H_
#define _WT_NCURSES_H_

#include <ncurses.h> //for getkey


#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"
#include "wt_backend_policy_if.h"

#define ARROW_D (0x42)
#define ARROW_R (0x43)
#define ARROW_L (0x44)
#define KEY_Q   (0x10) //tbd

#define NCURSES_WINDOW WINDOW( WtPolicyNcurses )
#define NCURSES_INPUT  INPUT( WtPolicyNcurses )

#define NCURSES_WIDTH   (540)
#define NCURSES_HEIGHT  (960)
WtCoord buttons[10];
uint8_t active_buttons=0;
class WtPolicyNcurses : public WtDrawingPolicyIf, public WtInputPolicyIf
{
// policy use only
protected:
    WtPolicyNcurses()
    {
        initscr();
        cbreak();
        noecho();
        curs_set(FALSE);
        nodelay(stdscr, TRUE);
        scrollok(stdscr, TRUE);
        active_buttons = 0;
    }

    ~WtPolicyNcurses()
    {
        endwin();
    }

    /**************************
     *
     *************************/
    void clr()
    {
        clear();
    }

    /**************************
     *
     *************************/
    void draw_player_stat( const WtPlayer& player )
    {
        mvprintw( 0,0, "player(%u): %lu @ %lu words\n", 
                                                  player.get_current_level(),
                                                  player.get_points(),
                                                  player.get_solved_word_count() );
    }

    /**************************
     *
     *************************/
    void draw_board( const WtBoard& board )
    {
        const char* SEP_LINE = "+--------------------+\n";

        mvprintw( 1, 0, SEP_LINE );
        for( int8_t i=WtBoard::row_count-1 ; i >= 0; i-- )
        {
            printw("|");
            for( uint8_t j=0; j < WtBoard::col_count; j++ )
            {
                char cell_val = board.get_cell( (uint8_t)i, j );
                if ( cell_val == '\0' )
                {
                    printw("  ");
                }
                else
                {
                    printw("%c ", cell_val );
                }
            }
            printw("|\n");
        }
        mvprintw( WtBoard::row_count+2, 0, SEP_LINE );
    }

    /**************************
     *
     *************************/
    void draw_active_letter( const WtLetter& active )
    {
        int x = 2+(WtBoard::row_count-1);
        int y = 1;
        mvprintw( x - active.current_row(), y + (active.current_column() * 2), "%c", active.current_value() );
    }

    /**************************
     *
     *************************/
    void draw_hint( const std::string hint )
    {
        mvprintw( 2, 23, hint.c_str() );
    }

     /**************************
      *
      *************************/   
    void draw_message( const std::string msg )
    {
        mvprintw( 23, 6, msg.c_str() );
    }

     /**************************
      *
      *************************/   
    void draw_text( const WtCoord& pos, const std::string text )
    {
        mvprintw( 23, 6, text.c_str() );
    }

    /**************************
     *
     *************************/
    void update()
    {
        refresh();
    }

    /**************************
     *
     *************************/
    void set_bg( const std::string )
    {
        clear();
        active_buttons = 0;
    }

    /**************************
     *
     *************************/
    void draw_pause_button( const WtCoord& pos )
    {

    }

    /**************************
     *
     *************************/
    void draw_button( const size_t& x, const size_t& y,
                      const size_t& w, const size_t& h,
                      const std::string img,
                      const std::string text )
    {
        WtCoord pos(x, y);
        uint8_t idx = active_buttons;
        for ( uint8_t i = 0; i < active_buttons; i++ )
            if ( buttons[i] == pos )
                idx = i;

        buttons[idx] = pos;
        mvprintw( idx+2, 5, "%d:%s (%d, %d)", idx+1, text.c_str(), pos.x, pos.y );
        if ( idx == active_buttons )
            active_buttons++;
    }

     /**************************
      *
      *************************/   
    WtInputEvent read_input()
    {
        WtInputEvent ev;
        ev.is_key_event = true;

        int ncurses_key = getch();


        switch( ncurses_key )
        {
            case ARROW_L:
                ev.key = wt_control_LEFT;
                break;
            case ARROW_R:
                ev.key = wt_control_RIGHT;
                break;
            case ARROW_D:
                ev.key = wt_control_DROP;
                break;
            case KEY_ENTER:
                ev.key = wt_control_QUIT;
                break;
           case 0x31:
           case 0x32:
           case 0x33:
           case 0x34:
           case 0x35:
                ev.is_key_event = false;
                ev.pos = buttons[ncurses_key-0x31];
                break;
            default:
                break;
        }
        return ev;
    }

    /**************************
      *
      *************************/   
    std::string get_key_map()
    {
        return std::string("Use Arrow-Keys for movement and ENTER for quitting\n");
    }
private:
    WtPolicyNcurses( const WtPolicyNcurses& ); 
    WtPolicyNcurses & operator = ( const WtPolicyNcurses& );


};

#endif /* _WT_NCURSES_H */
