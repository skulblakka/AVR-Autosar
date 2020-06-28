/**
 * @file
 *
 * @brief       Implementation of alarm management
 *
 * @date        2020-06-15
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "Alarm.h"
#include "OS_API.h"
#include "assert.h"

#include <util/atomic.h>
#include <string.h>

/************************************************************************/
/* STATIC FUNCTIONS                                                     */
/************************************************************************/
/**
 * @brief   Handle alarm expiration
 *
 * Handle an alarm expiration and perform the configured action. The alarm will be stopped
 * and restarted if configured as a cyclic alarm.
 *
 * @param   alarmID     Alarm to handle
 */
static void Alarm_handleAlarmExpiration(AlarmType alarmID);

/************************************************************************/
/* EXTERN FUNCTIONS                                                     */
/************************************************************************/
extern StatusType Alarm_GetAlarmBase(AlarmType alarmID, AlarmBaseRefType info)
{
    OS_SET_ERROR_INFO2(OSServiceId_GetAlarmBase, &alarmID, sizeof(alarmID), &info, sizeof(info));

    if (OS_EXTENDED && alarmID >= INVALID_ALARM) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && info == NULL) {
        OS_CALL_ERROR_HOOK();

        return E_OS_PARAM_POINTER;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        memcpy((void*) info, (void*) Alarm_Cfg[alarmID]->alarmBase, sizeof(AlarmBaseType));
    }

    return E_OK;
}

extern StatusType Alarm_GetAlarm(AlarmType alarmID, TickRefType tick)
{
    OS_SET_ERROR_INFO2(OSServiceId_GetAlarm, &alarmID, sizeof(alarmID), &tick, sizeof(tick));

    if (OS_EXTENDED && alarmID >= INVALID_ALARM) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    TickType currentTick = 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (!Alarm_Cfg[alarmID]->running) {
            OS_CALL_ERROR_HOOK();

            return E_OS_NOFUNC;
        }

        *tick = Alarm_Cfg[alarmID]->expiration;

        if (Alarm_Cfg[alarmID]->alarmBase == Counter_Cfg[SYSTEM_COUNTER]) {
            currentTick = sysTick;
        } else {
            currentTick = Alarm_Cfg[alarmID]->alarmBase->value;
        }
    }

    if (*tick >= currentTick) {
        *tick = *tick - currentTick;
    } else {
        *tick = Alarm_Cfg[alarmID]->alarmBase->maxallowedvalue - currentTick + *tick + 1;
    }

    return E_OK;
}

extern StatusType Alarm_SetRelAlarm(AlarmType alarmID, TickType increment, TickType cycle)
{
    OS_SET_ERROR_INFO3(OSServiceId_SetRelAlarm, &alarmID, sizeof(alarmID), &increment, sizeof(increment), &cycle,
            sizeof(cycle));

    if (OS_EXTENDED && alarmID >= INVALID_ALARM) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (increment == 0) {
        OS_CALL_ERROR_HOOK();

        return E_OS_VALUE;
    }

    TickType tick = 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Alarm_Cfg[alarmID]->alarmBase == Counter_Cfg[SYSTEM_COUNTER]) {
            tick = sysTick;
        } else {
            tick = Alarm_Cfg[alarmID]->alarmBase->value;
        }
    }

    tick += increment;

    if (tick > Alarm_Cfg[alarmID]->alarmBase->maxallowedvalue) {
        tick -= Alarm_Cfg[alarmID]->alarmBase->maxallowedvalue + 1;
    }

    return Alarm_SetAbsAlarm(alarmID, tick, cycle);
}

extern StatusType Alarm_SetAbsAlarm(AlarmType alarmID, TickType start, TickType cycle)
{
    OS_SET_ERROR_INFO3(OSServiceId_SetRelAlarm, &alarmID, sizeof(alarmID), &start, sizeof(start), &cycle, sizeof(cycle));

    if (OS_EXTENDED && alarmID >= INVALID_ALARM) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    if (OS_EXTENDED && start > Alarm_Cfg[alarmID]->alarmBase->maxallowedvalue) {
        // Requested increment outside of allowed range
        OS_CALL_ERROR_HOOK();

        return E_OS_VALUE;
    }

    if (OS_EXTENDED && (cycle != 0 && (cycle < Alarm_Cfg[alarmID]->alarmBase->mincycle
                            || cycle > Alarm_Cfg[alarmID]->alarmBase->maxallowedvalue))) {
        // Requested cycle outside of allowed range
        OS_CALL_ERROR_HOOK()

        return E_OS_VALUE;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Alarm_Cfg[alarmID]->running) {
            OS_CALL_ERROR_HOOK();

            return E_OS_STATE;
        }

        Alarm_Cfg[alarmID]->expiration = start;
        Alarm_Cfg[alarmID]->cycle = cycle;
        Alarm_Cfg[alarmID]->running = true;
    }

    return E_OK;
}

extern StatusType Alarm_CancelAlarm(AlarmType alarmID)
{
    OS_SET_ERROR_INFO1(OSServiceId_GetAlarm, &alarmID, sizeof(alarmID));

    if (OS_EXTENDED && alarmID >= INVALID_ALARM) {
        OS_CALL_ERROR_HOOK();

        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (!Alarm_Cfg[alarmID]->running) {
            OS_CALL_ERROR_HOOK();

            return E_OS_NOFUNC;
        }

        Alarm_Cfg[alarmID]->running = false;
    }

    return E_OK;
}

extern void Alarm_evaluateAlarm(CounterType counter)
{
    if (counter == SYSTEM_COUNTER) {
        return;
    }
    
    for (uint8_t i = 0; i < ALARM_COUNT; i++) {
        bool expired = false;

        if (Alarm_Cfg[i]->alarmBase != Counter_Cfg[counter]) {
            // Handle alarms for the changed counter only
            continue;
        }

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            expired = Alarm_Cfg[i]->running && (Alarm_Cfg[i]->alarmBase->value == Alarm_Cfg[i]->expiration);
        }

        if (expired) {
            Alarm_handleAlarmExpiration(i);
        }
    }
}

extern void Alarm_evaluateSysTickAlarm(void)
{
    for (uint8_t i = 0; i < ALARM_COUNT; i++) {
        bool expired = false;

        if (Alarm_Cfg[i]->alarmBase != Counter_Cfg[SYSTEM_COUNTER]) {
            // Handle SysTick based alarm only
            continue;
        }

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            expired = Alarm_Cfg[i]->running && (sysTick == Alarm_Cfg[i]->expiration);
        }

        if (expired) {
            Alarm_handleAlarmExpiration(i);
        }
    }
}

static void Alarm_handleAlarmExpiration(AlarmType alarmID)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        Alarm_Cfg[alarmID]->running = false;
    }

    /* Perform configured action */
    switch (Alarm_Cfg[alarmID]->actionType) {
    case ALARM_ACTION_TASK:
        Task_ActivateTask(Alarm_Cfg[alarmID]->action.task);
        break;
    case ALARM_ACTION_EVENT:
        Events_SetEvent(Alarm_Cfg[alarmID]->action.task, Alarm_Cfg[alarmID]->event);
        break;
    case ALARM_ACTION_CALLBACK:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            // Run callback with interrupts disabled
            Alarm_Cfg[alarmID]->action.callback();
        }
        break;
    case ALARM_ACTION_COUNTER:
        Counter_IncrementCounter(Alarm_Cfg[alarmID]->action.counter);
        break;
    default:
        // We should not reach this part.
        assert(false);
        break;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Alarm_Cfg[alarmID]->cycle != 0) {
            // Alarm is cyclic -> set it again as relative alarm
            Alarm_SetRelAlarm(alarmID, Alarm_Cfg[alarmID]->cycle, Alarm_Cfg[alarmID]->cycle);
        }
    }
}