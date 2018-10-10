/**
 * @file game.c
 * @author Isaac Daly (idd17@uclive.ac.nz)
 * @brief Main module for the game
 * @version 0.2
 * @date 2018-10-10
 *
 * @copyright Copyright (c) 2018
 *
 */

#include "game.h"

#include "../fonts/font3x5_1.h"
#include "ball.h"
#include "board.h"
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
    board_init();
    puck_init();
    ball_init();

    task_schedule(tasks, ARRAY_SIZE(tasks));
}
