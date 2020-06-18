/**
 * @file
 *
 * @brief       Counter management
 *
 * @date        2020-06-15
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef COUNTER_H_
#define COUNTER_H_

#include "Types.h"

/**
 * @brief   Increment counter
 *
 * This service increments a software counter.
 *
 * @param   counterID       Counter to be incremented
 *
 * @return  E_OK            No errors \n
 *          E_OS_ID         The counterID was not valid or the counter is implemented in hardware
 */
extern StatusType Counter_IncrementCounter(CounterType counterID);

/**
 * @brief   Read current counter value
 *
 * This service reads the current count value of a counter.
 *
 * @param   counterID       Counter which tick value should be read
 * @param   value           Contains the current tick value of the counter
 *
 * @return  E_OK            No errors \n
 *          E_OS_ID         The counterID was not valid
 */
extern StatusType Counter_GetCounterValue(CounterType counterID, TickRefType value);

/**
 * @brief   Get elapse counter value
 *
 * This service gets the number of ticks between the current tick value and a previously
 * read tick value.
 *
 * @param   counterID       The counter to be read
 * @param   value           Previously read tick value (contains the current tick afterwards)
 * @param   elapsedValue    Difference between previous and current value
 *
 * @return  E_OK            No errors \n
 *          E_OS_ID         The counterID was not valid \n
 *          E_OS_VALUE      The given value was not valid
 */
extern StatusType Counter_GetElapsedValue(CounterType counterID, TickRefType value, TickRefType elapsedValue);


#endif /* COUNTER_H_ */