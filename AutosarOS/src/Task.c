/**
* @file
*
* @date    2019-09-02
* @author  Pascal Romahn
*/

#include "Task.h"
#include "OS.h"

extern StatusType OS_ActivateTask(enum tasks_e TaskID)
{
    if (TaskID >= INVALID_TASK) {
        return E_OS_ID;
    }

    // TODO: Handle multiple activations
    if (TCB_Cfg[TaskID]->curState == SUSPENDED) {
        TCB_Cfg[TaskID]->curState = PRE_READY;
    } else if (TCB_Cfg[TaskID]->curState == WAITING) {
        TCB_Cfg[TaskID]->curState = READY;
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType OS_TerminateTask()
{
    if (isISR) { // TODO: Extended error check
        return E_OS_CALLLEVEL;
    }

    // TODO: Check for resources on extended error check
    TCB_Cfg[currentTask]->curState = SUSPENDED;
    currentTask = INVALID_TASK;

    OS_Schedule();

    return E_OK;
}

extern StatusType OS_GetTaskID(enum tasks_e* TaskID)
{
    *TaskID = currentTask;

    return E_OK;
}


extern StatusType OS_GetTaskState(enum tasks_e TaskID, OsTaskState* State)
{
    if (TaskID >= INVALID_TASK) {
        return E_OS_ID;
    }

    *State = TCB_Cfg[TaskID]->curState;

    /* Report PRE_READY as READY to conform to OSEK standard */
    if (*State == PRE_READY) {
        *State = READY;
    }

    return E_OK;
}