/**
 * @file ball.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains definitions for the ball
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 * @note The bottom of the board is the side closest to the USB port. The south
 * side is the bottom side of the board.
 * @note Comments for non-static functions and variables are inside the
 * associated header file.
 * @note For information pertaining to the structure of the transmitted and
 * received data, see README.md
 */

#include "ball.h"

#include "board.h"
#include "display.h"
#include "ir_uart.h"
#include "puck.h"

bool have_ball = false;

bool lost_game = false;

bool continue_game = true;

/**
 * @brief The ball for the game. It only holds the ball's attributes when this
 * board has the ball.
 *
 */
static Ball ball;

/**
 * @brief Transmits to the other board that this board has lost the game. Also
 * tells the custom task scheduler to stop the execution of the game. This is
 * kept inside this module because it hijacks the existing receiving scheme.
 *
 */
static void lost_transmit(void)
{
    lost_game = true;
    continue_game = false;
    ir_uart_putc(I_HAVE_LOST);
}

/**
 * @brief Transmits the ball's current attributes to the other board.
 *
 */
static void ball_transmit(void)
{
    int8_t ball_values = (ball.new_row << NEW_ROW_SHIFT) |
                         ((ball.velocity - 1) << VELOCITY_SHIFT) |
                         ball.direction;
    ir_uart_putc(ball_values);
    have_ball = false;
}

/**
 * @brief Gets the direction of the ball from the received transmission.
 *
 * @param received_data The received transmission
 * @return uint8_t The received direction
 */
static int8_t get_direction(uint8_t received_data)
{
    int8_t direction = received_data;
    // wipes the unneeded bits
    for (int8_t i = DIRECTION_BIT_LENGTH; i < 8; i++) {
        direction &= WIPE_BIT(i);
    }
    return direction;
}

/**
 * @brief Gets the velocity of the ball from the received transmission.
 *
 * @param received_data The received transmission
 * @return uint8_t The received velocity
 */
static int8_t get_velocity(uint8_t received_data)
{
    int8_t velocity = received_data >> VELOCITY_SHIFT;
    // wipes the unneeded bits
    for (int8_t i = VELOCITY_BIT_LENGTH; i < 8; i++) {
        velocity &= WIPE_BIT(i);
    }
    // increase the velocity of the ball so that it resides within the 1 to 4
    // range
    velocity++;
    return velocity;
}

/**
 * @brief Gets the new_row of the ball from the received transmission, and
 * returns the correct row for the receiving board - the boards have different
 * orientations.
 *
 * @param received_data The received transmission
 * @return uint8_t The received new_row
 */
static int8_t get_new_row(uint8_t received_data)
{
    return LAST_ROW - (received_data >> NEW_ROW_SHIFT);
}

/**
 * @brief Checks to see if the game should continue.
 *
 * @param received_data The data received from the other board
 * @return true The other board has indicated that it has lost the game, thus
 * the game should not continue.
 * @return false The other board has transmitted information about the ball, and
 * thus the game can continue.
 */
static bool check_won(uint8_t received_data)
{
    if (received_data == I_HAVE_LOST) {
        continue_game = false;
        return true;
    }
    return false;
}

/**
 * @brief Applies the received ball values so that they're correct for this
 * board.
 *
 * @param new_row The received new_row
 * @param velocity The received velocity
 * @param direction The received direction
 *
 * @note The received direction is for the old board. Since this board has a
 * different orientation to the board which transmitted the direction, the
 * direction and the new_row need to be updated.
 */
static void set_received_ball_values(int8_t new_row, int8_t velocity,
                                     int8_t direction)
{
    ball.old_column = STARTING_OLD;
    ball.old_row = STARTING_OLD;
    ball.new_column = BALL_RECEIVED_START;
    ball.new_row = new_row;

    // figures out the direction and new_row for this board
    switch (direction) {
        case EAST:
            ball.direction = WEST;
            break;
        case SOUTH_EAST:
            ball.new_row--;
            ball.direction = NORTH_WEST;
            break;
        case NORTH_EAST:
            ball.new_row++;
            ball.direction = SOUTH_WEST;
            break;
        default:
            break;
    }

    ball.velocity = velocity;
}

/**
 * @brief Receives data from the other board. This is either data about the
 * ball's attributes, or that the other board has lost the game.
 *
 */
static void ball_receive(void)
{
    if (ir_uart_read_ready_p()) {
        // setting received_data to an unsigned integer avoids a nasty bug in
        // get_new_row(). To see the old get_new_row go to
        // https://eng-git.canterbury.ac.nz/ence260-2018/group436/blob/b97516ce8a9072bec1e871a162c68e9ba6eb2a64/ball.c#L100
        uint8_t received_data = ir_uart_getc();

        if (!check_won(received_data)) {
            int8_t new_row = get_new_row(received_data);
            int8_t velocity = get_velocity(received_data);
            int8_t direction = get_direction(received_data);
            set_received_ball_values(new_row, velocity, direction);
            have_ball = true;
        }
    }
}

/**
 * @brief Updates the ball in the board/display.
 */
static void ball_update_display(void)
{
    display_pixel_set(ball.old_column, ball.old_row, false);
    if (have_ball) {
        display_pixel_set(ball.new_column, ball.new_row, true);
    }
}

/**
 * @brief Checks if the board should transmit the ball's position. If so, it
 * calls ball_transmit.
 *
 */
static void handle_ball_transmission(void)
{
    if (have_ball && ball.new_column == TRANSMIT_COLUMN) {
        ball_transmit();
    }
}

/**
 * @brief Gets the impact point between the puck and the ball, when the ball is
 * travelling west Assumes that pucks are three points long.
 *
 * @return ImpactPoint The impact point
 */
static ImpactPoint get_impact_point(void)
{
    if (ball.direction == WEST) {
        if (ball.new_row == puck.new_bottom) {
            return IMPACT_BOTTOM;
        } else if (ball.new_row == puck.new_top) {
            return IMPACT_TOP;
        } else if (puck.new_bottom < ball.new_row &&
                   ball.new_row < puck.new_top) {
            return IMPACT_MIDDLE;
        }
    } else if (ball.direction == SOUTH_WEST || ball.direction == NORTH_WEST) {
        if (ball.old_row == puck.new_top) {
            return IMPACT_TOP;
        } else if (ball.old_row == puck.new_bottom) {
            return IMPACT_BOTTOM;
        } else if (puck.new_bottom <= ball.new_row &&
                   ball.new_row <= puck.new_top) {
            return IMPACT_MIDDLE;
        }
    }
    return NO_IMPACT;
}

/**
 * @brief Handles collisions where the ball's direction is WEST.
 *
 */
static void handle_ball_puck_collision_west(void)
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
        } else if (impact == NO_IMPACT) {
            ball.new_row = STARTING_ROW;
            ball.new_column = STARTING_COLUMN;
            ball.direction = WEST;
        }
    }
}

/**
 * @brief Handles collisions where the ball's direction is SOUTH_WEST.
 *
 */
static void handle_ball_puck_collision_south_west(void)
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
            // per the model, this particular collision increases the velocity
            // by 2
            ball.velocity += 2;
        } else if (impact == NO_IMPACT) {
            ball.new_row = ball.old_row - 1;
            ball.new_column = LAST_COLUMN;
            ball.direction = WEST;
        }
    }
}

/**
 * @brief Handles collisions where the ball's direction is SOUTH_WEST.
 *
 */
static void handle_ball_puck_collision_north_west(void)
{
    if (ball.new_column == PUCK_COL) {
        ImpactPoint impact = get_impact_point();
        ball.new_column = ball.old_column - 1;
        if (impact == IMPACT_TOP) {
            ball.direction = NORTH_EAST;
            // per the model, this particular collision increases the velocity
            // by 2
            ball.velocity += 2;
        } else if (impact == IMPACT_MIDDLE) {
            ball.direction = EAST;
            ball.new_row = ball.old_row;
        } else if (impact == IMPACT_BOTTOM) {
            ball.direction = NORTH_EAST;
            ball.velocity++;
        } else if (impact == NO_IMPACT) {
            ball.new_row = ball.old_row + 1;
            ball.new_column = LAST_COLUMN;
            ball.direction = WEST;
        }
    }
}

/**
 * @brief Checks to see if the proposed location for the ball is currently
 * occupied by the puck.
 *
 * @return true The ball is in the puck
 * @return false The ball is not in the puck
 */
static void handle_ball_puck_collision(void)
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
 *
 */
static void set_ball_column_movement(void)
{
    if (NORTH_EAST <= ball.direction && ball.direction <= SOUTH_EAST) {
        // based on the direction compass. This includes NE, E, SE
        ball.new_column--;
    } else if (SOUTH_WEST <= ball.direction && ball.direction <= NORTH_WEST) {
        // This includes NW, W, SW
        ball.new_column++;
    }
}

/**
 * @brief If the ball collides with the wall, its row and direction are updated.
 *
 */
static void handle_ball_wall_collision(void)
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
 * within the board.
 *
 */
static void ball_update_value(void)
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

    // The ball should never reside in the LAST_COLUMN after it has collided
    // with the puck. If the ball is in LAST_COLUMN at this point, the player
    // has lost this game.
    if (ball.new_column == LAST_COLUMN) {
        lost_transmit();
    } else {
        handle_ball_wall_collision();
        handle_ball_transmission();

        if (ball.velocity > MAX_VELOCITY) {
            ball.velocity = MAX_VELOCITY;
        }

        ball_update_display();
    }
}

/**
 * @brief The counter which is used to determine how often the ball's position
 * should update, based on the velocity.
 *
 */
static uint16_t counter;

/**
 * @brief Checks whether the time_to_check matches the current time, and thus
 * whether the ball can update.
 *
 * @param time_to_check Time to check. Should between 0 and 99, inclusive.
 * @return true The ball's value can be updated now.
 * @return false The ball's value cannot be updated now.
 */
static bool can_update(uint8_t time_to_check)
{
    uint8_t timer = GET_TIMER(counter);

    if (timer == time_to_check) {
        return true;
    }
    return false;
}

void ball_init(void)
{
    if (have_ball) {
        ball = (Ball){.old_row = STARTING_OLD,
                      .old_column = STARTING_OLD,
                      .new_row = STARTING_ROW,
                      .new_column = STARTING_COLUMN,
                      .velocity = STARTING_VELOCITY,
                      .direction = STARTING_DIRECTION};
        ball_update_display();
    } else {
        ball = (Ball){.old_row = STARTING_OLD,
                      .old_column = STARTING_OLD,
                      .new_row = BALL_RECEIVED_START,
                      .new_column = BALL_RECEIVED_START,
                      .velocity = MAX_VELOCITY,
                      .direction = STARTING_OLD};
    }
}

void ball_task(__unused__ void* data)
{
    uint8_t time_to_check = FIRST_VALUE_FOR_UPDATE;
    uint8_t variable_period = VARIABLE_PERIOD_NUMERATOR / ball.velocity;
    for (uint8_t i = 0; i < ball.velocity; i++) {
        if (can_update(time_to_check)) {
            if (!have_ball) {
                ball_receive();
            } else {
                ball_update_value();
            }
            counter++;
            return;
        }
        time_to_check -= variable_period;
    }
    counter++;
    return;
}
