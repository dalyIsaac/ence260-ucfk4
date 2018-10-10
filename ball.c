/**
 * @file ball.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the ball
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 * @note The bottom of the board is the side closest to the USB port. The south side is the bottom side of the board. 
 */
#include <stdlib.h>
#include <time.h>
#include "display.h"
#include "ball.h"
#include "puck.h"
#include "tinygl.h"
#include "string.h"

/**
 * @brief Ball for the game
 * 
 */
static Ball ball = {0};

/**
 * @brief Returns a random value from the specified range.
 * This implementation is limited to values from 0 to 255, given that it uses uint8_t.
 * Based on https://stackoverflow.com/a/27712884/5018082
 * @param lower_limit Lower limit, inclusive. 0 <= lower_limit <= 255
 * @param upper_limit Upper limit, inclusive. 0 <= lower_limit <= 255
 * @return uint8_t Generated random value
 */
uint8_t random_from_range(uint8_t lower_limit, uint8_t upper_limit)
{
    srand(time(NULL)); // ensures that the seed is different each time
    float value = ((float)lower_limit + ((float)upper_limit) * rand()) / RAND_MAX;
    return (uint8_t)value;
}

/**
 * @brief Updates the ball in the board/display.
 */
void ball_update_display(void)
{
    display_pixel_set(ball.old_column, ball.old_row, false);
    display_pixel_set(ball.new_column, ball.new_row, true);
}

/**
 * @brief Gets the impact point between the puck and the ball.
 * Assumes that pucks are three points long.
 * @return ImpactPoint The impact point
 */
ImpactPoint get_impact_point(void)
{
    if (ball.new_row == puck.new_bottom)
    {
        return bottom;
    }
    else if (ball.new_row == puck.new_top)
    {
        return top;
    }
    return middle;
}

/**
 * @brief Handles collisions where the ball's direction is west
 */
void handle_ball_puck_collision_west(void)
{
    if (ball.new_column == PUCK_COL && puck.new_bottom <= ball.new_row && ball.new_row <= puck.new_top)
    {
        ImpactPoint impact = get_impact_point();
        ball.new_column = 2;

        if (impact == middle)
        {
            ball.direction = east;
        }
        else if (impact == top)
        {
            ball.direction = north_east;
            ball.new_row += 1;
        }
        else if (impact == bottom)
        {
            ball.direction = south_east;
            ball.new_row -= 1;
        }
    }
}

/**
 * @brief Hanles collisions where the ball's direction is south_west
 * 
 */
void handle_ball_puck_collision_south_west(void)
{
    if (ball.new_column == PUCK_COL)
    {
        ball.new_column = 2;
        if (ball.old_row == puck.new_top)
        {
            ball.direction = south_east;
            ball.velocity += 1;
        }
        else if (ball.old_row == puck.new_top - 1) // middle
        {
            ball.direction = east;
            ball.new_row = ball.old_row;
        }
        else if (ball.old_row == puck.new_bottom)
        {
            ball.direction = south_east;
            ball.velocity += 2;
        }
    }
}

void handle_ball_puck_collision_south_east(void)
{
}

/**
 * @brief Checks to see if the proposed location for the ball is currently occupied by
 * the puck
 * @return true The ball is in the puck
 * @return false The ball is not in the puck
 */
void handle_ball_puck_collision(void)
{
    if (ball.direction == west)
    {
        handle_ball_puck_collision_west();
    }
    else if (ball.direction == south_west)
    {
        handle_ball_puck_collision_south_west();
    }
    else if (ball.direction == south_east)
    {
        handle_ball_puck_collision_south_east();
    }
}

/**
 * @brief Updates the ball with the new column.
 */
void set_ball_column_movement(void)
{
    if (ball.direction >= north_east) // based on the direction compass. This includes NE, E, SE
    {
        ball.new_column -= 1;
    }
    else if (ball.direction <= north_west) // based on the direction compass. This includes NW, W, SW
    {
        ball.new_column += 1;
    }
}

/**
 * @brief If the ball collides with the wall, its row and direction are updated.
 */
void handle_ball_wall_collision(void)
{
    if (ball.new_row < 0) // impacts the bottom row
    {
        ball.new_row = 1;
        if (ball.direction == south_west)
        {
            ball.direction = north_west;
        }
        else if (ball.direction == south_east)
        {
            ball.direction = north_east;
        }
    }
    else if (ball.new_row > 6) // impacts the top row
    {
        ball.new_row = 5;
        if (ball.direction == north_west)
        {
            ball.direction = south_west;
        }
        else if (ball.direction == north_east)
        {
            ball.direction = south_east;
        }
    }
}

/**
 * @brief Updates the ball's location, based on its attributes and location within the board
 */
void ball_update_value(void)
{
    ball.old_column = ball.new_column;
    ball.old_row = ball.new_row;

    set_ball_column_movement();

    if (ball.direction == north_west || ball.direction == north_east)
    {
        ball.new_row += 1;
    }
    else if (ball.direction == south_west || ball.direction == south_east)
    {
        ball.new_row -= 1;
    }

    handle_ball_puck_collision();
    handle_ball_wall_collision();

    ball_update_display();
}

/**
 * @brief Creates a ball, and adds it to the board.
 * CAN ONLY BE USED AFTER board_init()
 */
void ball_init(void)
{
    // arbitrary old numbers. They if the new (row, column) is (0, 0), they'll be overwritten automatically
    Ball new_ball = {
        .old_row = 0,
        .old_column = 0,
        .new_row = 5,
        .new_column = 2,
        .velocity = 1,
        .direction = south_west};
    ball = new_ball;

    ball_update_display();
}

/**
 * @brief Counter which is used for the timer. 
 */
static uint16_t counter;

// time should be between 0 and 99, inclusive
bool check_timer(uint8_t time_to_check)
{
    uint8_t timer = ((counter / 10) % 10) * 10 + counter % 10;

    if (timer == time_to_check)
    {
        return true;
    }
    return false;
}

/**
 * @brief Updates the ball when it should
 * @param void 
 */
void ball_task(__unused__ void *data)
{
    uint8_t value = 99;
    uint8_t subtract_value = 100 / ball.velocity;
    for (uint8_t i = 0; i < ball.velocity; i++)
    {
        if (check_timer(value))
        {
            ball_update_value();
            counter++;
            return;
        }

        value -= subtract_value;
        // char text[3];
        // itoa(ball.direction, text, 10);
        // tinygl_clear();
        // tinygl_text(text);
        // tinygl_update();
    }
    counter++;
    return;
}