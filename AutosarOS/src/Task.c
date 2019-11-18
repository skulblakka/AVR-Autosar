/**
 * @file
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "Task.h"
#include "OS.h"
#include "assert.h"

extern StatusType OS_ActivateTask(enum tasks_e TaskID)
{
    if (TaskID >= INVALID_TASK) {
        return E_OS_ID;
    }

    if (TCB_Cfg[TaskID]->curNumberOfActivations + 1 > TCB_Cfg[TaskID]->numberOfActivations) {
        return E_OS_LIMIT;
    }

    TCB_Cfg[TaskID]->curNumberOfActivations += 1;

    if (TCB_Cfg[TaskID]->curState == SUSPENDED) {
        TCB_Cfg[TaskID]->curState = PRE_READY;
    } else if (TCB_Cfg[TaskID]->curState == WAITING) {
        TCB_Cfg[TaskID]->curState = READY;
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType OS_ChainTask(enum tasks_e TaskID)
{
    if (isISR) { // TODO: Extended error check
        return E_OS_CALLLEVEL;
    }

    if (TaskID >= INVALID_TASK) {
        return E_OS_ID;
    }

    /* Handle multiple activations of chained task */
    if (TCB_Cfg[TaskID]->curNumberOfActivations + 1 > TCB_Cfg[TaskID]->numberOfActivations && TaskID != currentTask) {
        return E_OS_LIMIT;
    }

    TCB_Cfg[TaskID]->curNumberOfActivations += 1;


    /* Handle multiple activations of current task */
    TCB_Cfg[currentTask]->curNumberOfActivations -= 1;
    assert(TCB_Cfg[currentTask]->curNumberOfActivations >= 0);
    assert(TCB_Cfg[currentTask]->curNumberOfActivations <= TCB_Cfg[currentTask]->numberOfActivations);
    if (TCB_Cfg[currentTask]->curNumberOfActivations > 0) {
        TCB_Cfg[currentTask]->curState = PRE_READY;
    } else {
        TCB_Cfg[currentTask]->curState = SUSPENDED;
    }

    currentTask = INVALID_TASK;

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

    TCB_Cfg[currentTask]->curNumberOfActivations -= 1;
    assert(TCB_Cfg[currentTask]->curNumberOfActivations >= 0);
    assert(TCB_Cfg[currentTask]->curNumberOfActivations <= TCB_Cfg[currentTask]->numberOfActivations);
    if (TCB_Cfg[currentTask]->curNumberOfActivations > 0) {
        TCB_Cfg[currentTask]->curState = PRE_READY;
    } else {
        TCB_Cfg[currentTask]->curState = SUSPENDED;
    }

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