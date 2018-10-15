/**
 * @file game.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @author Divyean Sivarman (dsi3@uclive.ac.nz)
 * @brief Main module for the game
 * @version 0.3
 * @date 2018-10-16
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "game.h"

#include "ball.h"
#include "board.h"
#include "ir_uart.h"
#include "navswitch.h"
#include "pio.h"
#include "puck.h"
#include "system.h"
#include "task.h"

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

    negotiate_first_player();

    board_init();
    puck_init();
    if (have_ball) {
        ball_init();
    }

    task_schedule(tasks, ARRAY_SIZE(tasks));
}
