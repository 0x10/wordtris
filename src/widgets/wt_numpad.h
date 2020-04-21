/*!*****************************************************************************
 * @file wt_numpad.h
 * @brief button helper class
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
#ifndef _WT_NUMPAD_H_
#define _WT_NUMPAD_H_

#include "widgets/wt_button.h"

class WtNumPad
{
public:
    using OnItemTapDelegate = std::function<void(size_t)>;
    using OnEditActiveDelegate = std::function<void(bool)>;

    WtNumPad( WtCoord pos, 
                      WtDim size,
                      OnItemTapDelegate on_item_tap,
                      OnEditActiveDelegate on_edit_active = nullptr,
                      const char* item_image_selected = "",
                      const char* item_image_unselected = "",
                      const WtDim item_image_size = WtDim( 200, 100 ),
                      const std::array<const char*, 10>& label = std::array<const char*, 10>{{ "C", "1", "2", "3", "4", "5", "6", "7", "8", "9"}},
                      const std::string frame_image = "" ) :
        m_pos( pos ),
        m_size( size ),
        m_item_image_selected(item_image_selected),
        m_item_image_unselected(item_image_unselected),
        m_item_size( item_image_size ),
        m_buttons{ 
            WtButton( WtCoord( pos.x + (( size.w / 2 ) - ( (size.w / 3) / 2)), pos.y + 3*(item_image_size.h+2) ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<0> ),
                      label[0],
                      WtCoord(0,0), WtFont("#999999","text_big")),
            WtButton( WtCoord( pos.x + (((1-1)%3)*(size.w / 3)), pos.y ),
                      WtDim( size.w / 3-2, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<1> ),
                      label[1],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((2-1)%3)*(size.w / 3)), pos.y ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<2> ),
                      label[2],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((3-1)%3)*(size.w / 3+1)), pos.y ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<3> ),
                      label[3],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((4-1)%3)*(size.w / 3)), pos.y + item_image_size.h + 2 ),
                      WtDim( size.w / 3-2, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<4> ),
                      label[4],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((5-1)%3)*(size.w / 3)), pos.y + item_image_size.h + 2 ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<5> ),
                      label[5],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((6-1)%3)*(size.w / 3+1)), pos.y + item_image_size.h + 2 ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<6> ),
                      label[6],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((7-1)%3)*(size.w / 3)), pos.y + 2*(item_image_size.h+2) ),
                      WtDim( size.w / 3-2, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<7> ),
                      label[7],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((8-1)%3)*(size.w / 3)), pos.y + 2*(item_image_size.h+2) ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<8> ),
                      label[8],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( WtCoord( pos.x + (((9-1)%3)*(size.w / 3+1)), pos.y + 2*(item_image_size.h+2) ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<9> ),
                      label[9],
                      WtCoord(0,0), WtFont("#999999","text_big") ),
            WtButton( pos, size, frame_image, nullptr, "" )
        },
        m_btn_possible{ true, true, true, true, true,
                        true, true, true, true, true },
        m_edit_btn( WtCoord( m_pos.x + ((m_size.w / 3)/2 - 60 / 2), m_pos.y + 3*(item_image_size.h+2) ),
                    WtDim( 60,66 ),
                    "edit.bmp",
                    WT_BIND_EVENT_HANDLER( WtNumPad::on_edit_click ) ),
        m_more_btn( WtCoord( m_pos.x + ((m_size.w / 3)/2 - 60 / 2), m_pos.y + 3*(item_image_size.h+2) ),
                    WtDim( size.w / 3,item_image_size.h ),
                    item_image_unselected,
                    WT_BIND_EVENT_HANDLER( WtNumPad::on_more_click ),
                    WtL10n_tr("+"),
                    WtCoord(0,0), WtFont("#999999","text_big")),
        m_undo_btn( WtCoord( m_pos.x, m_pos.y  ),
                    WtDim( 0,0 ),
                    item_image_unselected,
                    WT_BIND_EVENT_HANDLER( WtNumPad::on_undo_click ),
                    WtL10n_tr("Undo"),
                    WtCoord(0,0), WtFont("#999999","text_big")),
        m_save_btn( WtCoord( m_pos.x, m_pos.y+item_image_size.h ),
                    WtDim( 0,0 ),
                    item_image_unselected,
                    WT_BIND_EVENT_HANDLER( WtNumPad::on_save_click ),
                    WtL10n_tr("Save"),
                    WtCoord(0,0), WtFont("#999999","text_big")),
        m_restore_btn( WtCoord( m_pos.x, m_pos.y + 2*item_image_size.h ),
                    WtDim( 0,0 ),
                    item_image_unselected,
                    WT_BIND_EVENT_HANDLER( WtNumPad::on_restore_click ),
                    WtL10n_tr("Restore"),
                    WtCoord(0,0), WtFont("#999999","text_big")),
        m_on_item_tap( on_item_tap ),
        m_on_edit_active( on_edit_active ),
        m_is_9x9(true),
        m_is_edit_active( false ),
        m_is_more_active( false ),
        m_restore_possible( false ),
        m_undo_possible( false )
    {

    

    }
    
    ~WtNumPad() {}


    /**************************
     *
     *************************/
    template<size_t const idx>
    WtButton& item()
    {
        static_assert( idx < 11, "numpad has only 10+1 items" );
        return m_buttons[idx];
    }

    /**************************
     *
     *************************/
    WtButton& special_button_left()
    {
        return m_edit_btn;
    }

    /**************************
     *
     *************************/
    WtButton& special_button_right()
    {
        return m_more_btn;
    }

    /**************************
     *
     *************************/
    WtButton& special_button_more_1()
    {
        return m_undo_btn;
    }

    /**************************
     *
     *************************/
    WtButton& special_button_more_2()
    {
        return m_save_btn;
    }

    /**************************
     *
     *************************/
    WtButton& special_button_more_3()
    {
        return m_restore_btn;
    }
    /**************************
     *
     *************************/
    void set_position( WtCoord new_pos )
    {
        m_pos = new_pos;
    }

    /**************************
     *
     *************************/
    bool is_edit_active() const
    {
        return m_is_edit_active;
    }

    /**************************
     *
     *************************/
    bool is_more_active() const
    {
        return m_is_more_active;
    }
    /**************************
     *
     *************************/
    void setup_4x4_layout()
    {
        m_is_9x9 = false;
        m_buttons[0].set_position( WtCoord( m_pos.x + (( m_size.w / 2 ) - ( (m_size.w / 3) / 2)), m_pos.y + 3*(m_item_size.h+2) ) );
        m_buttons[0].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[0] )   m_buttons[0].set_font_color( "#999999" );
        else                       m_buttons[0].set_font_color( "#333333" );

        m_buttons[1].set_position( WtCoord( m_pos.x + (((1-1)%2)*(m_size.w / 2)), m_pos.y ) );
        m_buttons[1].set_size( WtDim( m_size.w / 2, m_item_size.h ) );
        if ( m_btn_possible[1] )   m_buttons[1].set_font_color( "#999999" );
        else                       m_buttons[1].set_font_color( "#333333" );

        m_buttons[2].set_position( WtCoord( m_pos.x + (((2-1)%2)*(m_size.w / 2)), m_pos.y ) );
        m_buttons[2].set_size( WtDim( m_size.w / 2, m_item_size.h ) );
        if ( m_btn_possible[2] )   m_buttons[2].set_font_color( "#999999" );
        else                       m_buttons[2].set_font_color( "#333333" );

        m_buttons[3].set_position( WtCoord( m_pos.x + (((3-1)%2)*(m_size.w / 2)),  m_pos.y + (m_item_size.h + 2 + m_item_size.h/2)) );
        m_buttons[3].set_size( WtDim( m_size.w / 2, m_item_size.h ) );
        if ( m_btn_possible[3] )   m_buttons[3].set_font_color( "#999999" );
        else                       m_buttons[3].set_font_color( "#333333" );

        m_buttons[4].set_position( WtCoord( m_pos.x + (((4-1)%2)*(m_size.w / 2+1)), m_pos.y + (m_item_size.h + 2  + m_item_size.h/2)) );
        m_buttons[4].set_size( WtDim( m_size.w / 2, m_item_size.h ) );
        if ( m_btn_possible[4] )   m_buttons[4].set_font_color( "#999999" );
        else                       m_buttons[4].set_font_color( "#333333" );

        m_buttons[5].set_size( WtDim( 0,0 ) );
        m_buttons[6].set_size( WtDim( 0,0 ) );
        m_buttons[7].set_size( WtDim( 0,0 ) );
        m_buttons[8].set_size( WtDim( 0,0 ) );
        m_buttons[9].set_size( WtDim( 0,0 ) );
 
        m_edit_btn.set_position( WtCoord( m_pos.x + ((m_size.w / 2)/2 - 60 / 2), m_pos.y + 3*(m_item_size.h+2)+66/4 ) );
        m_edit_btn.set_size( WtDim( 60,66 ) );

        m_more_btn.set_label (WtL10n_tr("+"));
        m_more_btn.set_position( WtCoord( m_pos.x + (( m_size.w / 2 ) - ( (m_size.w / 3) / 2)) + (m_size.w / 3) , m_pos.y + 3*(m_item_size.h+2) ) );
        //( WtCoord(m_pos.x + (((4-1)%3)*(m_size.w / 2+1)), m_pos.y + 3*(m_item_size.h+2) ) );
        m_more_btn.set_size( WtDim( m_size.w / 6, m_item_size.h ) );

        m_undo_btn.set_size( WtDim( 0,0 ) );
        m_save_btn.set_size( WtDim( 0,0 ) );
        m_restore_btn.set_size( WtDim( 0,0 ) );
    }

    /**************************
     *
     *************************/
    void setup_9x9_layout()
    {
        m_is_9x9 = true;
        m_buttons[0].set_position( WtCoord( m_pos.x + (( m_size.w / 2 ) - ( (m_size.w / 3) / 2)), m_pos.y + 3*(m_item_size.h+2) ) );
        m_buttons[0].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[0] )   m_buttons[0].set_font_color( "#999999" );
        else                       m_buttons[0].set_font_color( "#333333" );

        m_buttons[1].set_position( WtCoord( m_pos.x + (((1-1)%3)*(m_size.w / 3)), m_pos.y ) );
        m_buttons[1].set_size( WtDim( m_size.w / 3-2, m_item_size.h ) );
        if ( m_btn_possible[1] )   m_buttons[1].set_font_color( "#999999" );
        else                       m_buttons[1].set_font_color( "#333333" );


        m_buttons[2].set_position( WtCoord( m_pos.x + (((2-1)%3)*(m_size.w / 3)), m_pos.y ) );
        m_buttons[2].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[2] )   m_buttons[2].set_font_color( "#999999" );
        else                       m_buttons[2].set_font_color( "#333333" );


        m_buttons[3].set_position( WtCoord( m_pos.x + (((3-1)%3)*(m_size.w / 3+1)), m_pos.y ) );
        m_buttons[3].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[3] )   m_buttons[3].set_font_color( "#999999" );
        else                       m_buttons[3].set_font_color( "#333333" );


        m_buttons[4].set_position( WtCoord( m_pos.x + (((4-1)%3)*(m_size.w / 3)), m_pos.y + m_item_size.h + 2 ) );
        m_buttons[4].set_size( WtDim( m_size.w / 3-2, m_item_size.h ) );
        if ( m_btn_possible[4] )   m_buttons[4].set_font_color( "#999999" );
        else                       m_buttons[4].set_font_color( "#333333" );


        m_buttons[5].set_position( WtCoord( m_pos.x + (((5-1)%3)*(m_size.w / 3)), m_pos.y + m_item_size.h + 2 ) );
        m_buttons[5].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[5] )   m_buttons[5].set_font_color( "#999999" );
        else                       m_buttons[5].set_font_color( "#333333" );


        m_buttons[6].set_position( WtCoord( m_pos.x + (((6-1)%3)*(m_size.w / 3+1)), m_pos.y + m_item_size.h + 2 ) );
        m_buttons[6].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[6] )   m_buttons[6].set_font_color( "#999999" );
        else                       m_buttons[6].set_font_color( "#333333" );


        m_buttons[7].set_position( WtCoord( m_pos.x + (((7-1)%3)*(m_size.w / 3)), m_pos.y + 2*(m_item_size.h+2) ) );
        m_buttons[7].set_size( WtDim( m_size.w / 3-2, m_item_size.h ) );
        if ( m_btn_possible[7] )   m_buttons[7].set_font_color( "#999999" );
        else                       m_buttons[7].set_font_color( "#333333" );


        m_buttons[8].set_position( WtCoord( m_pos.x + (((8-1)%3)*(m_size.w / 3)), m_pos.y + 2*(m_item_size.h+2) ) );
        m_buttons[8].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[8] )   m_buttons[8].set_font_color( "#999999" );
        else                       m_buttons[8].set_font_color( "#333333" );


        m_buttons[9].set_position( WtCoord( m_pos.x + (((9-1)%3)*(m_size.w / 3+1)), m_pos.y + 2*(m_item_size.h+2) ) );
        m_buttons[9].set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        if ( m_btn_possible[9] )   m_buttons[9].set_font_color( "#999999" );
        else                       m_buttons[9].set_font_color( "#333333" );


        
        m_edit_btn.set_position( WtCoord( m_pos.x + ((m_size.w / 3)/2 - 60 / 2), m_pos.y + 3*(m_item_size.h+2)+66/4 ) );
        m_edit_btn.set_size( WtDim( 60,66 ) );

        m_more_btn.set_label (WtL10n_tr("+"));
        m_more_btn.set_position( WtCoord( m_pos.x + (((9-1)%3)*(m_size.w / 3+1)), m_pos.y + 3*(m_item_size.h+2) ) );
        m_more_btn.set_size( WtDim( m_size.w / 3, m_item_size.h ) );
        m_undo_btn.set_size( WtDim( 0,0 ) );
        m_save_btn.set_size( WtDim( 0,0 ) );
        m_restore_btn.set_size( WtDim( 0,0 ) );
    }

    /**************************
     *
     *************************/
    void restore_possible( bool is_possible )
    {
        bool old_val = m_restore_possible;
        m_restore_possible = is_possible;
        if ( ( m_restore_possible != old_val) && m_is_more_active )
        {
            if ( m_is_9x9 ) setup_9x9_more();
            else            setup_4x4_more();
        }
    }

    /**************************
     *
     *************************/
    void undo_possible( bool is_possible )
    {
        bool old_val = m_undo_possible;
        m_undo_possible = is_possible;
        if ( ( m_undo_possible != old_val ) && m_is_more_active )
        {
            if ( m_is_9x9 ) setup_9x9_more();
            else            setup_4x4_more();
        }
    }
    /**************************
     *
     *************************/
    void disable_edit_mode()
    {
        if ( m_is_edit_active )
            on_edit_click();
    }


    /**************************
     *
     *************************/
    void set_item_possible( const size_t idx, bool is_possible )
    {
        bool old_val = m_btn_possible[idx];
        m_btn_possible[idx] = is_possible;
        if ( ( m_btn_possible[idx] != old_val ) && !m_is_more_active )
        {
            if ( m_is_9x9 ) setup_9x9_layout();
            else            setup_4x4_layout();
        }
    }
private:
    /**************************
     *
     *************************/
    void setup_9x9_more()
    {
        m_buttons[0].set_size( WtDim(0,0) );
        m_buttons[1].set_size( WtDim(0,0) );
        m_buttons[2].set_size( WtDim(0,0) );
        m_buttons[3].set_size( WtDim(0,0) );
        m_buttons[4].set_size( WtDim(0,0) );
        m_buttons[5].set_size( WtDim(0,0) );
        m_buttons[6].set_size( WtDim(0,0) );
        m_buttons[7].set_size( WtDim(0,0) );
        m_buttons[8].set_size( WtDim(0,0) );
        m_buttons[9].set_size( WtDim(0,0) );
        m_edit_btn.set_size( WtDim( 0,0 ) );

        m_undo_btn.set_size( WtDim( m_size.w, m_item_size.h ) );
        m_save_btn.set_size( WtDim( m_size.w, m_item_size.h ) );
        m_restore_btn.set_size( WtDim( m_size.w, m_item_size.h ) );
        if ( m_restore_possible ) m_restore_btn.set_font_color( "#999999" );
        else                      m_restore_btn.set_font_color( "#333333" );
        if ( m_undo_possible )    m_undo_btn.set_font_color( "#999999" );
        else                      m_undo_btn.set_font_color( "#333333" );

        m_more_btn.set_label (WtL10n_tr("-"));
    }

    /**************************
     *
     *************************/
    void setup_4x4_more()
    {
        m_buttons[0].set_size( WtDim(0,0) );
        m_buttons[1].set_size( WtDim(0,0) );
        m_buttons[2].set_size( WtDim(0,0) );
        m_buttons[3].set_size( WtDim(0,0) );
        m_buttons[4].set_size( WtDim(0,0) );
        m_edit_btn.set_size( WtDim( 0,0 ) );
       
        m_undo_btn.set_size( WtDim( m_size.w, m_item_size.h ) );
        m_save_btn.set_size( WtDim( m_size.w, m_item_size.h ) );
        m_restore_btn.set_size( WtDim( m_size.w, m_item_size.h ) );
        if ( m_restore_possible ) m_restore_btn.set_font_color( "#999999" );
        else                      m_restore_btn.set_font_color( "#333333" );
        if ( m_undo_possible )    m_undo_btn.set_font_color( "#999999" );
        else                      m_undo_btn.set_font_color( "#333333" );

        m_more_btn.set_label (WtL10n_tr("-"));
    }

    /**************************
     *
     *************************/
    template<size_t const idx>
    void on_item_clicked()
    {
        if (( ! m_is_9x9 ) && (idx > 4))
            return;

        if ( m_on_item_tap ) m_on_item_tap( idx );
    }

    /**************************
     *
     *************************/
    void on_edit_click()
    {
        if ( !m_is_edit_active )
        {
            m_is_edit_active = true;
            m_edit_btn.set_image( "edit_active.bmp" );
            if ( m_on_edit_active ) m_on_edit_active( m_is_edit_active );
        }
        else
        {
            m_is_edit_active = false;
            m_edit_btn.set_image( "edit.bmp" );
            if ( m_on_edit_active ) m_on_edit_active( m_is_edit_active );
        }
        if ( m_on_item_tap ) m_on_item_tap( 10 );
    }

    /**************************
     *
     *************************/
    void on_more_click()
    {
        if ( ! m_is_more_active )
        {
            m_is_more_active = true;
            if ( m_is_9x9 ) setup_9x9_more();
            else            setup_4x4_more();
        }
        else
        {
            m_is_more_active = false;
            if ( m_is_9x9 ) setup_9x9_layout();
            else            setup_4x4_layout();
        }
        if ( m_on_item_tap ) m_on_item_tap( 11 );
    }

    /**************************
     *
     *************************/
    void on_undo_click()
    {
        if ( m_on_item_tap ) m_on_item_tap( 12 );
    }


    /**************************
     *
     *************************/
    void on_save_click()
    {
        if ( m_on_item_tap ) m_on_item_tap( 13 );
    }

    /**************************
     *
     *************************/
    void on_restore_click()
    {
        if ( m_restore_possible )
            if ( m_on_item_tap ) m_on_item_tap( 14 );
    }
private:
    WtNumPad( const WtNumPad& ); 
    WtNumPad& operator = (const WtNumPad&);

private:
    WtCoord           m_pos;
    const WtDim       m_size;
    const char*       m_item_image_selected;
    const char*       m_item_image_unselected;
    const WtDim       m_item_size;

    WtButton          m_buttons[11];
    bool              m_btn_possible[10];
    WtButton          m_edit_btn;
    WtButton          m_more_btn;
    WtButton          m_undo_btn;
    WtButton          m_save_btn;
    WtButton          m_restore_btn;
   

    OnItemTapDelegate m_on_item_tap;
    OnEditActiveDelegate m_on_edit_active;
    bool              m_is_9x9;
    bool              m_is_edit_active;
    bool              m_is_more_active;
    bool              m_restore_possible;
    bool              m_undo_possible;
};

#endif /* _WT_NUMPAD_H_ */
