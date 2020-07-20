/**
 * @file
 *
 * @brief       Implementation for schedule tables management
 *
 * @date        2020-06-25
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "ScheduleTables.h"
#include "Counter.h"
#include "Task.h"
#include "Events.h"

#include "OCB.h"
#include "OS.h"

#include "assert.h"

#include <util/atomic.h>

/************************************************************************/
/* STATIC FUNCTIONS                                                     */
/************************************************************************/
/**
 * @brief   Handle expiry point
 *
 * Handle an expiry point and perform the configured actions.
 *
 * @param   expiryPoint         Pointer to expiry point to handle
 */
static void ScheduleTable_handleExpiryPoint(const volatile struct scheduleTableExpiryPoint_s* expiryPoint)
{
    /* Handle task activations */
    for (uint8_t i = 0; i < expiryPoint->numTaskActions; i++) {
        Task_ActivateTask(expiryPoint->taskActionList[i].task);
    }

    /* Handle events */
    for (uint8_t i = 0; i < expiryPoint->numEventActions; i++) {
        Events_SetEvent(expiryPoint->eventActionList[i].task, expiryPoint->eventActionList[i].event);
    }
}

/**
 * @brief   Handle start of schedule table
 *
 * This handles the start of a schedule table. It will reset the current tick and set the
 * current state to #SCHEDULETABLE_RUNNING.
 *
 * The first expiry point will be handled if its offset equals zero. However scheduling
 * is blocked during this.
 *
 * @warning Interrupts must be disabled during this function.
 *
 * @param   scheduleTableID     Schedule table to be started
 */
static void ScheduleTable_handleScheduleTableStart(ScheduleTableType scheduleTableID)
{
    ScheduleTable_Cfg[scheduleTableID]->currentState = SCHEDULETABLE_RUNNING;
    ScheduleTable_Cfg[scheduleTableID]->currentTick = 0;

    /* Handle first expiry point if it has zero offset */
    if (ScheduleTable_Cfg[scheduleTableID]->expiryPointList[0].offset == 0) {
        bool savedBlock = blockScheduling;
        blockScheduling = true;
        ScheduleTable_handleExpiryPoint(&ScheduleTable_Cfg[scheduleTableID]->expiryPointList[0]);
        blockScheduling = savedBlock;
    }
}

/************************************************************************/
/* EXTERN FUNCTIONS                                                     */
/************************************************************************/
extern StatusType ScheduleTable_StartScheduleTableRel(ScheduleTableType scheduleTableID, TickType offset)
{
    OS_SET_ERROR_INFO2(OSServiceId_StartScheduleTableRel, &scheduleTableID, sizeof(scheduleTableID), &offset,
            sizeof(offset));

    if (OS_EXTENDED && scheduleTableID >= INVALID_SCHEDULETABLE) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && (offset == 0
                    || offset > Counter_Cfg[ScheduleTable_Cfg[scheduleTableID]->counter]->maxallowedvalue -
                    ScheduleTable_Cfg[scheduleTableID]->expiryPointList[0].offset)) {
        OS_CALL_ERROR_HOOK();

        return E_OS_VALUE;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (ScheduleTable_Cfg[scheduleTableID]->currentState != SCHEDULETABLE_STOPPED) {
            OS_CALL_ERROR_HOOK();

            return E_OS_STATE;
        }

        if (ScheduleTable_Cfg[scheduleTableID]->counter == SYSTEM_COUNTER) {
            needSysTickEval += 1;
        }

        ScheduleTable_Cfg[scheduleTableID]->currentTick = 0 - offset;
        ScheduleTable_Cfg[scheduleTableID]->currentState = SCHEDULETABLE_RUNNING;
    }

    return E_OK;
}

extern StatusType ScheduleTable_StartScheduleTableAbs(ScheduleTableType scheduleTableID, TickType start)
{
    OS_SET_ERROR_INFO2(OSServiceId_StartScheduleTableAbs, &scheduleTableID, sizeof(scheduleTableID), &start,
            sizeof(start));

    if (OS_EXTENDED && scheduleTableID >= INVALID_SCHEDULETABLE) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && start > Counter_Cfg[ScheduleTable_Cfg[scheduleTableID]->counter]->maxallowedvalue) {
        OS_CALL_ERROR_HOOK();

        return E_OS_VALUE;
    }

    TickType currentTick;
    Counter_GetCounterValue(ScheduleTable_Cfg[scheduleTableID]->counter, &currentTick);

    if (start > currentTick) {
        start -= currentTick;
    } else if (start < currentTick) {
        start -= Counter_Cfg[ScheduleTable_Cfg[scheduleTableID]->counter]->maxallowedvalue - currentTick;
    } else {
        start = Counter_Cfg[ScheduleTable_Cfg[scheduleTableID]->counter]->maxallowedvalue;
    }

    return ScheduleTable_StartScheduleTableRel(scheduleTableID, start);
}

extern StatusType ScheduleTable_StopScheduleTable(ScheduleTableType scheduleTableID)
{
    OS_SET_ERROR_INFO1(OSServiceId_StopScheduleTable, &scheduleTableID, sizeof(scheduleTableID));

    if (OS_EXTENDED && scheduleTableID >= INVALID_SCHEDULETABLE) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (ScheduleTable_Cfg[scheduleTableID]->currentState == SCHEDULETABLE_STOPPED) {
            OS_CALL_ERROR_HOOK();

            return E_OS_NOFUNC;
        }

        if (ScheduleTable_Cfg[scheduleTableID]->counter == SYSTEM_COUNTER) {
            needSysTickEval -= 1;
        }

        ScheduleTable_Cfg[scheduleTableID]->currentState = SCHEDULETABLE_STOPPED;
    }

    return E_OK;
}

extern StatusType ScheduleTable_NextScheduleTable(ScheduleTableType scheduleTableID_from,
        ScheduleTableType scheduleTableID_to)
{
    OS_SET_ERROR_INFO2(OSServiceId_NextScheduleTable, &scheduleTableID_from, sizeof(scheduleTableID_from),
            &scheduleTableID_to, sizeof(scheduleTableID_to));

    if (OS_EXTENDED && (scheduleTableID_from >= INVALID_SCHEDULETABLE || scheduleTableID_to >= INVALID_SCHEDULETABLE)) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED
            && (ScheduleTable_Cfg[scheduleTableID_from]->counter != ScheduleTable_Cfg[scheduleTableID_to]->counter)) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (ScheduleTable_Cfg[scheduleTableID_from]->currentState == SCHEDULETABLE_STOPPED
                || ScheduleTable_Cfg[scheduleTableID_from]->currentState == SCHEDULETABLE_NEXT) {
            OS_CALL_ERROR_HOOK();

            return E_OS_NOFUNC;
        }

        if (ScheduleTable_Cfg[scheduleTableID_to]->currentState != SCHEDULETABLE_STOPPED) {
            OS_CALL_ERROR_HOOK();

            return E_OS_STATE;
        }

        if (ScheduleTable_Cfg[scheduleTableID_from]->next != INVALID_SCHEDULETABLE) {
            ScheduleTable_Cfg[ScheduleTable_Cfg[scheduleTableID_from]->next]->currentState = SCHEDULETABLE_STOPPED;
        }

        ScheduleTable_Cfg[scheduleTableID_from]->next = scheduleTableID_to;
        ScheduleTable_Cfg[scheduleTableID_to]->currentState = SCHEDULETABLE_NEXT;
    }

    return E_OK;
}

extern StatusType ScheduleTable_GetScheduleTableStatus(ScheduleTableType scheduleTableID,
        ScheduleTableStatusRefType scheduleStatus)
{
    OS_SET_ERROR_INFO2(OSServiceId_GetScheduleTableStatus, &scheduleTableID, sizeof(scheduleTableID), &scheduleStatus,
            sizeof(scheduleStatus));

    if (OS_EXTENDED && scheduleTableID >= INVALID_SCHEDULETABLE) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && scheduleStatus == NULL) {
        OS_CALL_ERROR_HOOK();

        return E_OS_PARAM_POINTER;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        *scheduleStatus = ScheduleTable_Cfg[scheduleTableID]->currentState;
    }

    return E_OK;
}

extern void ScheduleTable_handleTick(CounterType counter)
{
    for (uint8_t i = 0; i < SCHEDULETABLE_COUNT; i++) {
        if (ScheduleTable_Cfg[i]->counter != counter) {
            // Only evaluate schedule table if the associated counter was changed
            continue;
        }

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            if (ScheduleTable_Cfg[i]->currentState != SCHEDULETABLE_RUNNING) {
                continue;
            }

            ScheduleTable_Cfg[i]->currentTick += 1;

            uint8_t expiryPoint;
            for (expiryPoint = 0; expiryPoint < ScheduleTable_Cfg[i]->numExpiryPoints; expiryPoint++) {
                if (ScheduleTable_Cfg[i]->expiryPointList[expiryPoint].offset == ScheduleTable_Cfg[i]->currentTick) {
                    ScheduleTable_handleExpiryPoint(&ScheduleTable_Cfg[i]->expiryPointList[expiryPoint]);

                    // Increment counter for end of table handling below
                    expiryPoint += 1;

                    // Only one expiry point per offset per schedule table allowed
                    break;
                } else if (ScheduleTable_Cfg[i]->expiryPointList[expiryPoint].offset
                        > ScheduleTable_Cfg[i]->currentTick) {
                    // Reached upcoming expiry points
                    break;
                }
            }

            /* Handle end of schedule table */
            if (expiryPoint == ScheduleTable_Cfg[i]->numExpiryPoints) {
                if (ScheduleTable_Cfg[i]->finalDelay == 0
                        || ScheduleTable_Cfg[i]->finalDelay == ScheduleTable_Cfg[i]->currentTick -
                        ScheduleTable_Cfg[i]->expiryPointList[expiryPoint - 1].offset) {
                    if (ScheduleTable_Cfg[i]->cyclic && ScheduleTable_Cfg[i]->next == INVALID_SCHEDULETABLE) {
                        // Schedule table is cyclic and no other schedule table has been queued => restart it
                        ScheduleTable_handleScheduleTableStart(i);
                    } else {
                        /* Stop schedule table */
                        ScheduleTable_Cfg[i]->currentState = SCHEDULETABLE_STOPPED;

                        if (ScheduleTable_Cfg[i]->counter == SYSTEM_COUNTER) {
                            needSysTickEval -= 1;
                        }

                        /* Check if another schedule table is queued */
                        if (ScheduleTable_Cfg[i]->next != INVALID_SCHEDULETABLE) {
                            if (i < ScheduleTable_Cfg[i]->next
                                    && ScheduleTable_Cfg[ScheduleTable_Cfg[i]->next]->counter == counter) {

                                /* Start queued schedule table (will be handled within this tick evaluation) */
                                if (ScheduleTable_Cfg[ScheduleTable_Cfg[i]->next]->counter == SYSTEM_COUNTER) {
                                    needSysTickEval += 1;
                                }

                                ScheduleTable_Cfg[ScheduleTable_Cfg[i]->next]->currentState = SCHEDULETABLE_RUNNING;
                                ScheduleTable_Cfg[ScheduleTable_Cfg[i]->next]->currentTick = -1;
                            } else {
                                /* Start queued schedule table and handle initial expiry point if necessary */
                                if (ScheduleTable_Cfg[ScheduleTable_Cfg[i]->next]->counter == SYSTEM_COUNTER) {
                                    needSysTickEval += 1;
                                }

                                ScheduleTable_handleScheduleTableStart(ScheduleTable_Cfg[i]->next);
                            }

                            // Reset queued schedule table field
                            ScheduleTable_Cfg[i]->next = INVALID_SCHEDULETABLE;
                        }
                    }
                }
            }
        }
    }
}

extern void ScheduleTable_handleSysTick(void)
{
    ScheduleTable_handleTick(SYSTEM_COUNTER);
}

extern void ScheduleTable_startup(void)
{
    if (SCHEDULETABLE_COUNT > 0) {
        assert(SCHEDULETABLE_COUNT <= UINT8_MAX);

        for (uint8_t i = 0; i < SCHEDULETABLE_COUNT; i++) {
            /* Start all schedule tables configured as autostart */
            if (ScheduleTable_Cfg[i]->autoStart == true) {
                if (ScheduleTable_Cfg[i]->counter == SYSTEM_COUNTER) {
                    needSysTickEval += 1;
                }

                ScheduleTable_handleScheduleTableStart(i);
            }
        }
    }
}