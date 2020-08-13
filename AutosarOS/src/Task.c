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

#include <util/atomic.h>

extern StatusType Task_ActivateTask(TaskType TaskID)
{
    OS_SET_ERROR_INFO1(OSServiceId_ActivateTask, &TaskID, sizeof(TaskID));

    if (OS_EXTENDED && TaskID >= INVALID_TASK) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (TCB_Cfg[TaskID]->curNumberOfActivations + 1 > TCB_Cfg[TaskID]->numberOfActivations) {
            OS_CALL_ERROR_HOOK();

            return E_OS_LIMIT;
        }

        TCB_Cfg[TaskID]->curNumberOfActivations += 1;

        if (TCB_Cfg[TaskID]->curState == SUSPENDED) {
            TCB_Cfg[TaskID]->curState = PRE_READY;
        }
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType Task_ChainTask(TaskType TaskID)
{
    OS_SET_ERROR_INFO1(OSServiceId_ChainTask, &TaskID, sizeof(TaskID));

    if (OS_EXTENDED && isISR) {
        OS_CALL_ERROR_HOOK();

        return E_OS_CALLLEVEL;
    }

    if (OS_EXTENDED && TaskID >= INVALID_TASK) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (OS_EXTENDED && TCB_Cfg[currentTask]->resourceQueue != NULL) {
            OS_CALL_ERROR_HOOK();

            return E_OS_RESOURCE;
        }

        /* Handle multiple activations of chained task */
        if (TCB_Cfg[TaskID]->curNumberOfActivations + 1 > TCB_Cfg[TaskID]->numberOfActivations && TaskID != currentTask) {
            OS_CALL_ERROR_HOOK();

            return E_OS_LIMIT;
        }

        TCB_Cfg[TaskID]->curNumberOfActivations += 1;

        /* Handle multiple activations of current task */
        TCB_Cfg[currentTask]->curNumberOfActivations -= 1;
        assert(TCB_Cfg[currentTask]->curNumberOfActivations >= 0);
        assert(TCB_Cfg[currentTask]->curNumberOfActivations <= TCB_Cfg[currentTask]->numberOfActivations);

#if defined(OS_CONFIG_HOOK_PRE_TASK) && OS_CONFIG_HOOK_PRE_TASK == true
        PostTaskHook();
#endif

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
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType Task_TerminateTask(void)
{
    OS_SET_ERROR_INFO0(OSServiceId_TerminateTask);

    if (OS_EXTENDED && isISR) {
        OS_CALL_ERROR_HOOK();

        return E_OS_CALLLEVEL;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (OS_EXTENDED && TCB_Cfg[currentTask]->resourceQueue != NULL) {
            OS_CALL_ERROR_HOOK();

            return E_OS_RESOURCE;
        }

        TCB_Cfg[currentTask]->curNumberOfActivations -= 1;

        assert(TCB_Cfg[currentTask]->curNumberOfActivations >= 0);
        assert(TCB_Cfg[currentTask]->curNumberOfActivations <= TCB_Cfg[currentTask]->numberOfActivations);

#if defined(OS_CONFIG_HOOK_PRE_TASK) && OS_CONFIG_HOOK_PRE_TASK == true
        PostTaskHook();
#endif

        if (TCB_Cfg[currentTask]->curNumberOfActivations > 0) {
            TCB_Cfg[currentTask]->curState = PRE_READY;
        } else {
            TCB_Cfg[currentTask]->curState = SUSPENDED;
        }

        Resource_ReleaseInternalResource();

        currentTask = INVALID_TASK;
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType Task_Schedule(void)
{
    OS_SET_ERROR_INFO0(OSServiceId_Schedule);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (OS_EXTENDED && isISR) {
            OS_CALL_ERROR_HOOK();

            return E_OS_CALLLEVEL;
        }

        if (TCB_Cfg[currentTask]->taskSchedule == PREEMPTIVE) {
            return E_OK;
        }

        if (OS_EXTENDED && TCB_Cfg[currentTask]->resourceQueue != NULL) {
            OS_CALL_ERROR_HOOK();

            return E_OS_RESOURCE;
        }

        Resource_ReleaseInternalResource();

        forceScheduling = true;
    }

    OS_Schedule();

    return E_OK;
}

extern StatusType Task_GetTaskID(TaskRefType TaskID)
{
    OS_SET_ERROR_INFO1(OSServiceId_GetTaskID, &TaskID, sizeof(TaskID));

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        *TaskID = currentTask;
    }

    return E_OK;
}


extern StatusType Task_GetTaskState(TaskType TaskID, TaskStateRefType State)
{
    OS_SET_ERROR_INFO2(OSServiceId_GetTaskState, &TaskID, sizeof(TaskID), &State, sizeof(State));

    if (OS_EXTENDED && TaskID >= INVALID_TASK) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && State == NULL) {
        OS_CALL_ERROR_HOOK();

        return E_OS_PARAM_POINTER;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        *State = TCB_Cfg[TaskID]->curState;
    }

    /* Report PRE_READY as READY to conform to OSEK standard */
    if (*State == PRE_READY) {
        *State = READY;
    }

    return E_OK;
}

extern void Task_startup(void)
{
    if (TASK_COUNT > 0) {
        assert(TASK_COUNT <= UINT8_MAX);
        /* Start all tasks configured as autostart */
        for (uint8_t i = 0; i < TASK_COUNT; i++) {
            if (TCB_Cfg[i]->autostart == AUTOSTART && TCB_Cfg[i]->curState == SUSPENDED) {
                TCB_Cfg[i]->curState = PRE_READY;
                TCB_Cfg[i]->curNumberOfActivations += 1;
            }

#if defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 2
            // Set stack top marker
            *(TCB_Cfg[i]->stack - 1) = 0xBE;
#if OS_CONFIG_STACK_MONITORING >= 3
            // Set stack memory for stack monitoring
            memset(TCB_Cfg[i]->stack, 0xBE, TCB_Cfg[i]->stackSize);
#endif /* OS_CONFIG_STACK_MONITORING >= 3 */
#endif /* defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 2 */
        }
    }
}