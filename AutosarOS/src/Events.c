/**
 * @file
 *
 * @brief       Implementation of event management
 *
 * @date        2020-05-28
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "Events.h"
#include "Types.h"
#include "OS.h"
#include "OCB.h"
#include "Resource.h"

#include <util/atomic.h>

extern StatusType Events_SetEvent(TaskType TaskID, EventMaskType events)
{
    OS_SET_ERROR_INFO2(OSServiceId_SetEvent, &TaskID, sizeof(TaskID), &events, sizeof(events));

    bool taskChanged = false;

    if (OS_EXTENDED && TaskID >= INVALID_TASK) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && TCB_Cfg[TaskID]->taskType != EXTENDED) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ACCESS;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (OS_EXTENDED && TCB_Cfg[TaskID]->curState == SUSPENDED) {
            OS_CALL_ERROR_HOOK();

            return E_OS_STATE;
        }

        // Set pending events
        TCB_Cfg[TaskID]->pendingEvents |= (events & TCB_Cfg[TaskID]->events);

        if ((TCB_Cfg[TaskID]->waitEvents & TCB_Cfg[TaskID]->pendingEvents) != 0
                && TCB_Cfg[TaskID]->curState == WAITING) {
            // Task was waiting for one of the pending events => transfer to READY state
            TCB_Cfg[TaskID]->curState = READY;

            taskChanged = true;
        }
    }

    if (taskChanged) {
        // Rescheduling might be required because the task was transferred to the READY state
        OS_Schedule();
    }

    return E_OK;
}

extern StatusType Events_ClearEvent(EventMaskType events)
{
    OS_SET_ERROR_INFO1(OSServiceId_ClearEvent, &events, sizeof(events));

    if (OS_EXTENDED && isISR) {
        OS_CALL_ERROR_HOOK();

        return E_OS_CALLLEVEL;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (OS_EXTENDED && TCB_Cfg[currentTask]->taskType != EXTENDED) {
            OS_CALL_ERROR_HOOK();

            return E_OS_ACCESS;
        }

        // Clear specified events
        TCB_Cfg[currentTask]->pendingEvents &= ~(events & TCB_Cfg[currentTask]->events);
    }

    return E_OK;
}

extern StatusType Events_GetEvent(TaskType TaskID, EventMaskRefType events)
{
    OS_SET_ERROR_INFO2(OSServiceId_GetEvent, &TaskID, sizeof(TaskID), &events, sizeof(events));

    if (TaskID >= INVALID_TASK) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && events == NULL) {
        OS_CALL_ERROR_HOOK();

        return E_OS_PARAM_POINTER;
    }

    if (OS_EXTENDED && TCB_Cfg[TaskID]->taskType != EXTENDED) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ACCESS;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (OS_EXTENDED && TCB_Cfg[TaskID]->curState == SUSPENDED) {
            OS_CALL_ERROR_HOOK();

            return E_OS_STATE;
        }

        // Copy pending events
        *events = TCB_Cfg[TaskID]->pendingEvents;
    }

    return E_OK;
}

extern StatusType Events_WaitEvent(EventMaskType events)
{
    OS_SET_ERROR_INFO1(OSServiceId_WaitEvent, &events, sizeof(events));

    if (OS_EXTENDED && isISR) {
        OS_CALL_ERROR_HOOK();

        return E_OS_CALLLEVEL;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (OS_EXTENDED && TCB_Cfg[currentTask]->taskType != EXTENDED) {
            OS_CALL_ERROR_HOOK();

            return E_OS_ACCESS;
        }

        if (OS_EXTENDED && TCB_Cfg[currentTask]->resourceQueue != NULL) {
            OS_CALL_ERROR_HOOK();

            return E_OS_RESOURCE;
        }

        // Set wait mask
        TCB_Cfg[currentTask]->waitEvents = (events & TCB_Cfg[currentTask]->events);

        if ((TCB_Cfg[currentTask]->waitEvents & TCB_Cfg[currentTask]->pendingEvents) == 0) {
            /* None of the requested events is pending => transfer to WAITING state */
            TCB_Cfg[currentTask]->curState = WAITING;

            Resource_ReleaseInternalResource();

            forceScheduling = true;
            OS_Schedule();
        }
    }

    return E_OK;
}