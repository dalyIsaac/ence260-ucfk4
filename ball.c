/**
 * @file ball.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the ball
 * @version 0.2
 * @date 2018-10-10
 *
 * @copyright Copyright (c) 2018
 *
 * @note The bottom of the board is the side closest to the USB port. The south
 * side is the bottom side of the board.
 */
#include "ball.h"

#include "board.h"
#include "display.h"
#include "puck.h"

/**
 * @brief Ball for the game
 *
 */
static Ball ball;

/**
 * @brief Updates the ball in the board/display.
 */
void ball_update_display(void)
{
    display_pixel_set(ball.old_column, ball.old_row, false);
    display_pixel_set(ball.new_column, ball.new_row, true);
}

/**
 * @brief Gets the impact point between the puck and the ball, when the ball is travelling west
 * Assumes that pucks are three points long.
 * @return ImpactPoint The impact point
 */
ImpactPoint get_impact_point(void)
{
    if (ball.direction == WEST) {
        if (ball.new_row == puck.new_bottom) {
            return IMPACT_BOTTOM;
        } else if (ball.new_row == puck.new_top) {
            return IMPACT_TOP;
        }
        return IMPACT_MIDDLE;
    } else if (ball.direction == SOUTH_WEST || ball.direction == NORTH_WEST) {
        if (ball.old_row == puck.new_top) {
            return IMPACT_TOP;
        } else if (ball.old_row == puck.new_bottom) {
            return IMPACT_BOTTOM;
        }
        return IMPACT_MIDDLE;
    }
    return IMPACT_MIDDLE; // this should never be reached
}

/**
 * @brief Handles collisions where the ball's direction is WEST
 */
void handle_ball_puck_collision_west(void)
{
    if (ball.new_column == PUCK_COL && puck.new_bottom <= ball.new_row &&
        ball.new_row <= puck.new_top) {
        ImpactPoint impact = get_impact_point();
        ball.new_column = ball.old_column - 1;
        if (impact == IMPACT_MIDDLE) {
            ball.direction = EAST;
        } else if (impact == IMPACT_TOP) {
            ball.direction = NORTH_EAST;
            ball.new_row++;
        } else if (impact == IMPACT_BOTTOM) {
            ball.direction = SOUTH_EAST;
            ball.new_row--;
        }
    }
}

/**
 * @brief Handles collisions where the ball's direction is SOUTH_WEST
 */
void handle_ball_puck_collision_south_west(void)
{
    if (ball.new_column == PUCK_COL) {
        ImpactPoint impact = get_impact_point();
        ball.new_column = ball.old_column - 1;
        if (impact == IMPACT_TOP) {
            ball.direction = SOUTH_EAST;
            ball.velocity++;
        } else if (impact == IMPACT_MIDDLE) {
            ball.direction = EAST;
            ball.new_row = ball.old_row;
        } else if (impact == IMPACT_BOTTOM) {
            ball.direction = SOUTH_EAST;
            ball.velocity += 2;
        }
    }
}

/**
 * @brief Handles collisions where the ball's direction is SOUTH_WEST
 */
void handle_ball_puck_collision_north_west(void)
{
    if (ball.new_column == PUCK_COL) {
        ImpactPoint impact = get_impact_point();
        ball.new_column = ball.old_column - 1;
        if (impact == IMPACT_TOP) {
            ball.direction = NORTH_EAST;
            ball.velocity += 2;
        } else if (impact == IMPACT_MIDDLE) {
            ball.direction = EAST;
            ball.new_row = ball.old_row;
        } else if (impact == IMPACT_BOTTOM) {
            ball.direction = NORTH_EAST;
            ball.velocity++;
        }
    }
}

/**
 * @brief Checks to see if the proposed location for the ball is currently
 * occupied by the puck
 * @return true The ball is in the puck
 * @return false The ball is not in the puck
 */
void handle_ball_puck_collision(void)
{
    if (ball.direction == WEST) {
        handle_ball_puck_collision_west();
    } else if (ball.direction == SOUTH_WEST) {
        handle_ball_puck_collision_south_west();
    } else if (ball.direction == NORTH_WEST) {
        handle_ball_puck_collision_north_west();
    }
}

/**
 * @brief Updates the ball with the new column.
 */
void set_ball_column_movement(void)
{
    if (ball.direction >= NORTH_EAST) { // based on the direction compass. This includes NE, E, SE
        ball.new_column--;
    } else if (ball.direction <= NORTH_WEST) { // This includes NW, W, SW
        ball.new_column++;
    }
}

/**
 * @brief If the ball collides with the wall, its row and direction are updated.
 */
void handle_ball_wall_collision(void)
{
    if (ball.new_row < BOTTOM_ROW) {
        ball.new_row = BOTTOM_ROW + 1;
        if (ball.direction == SOUTH_WEST) {
            ball.direction = NORTH_WEST;
        } else if (ball.direction == SOUTH_EAST) {
            ball.direction = NORTH_EAST;
        }
    } else if (ball.new_row > TOP_ROW) {
        ball.new_row = TOP_ROW - 1;
        if (ball.direction == NORTH_WEST) {
            ball.direction = SOUTH_WEST;
        } else if (ball.direction == NORTH_EAST) {
            ball.direction = SOUTH_EAST;
        }
    }
}

/**
 * @brief Updates the ball's location, based on its attributes and location
 * within the board
 */
void ball_update_value(void)
{
    ball.old_column = ball.new_column;
    ball.old_row = ball.new_row;

    set_ball_column_movement();

    if (ball.direction == NORTH_WEST || ball.direction == NORTH_EAST) {
        ball.new_row++;
    } else if (ball.direction == SOUTH_WEST || ball.direction == SOUTH_EAST) {
        ball.new_row--;
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
    Ball new_ball = {.old_row = STARTING_OLD,
                     .old_column = STARTING_OLD,
                     .new_row = STARTING_ROW,
                     .new_column = STARTING_COLUMN,
                     .velocity = STARTING_VELOCITY,
                     .direction = STARTING_DIRECTION};
    ball = new_ball;

    ball_update_display();
}

/**
 * @brief Counter which is used for the timer.
 */
static uint16_t counter;

/**
 * @brief Checks whether the time_to_check matches the current time, and thus whether the ball
 * can update.
 * @param time_to_check Time to check. Should between 0 and 99, inclusive.
 * @return true
 * @return false
 */
bool can_update(uint8_t time_to_check)
{
    uint8_t timer = GET_TIMER(counter);

    if (timer == time_to_check) {
        return true;
    }
    return false;
}

/**
 * @brief Updates the ball when it should.
 * If the velocity is 1, it updates when value is 99.
 * If the velocity is 2, it updates when value is 99, 49.
 * If the velocity is 3, it updates when value is 99, 66, 33.
 * ...
 * @param void
 */
void ball_task(__unused__ void* data)
{
    uint8_t time_to_check = FIRST_VALUE_FOR_UPDATE;
    uint8_t variable_period = VARIABLE_PERIOD_NUMERATOR / ball.velocity;
    for (uint8_t i = 0; i < ball.velocity; i++) {
        if (can_update(time_to_check)) {
            ball_update_value();
            counter++;
            return;
        }
        time_to_check -= variable_period;
    }
    counter++;
    return;
}