/**
 * @file
 *
 * @brief       Types used for counter management
 *
 * @date        2020-06-15
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef COUNTERTYPES_H_
#define COUNTERTYPES_H_

/**
 * @brief   Data type of counter values
 */
typedef uint64_t TickType;

/**
 * @brief   Reference to counter values
 *
 * References #TickType
 */
typedef TickType* TickRefType;

/**
 * @brief   Type for counter reference
 */
typedef enum counters_e CounterType;

/**
 * @brief   Type of counter
 */
enum counterType_e {
    HARDWARE,   /**< Counter is implemented in hardware */
    SOFTWARE    /**< Counter is implemented in software */
};

/**
 * @brief   Data structure for counter
 */
struct counter_s {
    const TickType maxallowedvalue; /**< Maximum allowed value of counter */
    const TickType mincycle;        /**< Smallest allowed value for the cycle-parameter of cyclic alarms */
    const TickType ticksperbase;    /**< Number of ticks required to reach a counter-specific unit */
    const enum counterType_e type;  /**< Type of counter */
    const float secondspertick;     /**< Time of one counter tick in seconds */
    TickType value;                 /**< Current tick value of counter */
};

#endif /* COUNTERTYPES_H_ */