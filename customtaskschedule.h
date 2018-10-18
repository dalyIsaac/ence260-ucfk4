/** @file   task.c
    @author M. P. Hayes, UCECE
    @date   17 August 2011
    @brief  Simple task scheduler.

    @note task_schedule was modified in order to allow the game to end,
   depending on the Boolean `continue_game`, which is defined in ball.h, rather
   than running in an infinite loop.

   We (Isaac Daly <idd17@uclive.ac.nz> and Divyean Sivarman <dsi3@uclive.ac.nz>)
   do not claim any ownership over this header file or the accompanying module.
*/

#ifndef CUSTOMTASKSCHEDULE_H
#define CUSTOMTASKSCHEDULE_H

#include "system.h"
#include "task.h"

/** Schedule tasks
    @param tasks pointer to array of tasks (the highest priority
                 task comes first)
    @param num_tasks number of tasks to schedule
    @return this never returns.
*/
void custom_task_schedule(task_t* tasks, uint8_t num_tasks);

#endif