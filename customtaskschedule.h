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