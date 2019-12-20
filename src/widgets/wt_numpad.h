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

    WtNumPad( WtCoord pos, 
                      WtDim size,
                      OnItemTapDelegate on_item_tap,
                      const char* item_image_selected = "",
                      const char* item_image_unselected = "",
                      const WtDim item_image_size = WtDim( 200, 100 ),
                      const std::array<const char*, 10>& label = std::array<const char*, 10>{{ "C", "1", "2", "3", "4", "5", "6", "7", "8", "9"}},
                      const std::string frame_image = "" ) :

        m_item_image_selected(item_image_selected),
        m_item_image_unselected(item_image_unselected),
        m_buttons{ 
            WtButton( WtCoord( pos.x + (( size.w / 2 ) - ( (size.w / 3) / 2)), pos.y + 3*(item_image_size.h+2) ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<0> ),
                      label[0],
                      WtCoord(0,0), WtFont("#4a90e2","text_big")),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((1-1)%3)*(size.w / 3)), pos.y ),
                      WtDim( size.w / 3-2, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<1> ),
                      label[1],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((2-1)%3)*(size.w / 3)), pos.y ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<2> ),
                      label[2],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((3-1)%3)*(size.w / 3+1)), pos.y ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<3> ),
                      label[3],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((4-1)%3)*(size.w / 3)), pos.y + item_image_size.h + 2 ),
                      WtDim( size.w / 3-2, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<4> ),
                      label[4],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((5-1)%3)*(size.w / 3)), pos.y + item_image_size.h + 2 ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<5> ),
                      label[5],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((6-1)%3)*(size.w / 3+1)), pos.y + item_image_size.h + 2 ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<6> ),
                      label[6],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((7-1)%3)*(size.w / 3)), pos.y + 2*(item_image_size.h+2) ),
                      WtDim( size.w / 3-2, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<7> ),
                      label[7],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((8-1)%3)*(size.w / 3)), pos.y + 2*(item_image_size.h+2) ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<8> ),
                      label[8],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( WtCoord( ( pos.x + ( (size.w / 3)/2 - ((size.w/3)/2) ) ) + (((9-1)%3)*(size.w / 3+1)), pos.y + 2*(item_image_size.h+2) ),
                      WtDim( size.w / 3, item_image_size.h ),
                      item_image_unselected,
                      WT_BIND_EVENT_HANDLER( WtNumPad::on_item_clicked<9> ),
                      label[9],
                      WtCoord(0,0), WtFont("#4a90e2","text_big") ),
            WtButton( pos, size, frame_image, nullptr, "" )
        },

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
    void select( uint8_t idx )
    {

    }

private:
    /**************************
     *
     *************************/
    template<size_t const idx>
    void on_item_clicked()
    {

        if ( m_on_item_tap ) m_on_item_tap( idx );
    }

private:
    WtNumPad( const WtNumPad& ); 
    WtNumPad& operator = (const WtNumPad&);

private:
    const char*       m_item_image_selected;
    const char*       m_item_image_unselected;

    WtButton          m_buttons[11];
    OnItemTapDelegate m_on_item_tap;
};

#endif /* _WT_NUMPAD_H_ */
