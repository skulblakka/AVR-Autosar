/**
 * @file
 *
 * @brief       Types used for alarm management
 *
 * @date        2020-05-29
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef ALARMTYPES_H_
#define ALARMTYPES_H_

#include "AlarmTypes.h"
#include "EventTypes.h"
#include "OCB.h"

/**
 * @brief   Define implementation of alarm callback
 *
 * Define used for implementation of alarm callback. This should be used in the app implementation
 * file. The callback must be defined in the app configuration using #OS_CONFIG_ALARM_CALLBACK.
 *
 * @param   Name    Name of the callback function
 */
#define ALARMCALLBACK(Name)     extern void Name(void)

/**
 * @brief   Alarm callback type
 */
typedef void (*pAlarmCallback)(void);

/**
 * @brief   Type for alarm base
 */
typedef volatile struct counter_s AlarmBaseType;

/**
 * @brief   Reference for alarm base
 */
typedef AlarmBaseType* AlarmBaseRefType;

typedef enum alarm_e AlarmType;

/**
 * @brief   Alarm action type
 */
enum alarmActionType_e {
    ALARM_ACTION_TASK = 0,  /**< Alarm activates task on expiration */
    ALARM_ACTION_EVENT,     /**< Alarm sets event on expiration */
    ALARM_ACTION_CALLBACK,  /**< Alarm calls callback on expiration */
    ALARM_ACTION_COUNTER    /**< Alarm increments counter on expiration */
};

/**
 * @brief   Data structure for alarm
 */
struct alarm_s {
    const AlarmBaseRefType alarmBase;           /**< Reference to counter used as alarm base */
    const EventMaskType event;                  /**< Event to set if type is #ALARM_ACTION_EVENT */
    const enum alarmActionType_e actionType;    /**< Type of the alarm */
    union {
        const void* action;                     /**< Untyped value to be used during configuration */
        const enum tasks_e task;                /**< Task to activate if type is #ALARM_ACTION_TASK or to set
                                                     event for if type is #ALARM_ACTION_EVENT */
        const pAlarmCallback callback;          /**< Callback to execute if type is #ALARM_ACTION_CALLBACK */
        const CounterType counter;              /**< Counter to increment if type is #ALARM_ACTION_COUNTER */
    } action;
    bool running;                               /**< Whether or not the alarm is currently running */
    TickType expiration;                        /**< Absolute expiration value. Will be compared against
                                                     alarmBase->value */
    TickType cycle;                             /**< Relative value used for cyclic alarm (zero for oneshot alarm) */
};

#endif /* ALARMTYPES_H_ */