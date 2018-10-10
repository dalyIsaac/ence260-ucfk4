/**
 * @file puck.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Contains definitions for the puck
 * @version 0.2
 * @date 2018-10-10
 *
 * @copyright Copyright (c) 2018
 *
 * @note The bottom of the board is the side closest to the USB port
 */
#include "puck.h"

#include "board.h"
#include "display.h"

/**
 * @brief Puck for this user in the game
 */
Puck puck;

/**
 * @brief Updates the puck in the board/display.
 * CAN ONLY BE USED AFTER board_init()
 */
void puck_update_display(void)
{
    // wipes the old puck from the face of the display
    for (int8_t row = puck.old_bottom; row <= puck.old_top; row++) {
        display_pixel_set(PUCK_COL, row, false);
    }

    // sets the new puck
    for (int8_t row = puck.new_bottom; row <= puck.new_top; row++) {
        display_pixel_set(PUCK_COL, row, true);
    }
}

/**
 * @brief Creates a puck, and adds it to the board.
 * CAN ONLY BE USED AFTER board_init()
 */
void puck_init(void)
{
    Puck new_puck = {.old_top = STARTING_OLD,
                     .old_bottom = STARTING_OLD,
                     .new_top = STARTING_TOP,
                     .new_bottom = STARTING_BOTTOM};
    puck = new_puck;
    puck_update_display();
}

/**
 * @brief Updates the value of a puck, following user input with the navswitch
 * @param change The change to the puck's position
 */
void puck_update_value(NavMovement change)
{
    if (puck.new_bottom + change >= BOTTOM_ROW && puck.new_top + change < LEDMAT_ROWS_NUM) {
        Puck new_puck = {.old_bottom = puck.new_bottom,
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
void puck_task(__unused__ void* data)
{
    navswitch_update();

    if (navswitch_push_event_p(NAVSWITCH_COMPASS_SOUTH)) {
        puck_update_value(PUCK_MOVE_SOUTH);
    }

    if (navswitch_push_event_p(NAVSWITCH_COMPASS_NORTH)) {
        puck_update_value(PUCK_MOVE_NORTH);
    }
}
