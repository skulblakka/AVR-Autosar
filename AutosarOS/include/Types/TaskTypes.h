/**
 * @file
 *
 * @brief       Types used for tasks
 *
 * @date        2019-09-03
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef TASKTYPES_H_
#define TASKTYPES_H_

#include <stdint.h>
#include "EventTypes.h"

#define TASK(TaskName) extern void Func ## TaskName(void)
#define PTASK(TaskName) &Func ## TaskName

typedef void (*pTaskFxn)();

/**
 * @brief   Type for task reference
 */
typedef enum tasks_e TaskType;

/**
 * @brief   Reference to a task reference
 *
 * Reference an #TaskType
 */
typedef TaskType* TaskRefType;

/**
 * @brief   OSEK task type
 */
typedef enum OsTaskType_e {
    BASIC = 0,      /**< The task is a basic task, thus unable to wait on an event */
    EXTENDED        /**< The task is a extended task */
} OsTaskType;

/**
 * @brief   Task scheduling scheme
 */
typedef enum OsTaskSchedule_e {
    PREEMPTIVE = 0, /**< The task may be preempted by another task */
    NON_PREEMPTIVE  /**< The task may not be preempted */
} OsTaskSchedule;

/**
 * @brief   Task state
 */
typedef enum OsTaskState_e {
    SUSPENDED = 0,  /**< The task is suspended and will not be scheduled */
    PRE_READY,      /**< The task is ready but its stack is uninitialized */
    READY,          /**< The task is ready to be scheduled */
    RUNNING,        /**< The task is currently running */
    WAITING         /**< The task is waiting for an event */
} TaskStateType;

/**
 * @brief   Type for task state reference
 *
 * Reference a #TaskStateType.
 */
typedef TaskStateType* TaskStateRefType;

/**
 * @brief   Task autostart mode
 */
typedef enum OsTaskAutostart_e {
    NO_AUTOSTART = 0, /**< Do not autostart task */
    AUTOSTART         /**< Autostart task */
} OsTaskAutostart;

/**
 * @brief   Data structure for task
 */
struct task_s {
    uint8_t* const stack;                                       /**< Pointer to stack base */
    const uint16_t stackSize;                                   /**< Size of stack in bytes */
    const uint8_t prio;                                         /**< Static priority of the task */
    const uint8_t numberOfActivations;                          /**< Number of simultaneous activations */
    const OsTaskAutostart autostart;                            /**< Whether or not to start the task during startup */
    const OsTaskType taskType;                                  /**< Type of task */
    const OsTaskSchedule taskSchedule;                          /**< Scheduling-type of task */
    const pTaskFxn taskFxn;                                     /**< Pointer to task function */
    volatile struct internalResource_s* const internalResource; /**< Pointer to internal resource */
    const EventMaskType events;                                 /**< Mask of assigned task events */
    uint8_t* context;                                           /**< Current context/stack pointer */
    uint8_t curPrio;                                            /**< Current priority */
    uint8_t curNumberOfActivations;                             /**< Current number of activations */
    TaskStateType curState;                                     /**< Current state of the task */

#if defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 1
    uint16_t curStackUse;                                       /**< Current stack usage */
    uint16_t maxStackUse;                                       /**< Maximum recorded stack usage */
#endif /* defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 1 */

    struct resource_s* resourceQueue;                           /**< Current queue of allocated resources */
    EventMaskType waitEvents;                                   /**< Mask of events the task is currently waiting on */
    EventMaskType pendingEvents;                                /**< Mask of currently pending events */
};

#endif /* TASKTYPES_H_ */