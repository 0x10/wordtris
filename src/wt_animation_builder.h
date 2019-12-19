/*!*****************************************************************************
 * @file wt_animation_builder.h
 * @brief examples and skeletons for animations
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
#ifndef _WT_ANIMATION_BUILDER_H_
#define _WT_ANIMATION_BUILDER_H_

#include "wt_animations.h"

/**************************
 *
 *************************/
class WtTextAnimationBuilder
{
public:
    /**************************
     *
     *************************/
    static void construct_level_up_animation( WtTextAnimation& animation )
    {
        std::cout << "construct level up animation\n";
        WtCoord start_pos( 0, 0 );

        for ( ssize_t y_idx = 0; y_idx < 500; y_idx++)
        {
            start_pos.y = y_idx;
            WtTextAnimation::TextAnimationStep step( WtAnimatableText( start_pos,
                                                                       "Level  Up!",
                                                                       "text" ),
                                                     WtTime::from_milliseconds(20) );
            animation.push_back( step );
        }
    }
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
    static void construct_level_up_animation( WtGridAnimation& animation )
    {
        std::cout << "construct level up animation\n";
        WtGridAnimation::GridAnimationStep step( WtGridAnimation::fromGridText( WtGridAnimation::GridText( 2, 0,
                                                                                                           true,
                                                                                                            "Level  Up!", 
                                                                            "grid_inverse" ) ),
                                                 WtTime::from_milliseconds(200), 2, "levelup" );
        animation.push_back( step );
        step.content.font = "grid";
        step.initial_sfx = "";
        animation.push_back( step );
        step.content.font = "grid_inverse";
        animation.push_back( step );
    }

    /**************************
     *
     *************************/
    static void construct_running_letter_animation( WtGridAnimation& animation,
                                             const uint8_t row_count, const uint8_t col_count,
                                             std::vector<char> letter_data,
                                             const uint8_t drop_until_row,
                                             const uint8_t col_offset,
                                             WtTime::TimeType speed_per_row,
                                             std::string init_sfx="" )
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

                WtAnimatableGridContent step_content( 1, col_offset, draw_idx, col_count, font, row_cells );
                if ( draw_idx == 1 )
                {
                    WtGridAnimation::GridAnimationStep step_next( step_content, speed_per_row, 0, init_sfx );
                    animation.push_back( step_next );
                }
                else
                {
                    WtGridAnimation::GridAnimationStep step_next( step_content, speed_per_row );
                    animation.push_back( step_next );
                }
                
                
            }
            else
            {
                WtAnimatableGridContent letter( draw_idx-row_count, col_offset, row_count, col_count,
                                                     font, letter_data );
                if ( draw_idx == 1 )
                {
                    WtGridAnimation::GridAnimationStep step_next( letter, speed_per_row, 0, init_sfx );
                    animation.push_back( step_next );
                }
                else
                {
                    WtGridAnimation::GridAnimationStep step_next( letter, speed_per_row );
                    animation.push_back( step_next );
                }
            }
        }
    }


    /**************************
     *
     *************************/
    static void construct_pause_animation( WtGridAnimation& pause_animation )
    {
#if 0
        const char x = WtBoard::empty_cell;
        const uint8_t final_row = 5;
        const uint8_t row_count = 12;
        const std::string font = "grid_inverse";

        std::vector<char> cells_one{' ',' ',' ',' ',' ',' ',
                                    ' ',' ',' ',' ',' ',' ',
                                     x , x ,' ',' ', x , x,
                                     x , x ,' ',' ', x , x,
                                     x , x ,' ',' ', x , x,
                                     x , x ,' ',' ', x , x,
                                     x , x ,' ',' ', x , x,
                                     x , x ,' ',' ', x , x,
                                     x , x ,' ',' ', x , x,
                                     x , x ,' ',' ', x , x,
                                    ' ',' ',' ',' ',' ',' ',
                                    ' ',' ',' ',' ',' ',' '};
        std::vector<char> cells_two{' ',' ',' ',' ',' ',' ',' ',' ',
                                    ' ',' ',' ',' ',' ',' ',' ',' ',
                                     x ,' ',' ', x , x ,' ',' ', x,
                                     x ,' ',' ', x , x ,' ',' ', x,
                                     x ,' ',' ', x , x ,' ',' ', x,
                                     x ,' ',' ', x , x ,' ',' ', x,
                                     x ,' ',' ', x , x ,' ',' ', x,
                                     x ,' ',' ', x , x ,' ',' ', x,
                                     x ,' ',' ', x , x ,' ',' ', x,
                                     x ,' ',' ', x , x ,' ',' ', x,
                                    ' ',' ',' ',' ',' ',' ',' ',' ',
                                    ' ',' ',' ',' ',' ',' ',' ',' '};
        std::vector<char> cells_three{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                                      ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                       x ,' ',' ', x ,' ',' ', x ,' ',' ', x,
                                      ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                                      ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

        const uint8_t col_count_3 = WtBoard::col_count;
        const uint8_t col_count_2 = WtBoard::col_count-2;
        const uint8_t col_count_1 = WtBoard::col_count-4;

        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_3,
                                            cells_three,
                                            final_row, 0,
                                            WtTime::from_milliseconds(30), "countdown_32" );

        WtAnimatableGridContent three( final_row, 0, row_count, WtBoard::col_count,
                                       font, cells_three );
        WtGridAnimation::GridAnimationStep step_three( three, WtTime::from_milliseconds(300) );
        pause_animation.push_back( step_three );



        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_2,
                                            cells_two,
                                            final_row, 1,
                                            WtTime::from_milliseconds(30), "countdown_32" );

        WtAnimatableGridContent two( final_row, 1, row_count, WtBoard::col_count-2,
                                     font, cells_two );
        WtGridAnimation::GridAnimationStep step_two( two, WtTime::from_milliseconds(300), 0 );
        pause_animation.push_back( step_two );



        construct_running_letter_animation( pause_animation,
                                            row_count, col_count_1,
                                            cells_one,
                                            final_row, 2,
                                            WtTime::from_milliseconds(30), "countdown_1" );

        WtAnimatableGridContent one( final_row, 2, row_count, WtBoard::col_count-4,
                                     font, cells_one );
        WtGridAnimation::GridAnimationStep step_one( one, WtTime::from_milliseconds(300), 0 );

        pause_animation.push_back( step_one );
#endif
    }

};


#endif /* _WT_ANIMATION_BUILDER_H_ */
