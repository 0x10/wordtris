/*!*****************************************************************************
 * @file wt_drawing.h
 * @brief generic, configurable implementation for drawing
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
#ifndef _WT_DRAWING_H_
#define _WT_DRAWING_H_

#include "global.h"

#include "wt_player.h"
#include "wt_board.h"
#include "wt_active_letter.h"

#define WINDOW( Policy )  WtDrawing<Policy>::instance()

template<typename DrawingPolicy>
class WtDrawing : private DrawingPolicy
{
// singleton definition
public:
    static WtDrawing& instance()
    {
        static WtDrawing _instance;
        return _instance;
    }
    ~WtDrawing() {}
private:
    WtDrawing() :
        DrawingPolicy()
    {
    }
    WtDrawing( const WtDrawing& ); 
    WtDrawing & operator = (const WtDrawing &);

// api defintion
public:
    /**************************
     *
     *************************/
    void init() {}

    /**************************
     *
     *************************/
    void clr()
    {
        DrawingPolicy::clr();
    }

    /**************************
     *
     *************************/
    void draw_player_stat( const WtPlayer& player )
    {
        DrawingPolicy::draw_player_stat( player );
    }

    /**************************
     *
     *************************/
    void draw_board( const WtBoard& board )
    {
        DrawingPolicy::draw_board( board );
    }

    /**************************
     *
     *************************/
    void draw_hint( const std::string hint )
    {
        DrawingPolicy::draw_hint( hint );
    }

    /**************************
     *
     *************************/
    void draw_active_letter( const WtLetter& active )
    {
        DrawingPolicy::draw_active_letter( active );
    }

     /**************************
      *
      *************************/   
    void draw_message( const std::string msg )
    {
        DrawingPolicy::draw_message( msg );
    }

    /**************************
     *
     *************************/
    void update()
    {
        DrawingPolicy::update( );
    }

    /**************************
     *
     *************************/
    void set_bg( const std::string bg_file )
    {
        DrawingPolicy::set_bg( bg_file );
    }

    /**************************
      *
      *************************/   
    void draw_text( const WtCoord&    pos,
                    const std::string text )
    {
        DrawingPolicy::draw_text( pos, text );
    }

    /**************************
      *
      *************************/   
    void draw_button( const WtCoord&    pos,
                      const std::string text )
    {
        DrawingPolicy::draw_button( pos, text );
    }

    /**************************
      *
      *************************/   
    void draw_pause_button( const WtCoord& pos )
    {
        DrawingPolicy::draw_pause_button( pos );
    }
private:
};


#endif /* _WT_DRAWING_H */
