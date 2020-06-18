/**
 * @file
 *
 * @brief       Implementation of task management
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
#include "Resource.h"

extern StatusType Task_ActivateTask(enum tasks_e TaskID)
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
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType Task_ChainTask(enum tasks_e TaskID)
{
    if (isISR) { // TODO: Extended error check
        return E_OS_CALLLEVEL;
    }

    if (TaskID >= INVALID_TASK) {
        return E_OS_ID;
    }

    if (TCB_Cfg[currentTask]->resourceQueue != NULL) { // TODO: Extended error check
        return E_OS_RESOURCE;
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

    Resource_ReleaseInternalResource();

    currentTask = INVALID_TASK;

    if (TCB_Cfg[TaskID]->curState == SUSPENDED) {
        TCB_Cfg[TaskID]->curState = PRE_READY;
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType Task_TerminateTask()
{
    if (isISR) { // TODO: Extended error check
        return E_OS_CALLLEVEL;
    }

    if (TCB_Cfg[currentTask]->resourceQueue != NULL) { // TODO: Extended error check
        return E_OS_RESOURCE;
    }

    TCB_Cfg[currentTask]->curNumberOfActivations -= 1;
    assert(TCB_Cfg[currentTask]->curNumberOfActivations >= 0);
    assert(TCB_Cfg[currentTask]->curNumberOfActivations <= TCB_Cfg[currentTask]->numberOfActivations);
    if (TCB_Cfg[currentTask]->curNumberOfActivations > 0) {
        TCB_Cfg[currentTask]->curState = PRE_READY;
    } else {
        TCB_Cfg[currentTask]->curState = SUSPENDED;
    }

    Resource_ReleaseInternalResource();

    currentTask = INVALID_TASK;

    OS_Schedule();

    return E_OK;
}

extern StatusType Task_Schedule()
{
    if (TCB_Cfg[currentTask]->taskSchedule == PREEMPTIVE) {
        return E_OK;
    }

    if (isISR) { // TODO Extended error check
        return E_OS_CALLLEVEL;
    }

    if (TCB_Cfg[currentTask]->resourceQueue != NULL) { // TODO Extended error check
        return E_OS_RESOURCE;
    }

    Resource_ReleaseInternalResource();

    forceScheduling = true;
    OS_Schedule();

    return E_OK;
}

extern StatusType Task_GetTaskID(enum tasks_e* TaskID)
{
    *TaskID = currentTask;

    return E_OK;
}


extern StatusType Task_GetTaskState(enum tasks_e TaskID, OsTaskState* State)
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