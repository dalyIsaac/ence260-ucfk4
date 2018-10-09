/**
 * @file ball.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the ball
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <stdlib.h>
#include <time.h>
#include "display.h"
#include "ball.h"
#include "puck.h"

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

void ball_update_value(void)
{
    uint8_t new_row = UNINITIALISED;
    uint8_t new_column = UNINITIALISED;
    Direction new_direction = ball.direction;

    // handle north*
    // handle south*
    new_row = ball.new_row; // remove later

    if (ball.direction == east)
    {
        new_column = ball.new_column + ball.velocity;
    }
    else if (ball.direction == west)
    {
        new_column = ball.new_column - ball.velocity;
    }

    // check if (new_row, new_column) is in the puck
    if (new_ball_is_in_puck(new_column, new_row))
    {
        new_direction *= -1;
        new_column = ball.new_column - ball.velocity;
    }
    // check if (new_row, new_column) means that the player loses
    // check if (new_row, new_column) needs to be transmitted to the other player

    ball.old_row = ball.new_row;
    ball.old_column = ball.new_column;
    ball.new_row = new_row;
    ball.new_column = new_column;
    ball.direction = new_direction;

    ball_update_display();
}

/**
 * @brief Creates a ball, and adds it to the board.
 * CAN ONLY BE USED AFTER board_init()
 */
void ball_init(void)
{
    // uint8_t starting_row = random_from_range(0, 6);
    // uint8_t starting_direction = random_from_range(north_east, south_east);
    uint8_t starting_row = 3;
    uint8_t starting_direction = east;

    // arbitrary old numbers. They if the new (row, column) is (0, 0), they'll be overwritten automatically
    Ball new_ball = {
        .old_row = 0,
        .old_column = 0,
        .new_row = starting_row,
        .new_column = STARTING_COLUMN,
        .velocity = INITIAL_VELOCITY,
        .direction = starting_direction};
    ball = new_ball;
    ball_update_display();
}

/**
 * @brief Updates the ball when it should
 * @param void 
 */
void ball_task(__unused__ void *data)
{
    static uint16_t time;
    uint8_t timer[2];

    timer[0] = (time / 10) % 10;
    timer[1] = time % 10;

    if (timer[0] == 9 && timer[1] == 9)
    {
        ball_update_value();
    }

    time++;
}