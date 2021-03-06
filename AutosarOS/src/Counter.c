/**
 * @file
 *
 * @brief       Implementation of counter management
 *
 * @date        2020-06-15
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "Counter.h"
#include "ScheduleTables.h"
#include "OCB.h"
#include "OS_API.h"

#include <util/atomic.h>

extern StatusType Counter_IncrementCounter(CounterType counterID)
{
    OS_SET_ERROR_INFO1(OSServiceId_IncrementCounter, &counterID, sizeof(counterID));

    if (OS_EXTENDED && counterID >= INVALID_COUNTER) {
        OS_CALL_ERROR_HOOK(E_OS_ID);

        return E_OS_ID;
    }

    if (OS_EXTENDED && Counter_Cfg[counterID]->type == HARDWARE) {
        OS_CALL_ERROR_HOOK(E_OS_ID);

        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Counter_Cfg[counterID]->value + 1 > Counter_Cfg[counterID]->maxallowedvalue) {
            Counter_Cfg[counterID]->value = 0;
        } else {
            Counter_Cfg[counterID]->value += 1;
        }
    }

    Alarm_evaluateAlarm(counterID);
    ScheduleTable_handleTick(counterID);

    return E_OK;
}

extern StatusType Counter_GetCounterValue(CounterType counterID, TickRefType value)
{
    OS_SET_ERROR_INFO2(OSServiceId_GetCounterValue, &counterID, sizeof(counterID), &value, sizeof(value));

    if (OS_EXTENDED && counterID >= INVALID_COUNTER) {
        OS_CALL_ERROR_HOOK(E_OS_ID);

        return E_OS_ID;
    }

    if (OS_EXTENDED && value == NULL) {
        OS_CALL_ERROR_HOOK(E_OS_PARAM_POINTER);

        return E_OS_PARAM_POINTER;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (counterID == SYSTEM_COUNTER) {
            *value = sysTick;
        } else {
            *value = Counter_Cfg[counterID]->value;
        }
    }

    return E_OK;
}

extern StatusType Counter_GetElapsedValue(CounterType counterID, TickRefType value, TickRefType elapsedValue)
{
    OS_SET_ERROR_INFO3(OSServiceId_GetElapsedValue, &counterID, sizeof(counterID), &value, sizeof(value), &elapsedValue,
            sizeof(elapsedValue));

    if (OS_EXTENDED && counterID >= INVALID_COUNTER) {
        OS_CALL_ERROR_HOOK(E_OS_ID);

        return E_OS_ID;
    }

    if (OS_EXTENDED && (value == NULL || elapsedValue == NULL)) {
        OS_CALL_ERROR_HOOK(E_OS_PARAM_POINTER);

        return E_OS_PARAM_POINTER;
    }

    if (OS_EXTENDED && *value > Counter_Cfg[counterID]->maxallowedvalue) {
        OS_CALL_ERROR_HOOK(E_OS_VALUE);

        return E_OS_VALUE;
    }

    TickType currentValue = 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (counterID == SYSTEM_COUNTER) {
            currentValue = sysTick;
        } else {
            currentValue = Counter_Cfg[counterID]->value;
        }
    }

    if (currentValue >= *value) {
        *elapsedValue = currentValue - *value;
    } else {
        *elapsedValue = Counter_Cfg[counterID]->maxallowedvalue - *value + currentValue + 1;
    }

    *value = currentValue;

    return E_OK;
}