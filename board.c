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
#include "ir_uart.h"
#include "navswitch.h"
#include "pacer.h"
#include "pio.h"
#include "tinygl.h"

void negotiate_first_player(void)
{
    int8_t received_data = 0;

    pacer_wait();
    if (ir_uart_read_ready_p()) {
        received_data = ir_uart_getc();
        if (received_data == 1) {
            while (1) { // remove later
                tinygl_text("1");
                tinygl_update();
                pacer_wait();
            }
        }
    } else {
        while (received_data != 2) {
            ir_uart_putc(1);
            pacer_wait();
            received_data = ir_uart_getc();
        }

        tinygl_text("2"); // remove later
        while (1) {       // remove later
            tinygl_update();
            pacer_wait();
        }
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