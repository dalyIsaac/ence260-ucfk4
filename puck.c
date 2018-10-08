/**
 * @file puck.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the puck
 * @version 0.1
 * @date 2018-10-08
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "puck.h"
#include "navswitch.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>

/**
 * @brief Puck for this user in the game
 */
static Puck puck = {0};

/**
 * @brief Updates the puck in the board.
 * CAN ONLY BE USED AFTER board_init()
 */
void puck_update_display(void)
{
    // wipes the old puck from the face of the display
    for (uint8_t i = puck.old_bottom; i <= puck.old_top; i++)
    {
        board[PUCK_COL][i] = false;
    }

    // sets the new puck
    for (uint8_t i = puck.new_bottom; i <= puck.new_top; i++)
    {
        board[PUCK_COL][i] = true;
    }
}

/**
 * @brief Creates a puck, and adds it to the board.
 * CAN ONLY BE USED AFTER board_init()
 */
void puck_init(void)
{
    Puck new_puck = {.old_top = 0, .old_bottom = 0, .new_top = 4, .new_bottom = 2};
    puck = new_puck;
    puck_update_display();
}

/**
 * @brief Updates the value of a puck, following user input with the navswitch
 * @param change The change to the puck's position
 */
void puck_update_value(NavMovement change)
{
    if (puck.new_bottom + change >= 0 && puck.new_top + change < LEDMAT_ROWS_NUM)
    {
        Puck new_puck = {
            .old_bottom = puck.new_bottom,
            .old_top = puck.new_top,
            .new_bottom = puck.new_bottom + change,
            .new_top = puck.new_top + change};
        puck = new_puck;
        puck_update_display();
    }
}

/**
 * @brief Updates the puck's position based on the user's interaction with the navswitch
 */
void puck_task(void)
{
    navswitch_update();

    if (navswitch_push_event_p(NAVSWITCH_NORTH))
    {
        puck_update_value(left);
    }

    if (navswitch_push_event_p(NAVSWITCH_SOUTH))
    {
        puck_update_value(right);
    }
}

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

void ball_update_display(void)
{
    board[ball.old_column][ball.old_row] = false;
    board[ball.new_column][ball.new_row] = true;
}

/**
 * @brief Creates a ball, and adds it to the board.
 * CAN ONLY BE USED AFTER board_init()
 */
void ball_init(void)
{
    uint8_t starting_row = random_from_range(0, 6);
    uint8_t starting_direction = random_from_range(north_east, south_east);

    // arbitrary old numbers. They if the new (row, column) is (0, 0), they'll be overwritten automatically
    Ball new_ball = {
        .old_row = 0,
        .old_column = 0,
        .new_row = starting_row,
        .new_column = STARTING_COLUMN,
        .velocity = 1,
        .direction = starting_direction};
    ball = new_ball;
    ball_update_display();
}