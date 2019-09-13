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
    } else if (TCB_Cfg[TaskID]->curState != RUNNING) {
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
    OS_Schedule();

    return E_OK;
}