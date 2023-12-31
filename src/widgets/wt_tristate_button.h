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
public:
    using OnItemTapDelegate = std::function<void(size_t)>;

    WtTriStateButton( WtCoord pos, 
                      WtDim size,
                      size_t selected,
                      OnItemTapDelegate on_item_tap,
                      const std::array<const char*, 3>& label,
                      const std::array<const char*, 3>& item_images_selected = std::array<const char*, 3>{{ "tri_state_btn_selected.bmp",
                                                                                                            "tri_state_btn_selected.bmp",
                                                                                                            "tri_state_btn_selected.bmp" }},
                      const std::array<const char*, 3>& item_images_unselected = std::array<const char*, 3>{{ "tri_state_btn_unselected.bmp",
                                                                                                              "tri_state_btn_unselected.bmp",
                                                                                                              "tri_state_btn_unselected.bmp" }},
                      const WtDim item_image_size = WtDim( 60, 60 ),
                      const std::string frame_image = "" ) :

        m_item_images_selected(item_images_selected),
        m_item_images_unselected(item_images_unselected),
        m_buttons{ 
            WtButton( pos, size, frame_image, nullptr, "" ),
            WtButton( WtCoord( (pos.x ) + (0*((size.w ) / 3)), pos.y  ),
                      item_image_size,
                      ( selected == 0 ? item_images_selected[0] : item_images_unselected[0] ),
                      WT_BIND_EVENT_HANDLER( WtTriStateButton::on_item_clicked<0> ),
                      label[0],
                      WtCoord( 0, item_image_size.h ) ),
            WtButton( WtCoord( (pos.x ) + (1*((size.w ) / 3)), pos.y  ),
                      item_image_size,
                      ( selected == 1 ? item_images_selected[1] : item_images_unselected[1] ),
                      WT_BIND_EVENT_HANDLER( WtTriStateButton::on_item_clicked<1> ),
                      label[1],
                      WtCoord( 0, item_image_size.h ) ),
            WtButton( WtCoord( (pos.x ) + (2*((size.w ) / 3)), pos.y  ),
                      item_image_size,
                      ( selected == 2 ? item_images_selected[2] : item_images_unselected[2] ),
                      WT_BIND_EVENT_HANDLER( WtTriStateButton::on_item_clicked<2> ),
                      label[2],
                      WtCoord( 0, item_image_size.h ) )
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
        m_buttons[1].set_image( ( m_selected == 0 ? m_item_images_selected[0] : m_item_images_unselected[0] ) );
        m_buttons[2].set_image( ( m_selected == 1 ? m_item_images_selected[1] : m_item_images_unselected[1] ) );
        m_buttons[3].set_image( ( m_selected == 2 ? m_item_images_selected[2] : m_item_images_unselected[2] ) );
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
    const std::array<const char*, 3> m_item_images_selected;
    const std::array<const char*, 3> m_item_images_unselected;
    WtButton          m_buttons[4];
    size_t            m_selected;
    OnItemTapDelegate m_on_item_tap;
};

#endif /* _WT_TRISTATE_BUTTON_H_ */
