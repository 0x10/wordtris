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
#define NUM_BUTTON( ID ) \
            WtButton( WtCoord( (pos.x ) + (ID*((size.w ) / 10)), pos.y  ),              \
                      item_image_size,                                                  \
                      ( selected == 0 ? item_image_selected : item_image_unselected ),  \
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<ID> ),           \
                      label[ID],                                                        \
                      WtCoord( 0, item_image_size.h ) )
public:
    using OnItemTapDelegate = std::function<void(size_t)>;

    WtNumPad( WtCoord pos, 
                      WtDim size,
                      size_t selected,
                      OnItemTapDelegate on_item_tap,
                      const char* item_image_selected = "tri_state_btn_selected.bmp",
                      const char* item_image_unselected = "tri_state_btn_unselected.bmp",
                      const WtDim item_image_size = WtDim( 60, 60 ),
                      const std::array<const char*, 10>& label = std::array<const char*, 10>{{ "C", "1", "2", "3", "4", "5", "6", "7", "8", "9"}},
                      const std::string frame_image = "" ) :

        m_item_image_selected(item_image_selected),
        m_item_image_unselected(item_image_unselected),
        m_buttons{ 
            NUM_BUTTON( 0 ),
            NUM_BUTTON( 1 ),
            NUM_BUTTON( 2 ),
            NUM_BUTTON( 3 ),
            NUM_BUTTON( 4 ),
            NUM_BUTTON( 5 ),
            NUM_BUTTON( 6 ),
            NUM_BUTTON( 7 ),
            NUM_BUTTON( 8 ),
            NUM_BUTTON( 9 ),
            WtButton( pos, size, frame_image, nullptr, "" )
        },

        m_selected( selected ),
        m_on_item_tap( on_item_tap )
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
    void select( uint8_t id )
    {
        if ( id < 10 )
        {
            m_buttons[m_selected].set_image( m_item_image_unselected );
            m_selected = id;
            m_buttons[m_selected].set_image( m_item_image_selected );
        }
        else
        {
            std::cout << "id out of range\n";
        }
    }

private:
    /**************************
     *
     *************************/
    template<size_t const idx>
    void on_item_clicked()
    {
        if ( m_selected != idx )
        { 
            m_buttons[m_selected].set_image( m_item_image_unselected );
            m_selected = idx;
            m_buttons[m_selected].set_image( m_item_image_selected );
        }
        if ( m_on_item_tap ) m_on_item_tap( m_selected );
    }

private:
    WtNumPad( const WtNumPad& ); 
    WtNumPad& operator = (const WtNumPad&);

private:
    const char*       m_item_image_selected;
    const char*       m_item_image_unselected;

    WtButton          m_buttons[11];
    size_t            m_selected;
    OnItemTapDelegate m_on_item_tap;
};

#endif /* _WT_NUMPAD_H_ */
