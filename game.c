/**
 * @file game.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Main module for the game
 * @version 1.0
 * @date 2018-10-17
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "ball.h"
#include "board.h"
#include "customtaskschedule.h"
#include "ir_uart.h"
#include "navswitch.h"
#include "pio.h"
#include "puck.h"
#include "system.h"
#include "task.h"

#define BOARD_DISPLAY_TASK_RATE 250
#define PUCK_TASK_RATE 100
#define BALL_TASK_RATE 100

/**
 * @brief Main function for the game
 * @return int
 */
int main(void)
{
    task_t tasks[] = {{.func = board_task, .period = TASK_RATE / BOARD_DISPLAY_TASK_RATE},
                      {.func = puck_task, .period = TASK_RATE / PUCK_TASK_RATE},
                      {.func = ball_task, .period = TASK_RATE / BALL_TASK_RATE}};

    system_init();
    navswitch_init();
    ir_uart_init();

    text_init();
    show_initial_text();

    while (1) {
        negotiate_first_player();

        board_init();
        puck_init();
        ball_init();

        custom_task_schedule(tasks, ARRAY_SIZE(tasks));

        notify();
    }
}
