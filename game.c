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

#include "game.h"

#include "ball.h"
#include "board.h"
#include "ir_uart.h"
#include "navswitch.h"
#include "pio.h"
#include "puck.h"
#include "system.h"
#include "task.h"

/** With 16-bit times the maximum value is 32768.  */
#define TASK_OVERRUN_MAX 32767

/** Schedule tasks
    @param tasks pointer to array of tasks (the highest priority
                 task comes first)
    @param num_tasks number of tasks to schedule
    @return this never returns.
*/
void custom_task_schedule(task_t* tasks, uint8_t num_tasks)
{
    uint8_t i;
    timer_tick_t now;
    task_t* next_task;

    timer_init();
    now = timer_get();

    /* Start by scheduling the first task.  */
    next_task = tasks;

    while (continue_game) {
        timer_tick_t sleep_min;

        /* Wait until the next task is ready to run.  */
        timer_wait_until(next_task->reschedule);

        /* Schedule the task.  */
        next_task->func(next_task->data);

        /* Update the reschedule time.  */
        next_task->reschedule += next_task->period;

        sleep_min = ~0;
        now = timer_get();

        /* Search array of tasks.  Schedule the first task (highest priority)
           that needs to run otherwise wait until first task ready.  */
        for (i = 0; i < num_tasks; i++) {
            task_t* task = tasks + i;
            timer_tick_t overrun;

            overrun = now - task->reschedule;
            if (overrun < TASK_OVERRUN_MAX) {
                /* Have found a task that can run immediately.  */
                next_task = task;
                break;
            } else {
                timer_tick_t sleep;

                sleep = -overrun;
                if (sleep < sleep_min) {
                    sleep_min = sleep;
                    next_task = task;
                }
            }
        }
    }
}

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
