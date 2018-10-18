/**
 * @file game.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Main module for the game
 * @version 1.0
 * @date 2018-10-18
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "ball.h"
#include "board.h"
#include "customtaskschedule.h"
#include "ir_uart.h"
#include "navswitch.h"
#include "pacer.h"
#include "pio.h"
#include "puck.h"
#include "system.h"
#include "task.h"
#include "text.h"

/**
 * @brief The rate at which the board's display updates.
 *
 */
#define BOARD_DISPLAY_TASK_RATE 250

/**
 * @brief The rate at which the puck's task runs.
 *
 */
#define PUCK_TASK_RATE 100

/**
 * @brief The rate at which the ball's task runs.
 *
 */
#define BALL_TASK_RATE 100

/**
 * @brief Negotiates between the two boards who the first player is.
 *
 */
void negotiate_first_player(void)
{
    int8_t received_data = 0;

    pacer_wait();
    if (ir_uart_read_ready_p()) { // receives data first, so is player 2
        received_data = ir_uart_getc();
        if (received_data == I_AM_PLAYER_ONE) {
            ir_uart_putc(I_AM_PLAYER_TWO);
            have_ball = false;
        }
    } else {
        while (received_data !=
               I_AM_PLAYER_TWO) {          // sends data first, so is player 1
            ir_uart_putc(I_AM_PLAYER_ONE); // this board has "claimed" player 1
            pacer_wait();
            received_data = ir_uart_getc();
        }
        have_ball = true;
    }
}

/**
 * @brief Main function for the game.
 *
 * @return int
 */
int main(void)
{
    task_t tasks[] = {
        {.func = board_task, .period = TASK_RATE / BOARD_DISPLAY_TASK_RATE},
        {.func = puck_task, .period = TASK_RATE / PUCK_TASK_RATE},
        {.func = ball_task, .period = TASK_RATE / BALL_TASK_RATE}};

    system_init();
    navswitch_init();
    ir_uart_init();

    text_init();
    show_initial_text();

    // To exit the application, the user presses the reset button, which kills
    // the program by itself. Thus, an infinite loop is justified.
    while (1) {
        negotiate_first_player();

        board_init();
        puck_init();
        ball_init();

        custom_task_schedule(tasks, ARRAY_SIZE(tasks));

        notify();
    }
}
