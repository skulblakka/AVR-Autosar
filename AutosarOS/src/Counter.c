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
#include "OCB.h"

#include <util/atomic.h>

extern StatusType Counter_IncrementCounter(CounterType counterID)
{
    if (counterID >= INVALID_COUNTER) {
        return E_OS_ID;
    }

    if (Counter_Cfg[counterID]->type == HARDWARE) {
        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Counter_Cfg[counterID]->value + 1 > Counter_Cfg[counterID]->maxallowedvalue) {
            Counter_Cfg[counterID]->value = 0;
        } else {
            Counter_Cfg[counterID]->value += 1;
        }
    }

    return E_OK;
}

extern StatusType Counter_GetCounterValue(CounterType counterID, TickRefType value)
{
    if (counterID >= INVALID_COUNTER) {
        return E_OS_ID;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        *value = Counter_Cfg[counterID]->value;
    }

    return E_OK;
}

extern StatusType Counter_GetElapsedValue(CounterType counterID, TickRefType value, TickRefType elapsedValue)
{
    if (counterID >= INVALID_COUNTER) {
        return E_OS_ID;
    }

    TickType currentValue = 0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        currentValue = Counter_Cfg[counterID]->value;
    }

    if (currentValue >= *value) {
        *elapsedValue = currentValue - *value;
    } else {
        *elapsedValue = Counter_Cfg[counterID]->maxallowedvalue - *value + currentValue + 1;
    }

    *value = currentValue;

    return E_OK;
}