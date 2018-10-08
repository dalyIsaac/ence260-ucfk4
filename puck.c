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