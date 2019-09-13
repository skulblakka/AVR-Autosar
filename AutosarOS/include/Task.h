/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */


#ifndef TASK_H_
#define TASK_H_

#include "Types.h"
#include "TaskTypes.h"
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
 * @param   TaskID  ID of the task to be activated
 *
 * @return  E_OK        No error \n
 *          E_OS_LIMIT  Too many activations of the task \n
 *          E_OS_ID     TaskID is invalid
 */
extern StatusType OS_ActivateTask(enum tasks_e TaskID);

/**
 * @brief   Terminate active task
 *
 * The calling task is transferred from the running state into the suspended state.
 *
 * @return  E_OS_RESOURCE   Task still occupies resources \n
 *          E_OS_CALLLEVEL  Call at interrupt level
 */
extern StatusType OS_TerminateTask();

#endif /* TASK_H_ */