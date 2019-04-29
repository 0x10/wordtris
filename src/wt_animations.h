/*!*****************************************************************************
 * @file wt_animations.h
 * @brief base implementations and derived types for animations
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
#ifndef _WT_ANIMATIONS_H_
#define _WT_ANIMATIONS_H_

#include "wt_view_if.h"
#include "wt_utils.h"
#include <vector>

/**************************
 *
 *************************/
template<typename animation_content>
class WtAnimationStep
{
public:
    WtAnimationStep( animation_content c, const WtTime::TimeType duration  ) :
        content( c ),
        step_duration( duration )
    {}

    ~WtAnimationStep() {}

public:
    animation_content content;
    WtTime::TimeType  step_duration;
};

/**************************
 *
 *************************/
class WtGridAnimation : public WtViewIf
{
public:
    using OnUpdateOverlayDelegate = std::function<void(void)>;
    struct GridText {
        GridText( uint8_t r,
                  uint8_t c,
                  bool    horiz,
                  std::string t,
                  std::string f ) :
            text( t ),
            font( f ),
            row ( r ),
            col ( c ),
            horizontal( horiz )
            {}
        std::string text;
        std::string font;
        uint8_t row;
        uint8_t col;
        bool    horizontal;
    };
    struct GridContent {
        GridContent( uint8_t r,
                     uint8_t c,
                     uint8_t r_count,
                     uint8_t c_count,
                     std::string f,
                     std::vector<char> cells ) :
            row( r ),
            col( c ),
            row_count( r_count ),
            col_count( c_count ),
            font( f ),
            cell_content( cells )
        {}

        uint8_t row;
        uint8_t col;
        uint8_t row_count;
        uint8_t col_count;
        std::string font;
        std::vector<char> cell_content;
    };
    typedef WtAnimationStep<GridContent> GridAnimationStep;

    /**************************
     *
     *************************/
    static const WtGridAnimation& no_animation()
    {
        static const WtGridAnimation empty;
        return empty;
    }

public:
    WtGridAnimation() :
        WtViewIf( "bg.bmp", false ),
        m_animation_steps(),
        m_update_overlay( nullptr ),
        m_a_idx( 0 )
     {
     }
    ~WtGridAnimation() {}

    /**************************
     *
     *************************/
    void push_back( const GridAnimationStep& step )
    {
        m_animation_steps.push_back( step );
    }

    /**************************
     *
     *************************/
    const GridAnimationStep& operator[]( const size_t idx) const
    {
        return m_animation_steps[idx];
    }

    /**************************
     *
     *************************/
    bool empty() const
    {
        return m_animation_steps.empty();
    }


    /**************************
     *
     *************************/
    static GridContent fromGridText( GridText gt )
    {
        GridContent gc( gt.row, gt.col,
                        ( gt.horizontal ? 1 : gt.text.length() ),
                        ( gt.horizontal ? gt.text.length() : 1 ),
                        gt.font, 
                        std::vector<char>(gt.text.begin(), gt.text.end()));
        return gc;
    }

    /**************************
     *
     *************************/
    void set_overlay_drawing( OnUpdateOverlayDelegate update_overlay )
    {
        m_update_overlay = update_overlay;
    }


private:
    /**************************
     *
     *************************/
    void draw_animation_step( const GridAnimationStep& step )
    {
        for ( uint8_t r_idx = 0; r_idx < step.content.row_count; r_idx++ )
        {
            for ( uint8_t c_idx = 0; c_idx < step.content.col_count; c_idx++ )
            {
                size_t cell_idx = static_cast<size_t>( (r_idx * step.content.col_count) + c_idx );
                ACTIVE_WINDOW.draw_at_grid( step.content.row + r_idx,
                                            step.content.col + c_idx,
                                            step.content.cell_content[cell_idx],
                                            step.content.font );
            }
        }
    }

    /**************************
     *
     *************************/
    virtual void draw_view()
    {
        // empty instance just to omit drawing
    }

    /**************************
     *
     *************************/
    virtual void update_view()
    {
        if ( m_update_overlay ) m_update_overlay();
        if ( m_a_idx < m_animation_steps.size() )
        {
            draw_animation_step( m_animation_steps[m_a_idx] );

            ACTIVE_WINDOW.update();
            WtTime::sleep( m_animation_steps[m_a_idx].step_duration );
            m_a_idx++;
        }
        else
        {
            ACTIVE_WINDOW.update();
        }

        if ( m_a_idx >= m_animation_steps.size() )
        {
            m_a_idx = 0;
            leave();
        }
    }

    /**************************
     *
     *************************/
    void left_view()
    {
        m_a_idx = 0;
    }
private:
    std::vector< GridAnimationStep > m_animation_steps;
    OnUpdateOverlayDelegate m_update_overlay;
    size_t m_a_idx;
};

/**************************
 *
 *************************/
class WtGridAnimationBuilder
{
public:
    /**************************
     *
     *************************/
    static void construct_running_letter_animation( WtGridAnimation& animation,
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
    static void construct_pause_animation( WtGridAnimation& pause_animation )
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
                                            WtTime::from_milliseconds(30) );

        WtGridAnimation::GridContent three( final_row, 0, row_count, WtBoard::col_count,
                                            font, cells_three );
        WtGridAnimation::GridAnimationStep step_three( three, WtTime::from_milliseconds(300) );
        pause_animation.push_back( step_three );



        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_2,
                                            cells_two,
                                            final_row, 1,
                                            WtTime::from_milliseconds(30) );

        WtGridAnimation::GridContent two( final_row, 1, row_count, WtBoard::col_count-2,
                                          font, cells_two );
        WtGridAnimation::GridAnimationStep step_two( two, WtTime::from_milliseconds(300));
        pause_animation.push_back( step_two );



        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_1,
                                            cells_one,
                                            final_row, 2,
                                            WtTime::from_milliseconds(30) );

        WtGridAnimation::GridContent one( final_row, 2, row_count, WtBoard::col_count-4,
                                          font, cells_one );
        WtGridAnimation::GridAnimationStep step_one( one, WtTime::from_milliseconds(300));

        pause_animation.push_back( step_one );
    }

};


#endif /* _WT_ANIMATIONS_H_ */
