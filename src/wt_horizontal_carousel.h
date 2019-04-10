/*!*****************************************************************************
 * @file wt_horizontal_carousel.h
 * @brief carousel helper class
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
#ifndef _WT_HORIZONTAL_CAROUSEL_H_
#define _WT_HORIZONTAL_CAROUSEL_H_


#include "wt_clickable_if.h"


class WtHorizontalCarousel
{
private:
    const char* m_inactive_img = "list_item_inactive.bmp";
    const char* m_selected_img = "list_item_active.bmp";
    const WtDim m_item_img_size = WtDim( 328, 200 );

public:
    using OnGameSelected = std::function<void(size_t)>;


    WtHorizontalCarousel( WtCoord pos, WtDim size,
                          std::vector<std::string> labels,
                          size_t selected,
                          OnGameSelected on_game_selected ) :

        m_clickable( std::bind ( &WtHorizontalCarousel::on_press, this, std::placeholders::_1 ),
                     std::bind ( &WtHorizontalCarousel::on_release, this, std::placeholders::_1 ),
                     std::bind ( &WtHorizontalCarousel::on_motion, this, std::placeholders::_1, std::placeholders::_2 ) ),

        m_pos( pos ),
        m_size( size ),
        m_labels( labels ),
        m_selected( selected ),
        m_first_pos( (size.w - m_item_img_size.w) / 2, 0 ),
        m_press_start_pos( -1, -1 ),
        m_on_game_selected( on_game_selected )
    {
    }
    
    ~WtHorizontalCarousel() {}

    /**************************
     *
     *************************/
    operator WtClickableIf&()
    {
        return get_observable();
    }

    /**************************
     *
     *************************/
    WtClickableIf& get_observable()
    {
        return m_clickable;
    }

    /**************************
     *
     *************************/
    WtButton operator[](size_t idx)
    {
        return WtButton( get_pos_of_item( idx ),
                         m_item_img_size,
                         ( idx == m_selected ? m_selected_img : m_inactive_img ),
                         [](){},
                         m_labels[idx] );
    }

    /**************************
     *
     *************************/
    size_t size() const
    {
        return m_labels.size();
    }

private:
    /**************************
     *
     *************************/
    WtCoord get_pos_of_item( size_t idx )
    {
        return WtCoord(m_first_pos.x+(328+6)*idx,0);

    }

public:
    /**************************
     *
     *************************/
    void on_press( WtCoord& pos )
    {
        std::cout << "on press at " << pos << std::endl;
        if ( pos.in_region( m_pos, m_size ) )
        {
            m_press_start_pos = pos;
            m_active_motion_pos = pos;
        }
    }

    /**************************
     *
     *************************/
    void on_release( WtCoord& pos )
    {
//        std::cout << "release detected: (" << pos.x << "," << pos.y << ") -> (" << m_pos.x << "," << m_pos.y << "):(" << m_size.w << "," << m_size.h << ") -> " << m_label << std::endl;
        if ( m_press_start_pos == WtCoord( -1, -1 ) )
        {
            if ( pos.in_region( m_pos, m_size ) )
            {
            }
        }
        m_press_start_pos = WtCoord( -1, -1 );
        m_active_motion_pos = m_press_start_pos;
    }

    /**************************
     *
     *************************/
    void on_motion( WtCoord& /*pos*/, WtCoord& d_pos )
    {
        if ( m_press_start_pos != WtCoord( -1, -1 ) )
        {
            m_first_pos.x = m_first_pos.x + d_pos.x;
#if 0
                        if ( ( drag_btn->x() < 0 ) || ( drag_btn->x()+drag_btn->width() > ACTIVE_WINDOW_WIDTH ) )
                           drag_btn->set_image("list_item_inactive.bmp"); 
                        else
                           drag_btn->set_image("list_item_active.bmp"); 

#endif
            std::cout << "first = " << m_first_pos<<std::endl;
            m_active_motion_pos = m_active_motion_pos + d_pos;
        }
    }

private:
    WtClickableIf m_clickable;

    const WtCoord   m_pos;
    const WtDim     m_size;

    std::vector<std::string> m_labels;
    size_t                   m_selected;
    WtCoord         m_first_pos;
    WtCoord         m_press_start_pos;
    WtCoord         m_active_motion_pos;

    OnGameSelected  m_on_game_selected;
};


#endif /* _WT_HORIZONTAL_CAROUSEL_H_ */
