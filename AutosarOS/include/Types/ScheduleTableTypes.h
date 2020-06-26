/**
 * @file
 *
 * @brief       Types for schedule tables
 *
 * @date        2020-06-25
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef SCHEDULETABLETYPES_H_
#define SCHEDULETABLETYPES_H_

#include "CounterTypes.h"
#include "TaskTypes.h"
#include "EventTypes.h"

/**
 * @brief   Type for schedule table reference
 */
typedef enum scheduleTables_e ScheduleTableType;

/**
 * @brief   State of schedule table
 */
typedef enum scheduleTableState_e {
    SCHEDULETABLE_STOPPED = 0,  /**< Schedule table is stopped */
    SCHEDULETABLE_NEXT,         /**< Schedule table is queued after another table */
    SCHEDULETABLE_RUNNING       /**< Schedule table is running */
} ScheduleTableStatusType;

/**
 * @brief   Type of status reference
 */
typedef ScheduleTableStatusType* ScheduleTableStatusRefType;

/**
 * @brief   Structure for task activation action
 */
struct scheduleTableExpiryActionTask_s {
    const struct scheduleTableExpiryActionTask_s* next;     /**< Pointer to next element (NULL if this is the last) */
    const TaskType task;                                    /**< ID of task to activate */
};

/**
 * @brief   Structure for event activation action
 */
struct scheduleTableExpiryActionEvent_s {
    const struct scheduleTableExpiryActionEvent_s* next;    /**< Pointer to next element (NULL if this is the last */
    const TaskType task;                                    /**< ID of task to set events for */
    const EventMaskType event;                              /**< Mask of events to set */
};

/**
 * @brief   Structure for expiry point
 */
struct scheduleTableExpiryPoint_s {
    const TickType offset;                                          /**< Offset from schedule table start */
    const struct scheduleTableExpiryActionTask_s* taskActionList;   /**< List of task activation actions (may be NULL) */
    const struct scheduleTableExpiryActionEvent_s* eventActionList; /**< List of event activation actions (may be NULL) */
    const struct scheduleTableExpiryPoint_s* next;                  /**< Pointer to next element (NULL if this is the
                                                                         final expiry point) */
};

/**
 * @brief   Structure for schedule table
 */
struct scheduleTable_s {
    const TickType finalDelay;                                  /**< Final delay of schedule table (applied after last
                                                                     expiry point */
    const struct scheduleTableExpiryPoint_s expiryPointList;    /**< List of expiry points */
    const CounterType counter;                                  /**< Counter driving the schedule table */
    const bool autoStart;                                       /**< If true the schedule table will be started at
                                                                     startup */
    const bool cyclic;                                          /**< If true the schedule table is cyclic */
    TickType currentTick;                                       /**< Current tick of the schedule table */
    ScheduleTableStatusType currentState;                       /**< Current state of the schedule table */
    ScheduleTableType next;                                     /**< Schedule table queued after the current one */
};

#endif /* SCHEDULETABLETYPES_H_ */