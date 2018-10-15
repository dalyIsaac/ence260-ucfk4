/**
 * @file board.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Contains definitions for the board/display, and the initial text
 * @version 0.3
 * @date 2018-10-16
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "board.h"

#include "../fonts/font5x7_1.h"
#include "display.h"
#include "navswitch.h"
#include "pacer.h"
#include "pio.h"
#include "tinygl.h"

bool start_game = false;

/**
 * @brief Initialises tinygl and the pacer.
 */
void text_init(void)
{
    pacer_init(PACER_RATE);
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}

/**
 * @brief Displays the initial text for the game. It exits when the user pushes the navswitch.
 */
void show_initial_text(void)
{
    tinygl_text("PONG");
    while (!start_game) {
        pacer_wait();
        tinygl_update();

        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            start_game = true;
        }
    }
}

/**
 * @brief Initialises the display/board
 */
void board_init(void)
{
    display_init();
}

/**
 * @brief Displays the board
 */
void board_task(__unused__ void* data)
{
    display_update();
}