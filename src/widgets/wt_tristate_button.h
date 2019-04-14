/*!*****************************************************************************
 * @file wt_tristate_button.h
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
#ifndef _WT_TRISTATE_BUTTON_H_
#define _WT_TRISTATE_BUTTON_H_

#include "widgets/wt_button.h"

class WtTriStateButton 
{
private:
    const char* m_tri_state_frame           =   "tri_state_btn.bmp";
    const char* m_tri_state_selected_img[3] = { "tri_state_btn_select0.bmp", 
                                                "tri_state_btn_select1.bmp", 
                                                "tri_state_btn_select2.bmp" };
    const char* m_tri_state_unselected_img  =   "tri_state_btn_select_none.bmp";

public:
    using OnItemTapDelegate = std::function<void(size_t)>;

    WtTriStateButton( WtCoord pos, 
                      WtDim size,
                      const std::array<const char*, 3>& label,
                      size_t selected,
                      OnItemTapDelegate on_item_tap ) :


        m_buttons{ 
            WtButton( pos, size, m_tri_state_frame, nullptr, "" ),
            WtButton( WtCoord( (pos.x + 1) + (0*((size.w - 1) / 3)), pos.y + 1 ),
                      WtDim( (size.w - 1) / 3, size.h - 2 ),
                      ( selected == 0 ? m_tri_state_selected_img[0] : m_tri_state_unselected_img ),
                      WT_BIND_EVENT_HANDLER( WtTriStateButton::on_item_clicked<0> ),
                      label[0] ),
            WtButton( WtCoord( (pos.x + 1) + (1*((size.w - 1) / 3)), pos.y + 1 ),
                      WtDim( (size.w - 1) / 3, size.h - 2 ),
                      ( selected == 1 ? m_tri_state_selected_img[1] : m_tri_state_unselected_img ),
                      WT_BIND_EVENT_HANDLER( WtTriStateButton::on_item_clicked<1> ),
                      label[1] ),
            WtButton( WtCoord( (pos.x + 1) + (2*((size.w - 1) / 3)), pos.y + 1 ),
                      WtDim( (size.w - 1) / 3, size.h - 2 ),
                      ( selected == 2 ? m_tri_state_selected_img[2] : m_tri_state_unselected_img ),
                      WT_BIND_EVENT_HANDLER( WtTriStateButton::on_item_clicked<2> ),
                      label[2] )
        },

        m_selected( selected ),
        m_on_item_tap( on_item_tap )
    {
    }
    
    ~WtTriStateButton() {}


    /**************************
     *
     *************************/
    template<size_t const idx>
    WtButton& item()
    {
        static_assert( idx < 4, "tri_state_button has only 3+1 items" );
        return m_buttons[idx];
    }


    /**************************
     *
     *************************/
    void select( uint8_t id )
    {
        if ( id < 3 )
        {
            m_selected = id;
        }
        else
        {
            std::cout << "id out of range\n";
        }
        update_images();
    }

private:
    /**************************
     *
     *************************/
    void update_images()
    {
        m_buttons[1].set_image( ( m_selected == 0 ? m_tri_state_selected_img[0] : m_tri_state_unselected_img ) );
        m_buttons[2].set_image( ( m_selected == 1 ? m_tri_state_selected_img[1] : m_tri_state_unselected_img ) );
        m_buttons[3].set_image( ( m_selected == 2 ? m_tri_state_selected_img[2] : m_tri_state_unselected_img ) );
    }

    /**************************
     *
     *************************/
    template<size_t const idx>
    void on_item_clicked()
    {
        if ( m_selected != idx )
        {
            m_selected = idx;
            update_images();
            if ( m_on_item_tap ) m_on_item_tap( m_selected );
        }
    }

private:
    WtTriStateButton( const WtTriStateButton& ); 
    WtTriStateButton& operator = (const WtTriStateButton&);

private:
    WtButton          m_buttons[4];
    size_t            m_selected;

    OnItemTapDelegate m_on_item_tap;
};

#endif /* _WT_TRISTATE_BUTTON_H_ */
