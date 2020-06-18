/**
 * @file
 * 
 * @brief       Task management
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef TASK_H_
#define TASK_H_

#include "Types.h"
#include "OCB.h"

#include <stdint.h>

/**
 * @brief   Activate a task
 *
 * The task is transferred from the suspended state into the ready state.
 *
 * @note    ActivateTask will not immediately change the state of the task in case of multiple activation requests.
 *          If the task is not suspended, the activation will only be recorded and performed later.
 *
 * @param   TaskID      ID of the task to be activated
 *
 * @return  E_OK        No error \n
 *          E_OS_LIMIT  Too many activations of the task \n
 *          E_OS_ID     TaskID is invalid
 */
extern StatusType Task_ActivateTask(enum tasks_e TaskID);

/**
 * @brief   Chain task
 *
 * The current is transferred from the running state into the suspended state. The specified task will be transferred
 * into the ready state. The specified task may be identical to the current task.
 *
 * @param   TaskID          ID of the task to be chained
 *
 * @return  E_OK            No error \n
 *          E_OS_LIMIT      Too many activations of the task \n
 *          E_OS_ID         TaskID is invalid \n
 *          E_OS_RESOURCE   Task still occupies resources \n
 *          E_OS_CALLLEVEL  Call at interrupt level
 */
extern StatusType Task_ChainTask(enum tasks_e TaskID);

/**
 * @brief   Terminate active task
 *
 * The calling task is transferred from the running state into the suspended state.
 *
 * @return  E_OS_RESOURCE   Task still occupies resources \n
 *          E_OS_CALLLEVEL  Call at interrupt level
 */
extern StatusType Task_TerminateTask();

/**
 * @brief   Reschedule current task
 *
 * If a higher priority task is ready it will be executed. Otherwise the calling task is continued. This allows a
 * processor assignment to other tasks with lower or equal priority than the ceiling priority of the current task.
 *
 * This service has no influence on preemptive tasks with no internal resource.
 *
 * @return  E_OK            No error \n
 *          E_OS_RESOURCE   Task still occupies resources \n
 *          E_OS_CALLLEVEL  Call at interrupt level
 *
 */
extern StatusType Task_Schedule();

/**
 * @brief   Return the ID of the task currently running
 *
 * @param   TaskID  Reference of the task currently running. INVALID_TASK if no task is in running state.
 *
 * @return  E_OK    No error
 */
extern StatusType Task_GetTaskID(enum tasks_e* TaskID);

/**
 * @brief   Return the state of a task
 *
 * @param   TaskID  ID of the task to return the state for
 * @param   State   Reference of the specified tasks state
 *
 * @return  E_OK        No error \n
 *          E_OS_ID     TaskID is invalid
 */
extern StatusType Task_GetTaskState(enum tasks_e TaskID, OsTaskState* State);

#endif /* TASK_H_ */