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
 * @note Comments for non-static functions and variables are inside the associated header file.
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
 * @brief The ball for the game. It only holds the ball's attributes when this board has the ball.
 *
 */
static Ball ball;

/**
 * @brief Transmits to the other board that this board has lost the game. Also tells the custom task
 * scheduler to stop the execution of the game.
 *
 */
static void transmit_lost(void)
{
    lost_game = true;
    continue_game = false;
    ir_uart_putc(I_HAVE_LOST);
}

/**
 * @brief Transmits the ball's current attributes to the other board.
 *
 */
static void transmit_ball(void)
{
    // subtract a value from ball.velocity to ensure that it can fit inside 8 bits
    int8_t received_data = (ball.new_row << 5) | ((ball.velocity - 1) << 3) |
                           ball.direction; // bit shifting for transmitting;
    ir_uart_putc(received_data);
    have_ball = false;
}

/**
 * @brief Gets the direction of the ball from the received transmission.
 *
 * @param received_data The received transmission
 * @return int8_t The received direction
 */
static int8_t get_direction(int8_t received_data)
{
    int8_t direction = received_data;
    // using bit shifting for last value of the trasmited integer
    for (int8_t i = 3; i < 8; i++) {
        direction &= ~(1 << i);
    }
    return direction;
}

/**
 * @brief Gets the velocity of the ball from the received transmission.
 *
 * @param received_data The received transmission
 * @return int8_t The received velocity
 */
static int8_t get_velocity(int8_t received_data)
{
    int8_t velocity = received_data >> 3;
    // using bit shifting for middle value of the trasmited integer
    for (int8_t i = 2; i < 8; i++) {
        velocity &= ~(1 << i); // wipes the bits
    }
    velocity += 1;
    return velocity;
}

/**
 * @brief Gets the new_row of the ball from the received transmission. Currently does some funky
 * things to handle a mix-up between signed and unsigned integers.
 *
 * @param received_data The received transmission
 * @return int8_t The received new_row
 */
static int8_t get_new_row(int8_t received_data)
{
    // using bit shifting for first three bit of the trasmited integer
    int8_t new_row = (received_data >> 5);
    // handles a mix-up between signed and unsigned integers.
    // the code below is to make sure the right row get passed correctly.
    switch (new_row) {
        case 0:
            return 6;
            break;
        case 1:
            return 5;
            break;
        case 2:
            return 4;
            break;
        case 3:
            return 3;
            break;
        case -4:
            return 2;
            break;
        case -3:
            return 1;
            break;
        case -2:
            return 0;
            break;
    }
    return new_row;
}

/**
 * @brief Checks to see if the game should continue.
 *
 * @param received_data The data received from the other board
 * @return true The other board has indicated that it has lost the game, thus the game should not
 * continue.
 * @return false The other board has transmitted information about the ball.
 */
static bool check_won(int8_t received_data)
{
    if (received_data == I_HAVE_LOST) {
        continue_game = false;
    }
    return continue_game;
}

/**
 * @brief Applies the received ball values so that they're correct for this board.
 *
 * @param new_row The received new_row
 * @param velocity The received velocity
 * @param direction The received direction
 *
 * @note The received direction is for the old board. Since this board has a different orientation
 * to the board which transmitted the direction, the direction and the new_row need to be updated.
 */
static void set_received_ball_values(int8_t new_row, int8_t velocity, int8_t direction)
{
    ball.old_column = STARTING_OLD;
    ball.old_row = STARTING_OLD;
    ball.new_column = BALL_RECEIVED_START;

    // figures out the direction and new_row for this board
    switch (direction) {
        case EAST:
            ball.new_row = new_row;
            ball.direction = WEST;
            break;
        case SOUTH_EAST:
            ball.new_row = new_row - 1;
            ball.direction = NORTH_WEST;
            break;
        case NORTH_EAST:
            ball.new_row = new_row + 1;
            ball.direction = SOUTH_WEST;
            break;
        default:
            break;
    }

    ball.velocity = velocity;
}

/**
 * @brief Receives data from the other board. This is either data about the ball's attributes, or
 * that the other board has lost the game.
 *
 */
static void ball_receive(void)
{
    if (ir_uart_read_ready_p()) {
        int8_t received_data = ir_uart_getc();
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
 * @brief Checks if the board should transmit the ball's position. If so, it calls transmit_ball.
 *
 */
static void handle_ball_transmission(void)
{
    if (have_ball && ball.new_column == -1) {
        transmit_ball();
    }
}

/**
 * @brief Gets the impact point between the puck and the ball, when the ball is travelling west
 * Assumes that pucks are three points long.
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
        } else if (puck.new_bottom < ball.new_row && ball.new_row < puck.new_top) {
            return IMPACT_MIDDLE;
        }
    } else if (ball.direction == SOUTH_WEST || ball.direction == NORTH_WEST) {
        if (ball.old_row == puck.new_top) {
            return IMPACT_TOP;
        } else if (ball.old_row == puck.new_bottom) {
            return IMPACT_BOTTOM;
        } else if (puck.new_bottom <= ball.new_row && ball.new_row <= puck.new_top) {
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
            // per the model, this particular collision increases the velocity by 2
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
            // per the model, this particular collision increases the velocity by 2
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

    // The ball should never reside in the LAST_COLUMN after it has collided with the puck.
    // If the ball is in LAST_COLUMN at this point, the player has lost this game.
    if (ball.new_column == LAST_COLUMN) {
        transmit_lost();
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
 * @brief The counter which is used to determine how often the ball's position should update, based
 * on the velocity.
 *
 */
static uint16_t counter;

/**
 * @brief Checks whether the time_to_check matches the current time, and thus whether the ball
 * can update.
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
                      .velocity = 4,
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
