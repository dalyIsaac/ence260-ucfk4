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
#include "ball.h"
#include "display.h"
#include "ir_uart.h"
#include "navswitch.h"
#include "pacer.h"
#include "pio.h"
#include "tinygl.h"

/**
 * @brief Negotiates who the first player is.
 */
void negotiate_first_player(void)
{
    int8_t received_data = 0;

    pacer_wait();
    if (ir_uart_read_ready_p()) { // receives data first, so is player 2
        received_data = ir_uart_getc();
        if (received_data == 2) {
            ir_uart_putc(1);
            have_ball = false;
        }
    } else {
        while (received_data != 1) { // sends data first, so is player 1
            ir_uart_putc(2);
            pacer_wait();
            received_data = ir_uart_getc();
        }
        have_ball = true;
    }
}

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
    bool start_game = false;
    tinygl_text("PRESS THE NAVSWITCH ONCE THE BOARDS ARE ALIGNED TO PLAY.");
    while (!start_game) {
        pacer_wait();
        tinygl_update();

        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            start_game = true;
        }
    }
}

void notify(void)
{
    if (lost_game) {
        tinygl_text("LOST. PRESS NAVSWITCH DOWN TO PLAY AGAIN. PRESS RESET BUTTON TO END GAME.");
    } else {
        tinygl_text("WON. PRESS NAVSWITCH DOWN TO PLAY AGAIN. PRESS RESET BUTTON TO END GAME.");
    }
    while (1) {
        pacer_wait();
        tinygl_update();

        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            break;
        }
    }
    display_init();
}

/**
 * @brief Initialises the display/board
 */
void board_init(void)
{
    lost_game = false;
    continue_game = true;
    display_init();
}

/**
 * @brief Displays the board
 */
void board_task(__unused__ void* data)
{
    display_update();
}