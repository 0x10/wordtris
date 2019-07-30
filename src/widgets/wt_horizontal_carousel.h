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
    const WtDim m_item_img_size = WtDim( 203, 297 );
    const uint8_t m_item_padding = 24;

public:
    using OnGameSelected = std::function<void(size_t)>;


    WtHorizontalCarousel( WtCoord pos, WtDim size,
                          std::vector<std::string> labels,
                          size_t selected,
                          OnGameSelected on_game_selected,
                          bool scrollable ) :

        m_clickable( pos, size, 
                     WT_BIND_EVENT_HANDLER_1( WtHorizontalCarousel::on_click ),
                     WT_BIND_EVENT_HANDLER_3( WtHorizontalCarousel::on_pan ) ),

        m_pos( pos ),
        m_size( size ),
        m_labels( labels ),
        m_selected( selected ),
        m_selected_pos( (size.w - m_item_img_size.w) / 2, pos.y ),
        m_scrollable( scrollable ),
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
        if ( m_labels.size() > idx )
        {
            if ( m_scrollable )
            {
                if ( idx == m_selected )
                {
                    return WtButton( get_pos_of_item( 0 ),
                                     m_item_img_size,
                                     m_selected_img,
                                     [](){},
                                     m_labels[idx] );
                }
                else if ( idx == get_prev_idx( m_selected ) )
                {
                    return WtButton( get_pos_of_item( -1 ),
                                     m_item_img_size,
                                     m_inactive_img,
                                     [](){},
                                     m_labels[idx] );
                }
                else if ( idx == get_next_idx( m_selected ) )
                {
                    return WtButton( get_pos_of_item( 1 ),
                                     m_item_img_size,
                                     m_inactive_img,
                                     [](){},
                                     m_labels[idx] );
                }
                else
                {
                    return WtButton( get_pos_of_item( 0 ),
                                     WtDim( 0, 0 ),
                                     m_inactive_img,
                                     [](){},
                                     m_labels[idx] );
                }
            }
            else
            {
                ssize_t normalized_id = static_cast<ssize_t>(idx % m_labels.size()) - (m_labels.size() == 1 ? 0 : 1);
                return WtButton( get_pos_of_item( normalized_id ),
                                 m_item_img_size,
                                 ( idx == m_selected ? m_selected_img : m_inactive_img ),
                                 [](){},
                                 m_labels[idx] );
            }
        }
        else
        {
            return WtButton( get_pos_of_item( 0 ),
                             m_item_img_size,
                             m_selected_img,
                             [](){},
                             "NoLabel" );
        }
    }

    /**************************
     *
     *************************/
    size_t size() const
    {
        return m_labels.size();
    }


    /**************************
     *
     *************************/
    void set_labels( std::vector<std::string> labels )
    {
        if ( labels.size() == m_labels.size() )
        {
            m_labels.clear();
            m_labels = labels;
        }
    }


    /**************************
     *
     *************************/
    size_t selected() const
    {
        return m_selected;
    }
private:
    /**************************
     *
     *************************/
    WtCoord get_pos_of_item( ssize_t idx )
    {
        return WtCoord( m_selected_pos.x + ( ( m_item_img_size.w + m_item_padding ) * idx ), m_pos.y );

    }

    /**************************
     *
     *************************/
    size_t get_next_idx( const size_t idx )
    {
        return ( idx + 1 ) % m_labels.size();
    }

    /**************************
     *
     *************************/
    size_t get_prev_idx( const size_t idx )
    {
        size_t res = idx;

        if ( idx == 0 )
            res = m_labels.size() - 1;
        else
            res--;

        return res;
    }
public:
    /**************************
     *
     *************************/
    void on_click( WtCoord& pos )
    {
        if ( m_scrollable )
        {
            if ( pos.in_region( m_selected_pos, m_item_img_size ) )
            {
                if ( m_on_game_selected ) m_on_game_selected( m_selected );
            }
            else
            {
                if ( pos.x > ( m_selected_pos.x + m_item_img_size.w ) )
                {
                    m_selected = get_next_idx( m_selected );
                    m_selected_pos.x = (m_size.w - m_item_img_size.w) / 2;
                }
                else
                {
                    m_selected = get_prev_idx( m_selected );
                    m_selected_pos.x = (m_size.w - m_item_img_size.w) / 2;
                }
            }
        }
        else
        {
            for( size_t item_idx = 0; item_idx < m_labels.size(); item_idx++ )
            {
                ssize_t normalized_id = static_cast<ssize_t>(item_idx) - 1;
                if ( pos.in_region( get_pos_of_item ( normalized_id ), m_item_img_size ) )
                {
                    if ( item_idx == m_selected )
                    {
                        if ( m_on_game_selected ) m_on_game_selected( m_selected );
                    }
                    else
                    {
                        m_selected = item_idx;
                    }

                    break;
                }
            }
        }
    }

    /**************************
     *
     *************************/
    void on_pan( WtCoord& /*press_start_pos*/, WtCoord& /*active_pos*/, WtCoord& d_pos )
    {
        if ( ( m_scrollable ) && ( m_labels.size() > 1 ) )
        {
           // std::cout << "on pan ... " << press_start_pos << ";"<< active_pos << ";" << d_pos << std::endl;
            m_selected_pos.x = m_selected_pos.x + d_pos.x;
            if ( ( m_selected_pos.x + m_item_img_size.w ) >= m_size.w )
            {
                m_selected = get_prev_idx( m_selected );
                m_selected_pos.x = m_selected_pos.x - (m_item_img_size.w+m_item_padding);
            }
            if ( m_selected_pos.x <= ( m_size.w - ((2*m_item_img_size.w)+m_item_padding) ) )
            {
                m_selected = get_next_idx( m_selected );
                m_selected_pos.x = m_selected_pos.x + (m_item_img_size.w+m_item_padding);
            }
        }
    }

private:
    WtHorizontalCarousel( const WtHorizontalCarousel& ); 
    WtHorizontalCarousel & operator = (const WtHorizontalCarousel &);

private:
    WtClickableIf m_clickable;

    const WtCoord   m_pos;
    const WtDim     m_size;

    std::vector<std::string> m_labels;
    size_t                   m_selected;
    WtCoord                  m_selected_pos;
    bool                     m_scrollable;

    OnGameSelected  m_on_game_selected;
};


#endif /* _WT_HORIZONTAL_CAROUSEL_H_ */
