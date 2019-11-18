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

#define TASK(TaskName) extern void Func ## TaskName(void)
#define PTASK(TaskName) &Func ## TaskName

typedef void (*pTaskFxn)();

typedef enum OsTaskType_e {
    BASIC = 0,      /*!< The task is a basic task, thus unable to wait on an event */
    EXTENDED        /*!< The task is a extended task */
} OsTaskType;

typedef enum OsTaskSchedule_e {
    PREEMPTIVE = 0, /*!< The task may be preempted by another task */
    NON_PREEMPTIVE  /*!< The task may not be preempted */
} OsTaskSchedule;

typedef enum OsTaskState_e {
    SUSPENDED = 0,  /*!< The task is suspended and will not be scheduled */
    PRE_READY,      /*!< The task is ready but its stack is uninitialized */
    READY,          /*!< The task is ready to be scheduled */
    RUNNING,        /*!< The task is currently running */
    WAITING         /*!< The task is waiting for an event */
} OsTaskState;

typedef enum OsTaskAutostart_e {
    NO_AUTOSTART = 0, /*!< Do not autostart task */
    AUTOSTART         /*!< Autostart task */
} OsTaskAutostart;

struct task_s {
    uint8_t* const stack;
    const uint16_t stackSize;
    const uint8_t prio;
    const uint8_t numberOfActivations;
    const OsTaskAutostart autostart;
    const OsTaskType taskType;
    const OsTaskSchedule taskSchedule;
    const pTaskFxn taskFxn;
    uint8_t* context;
    uint8_t curPrio;
    uint8_t curNumberOfActivations;
    OsTaskState curState;
    uint16_t curStackUse;
    uint16_t maxStackUse;
};

#endif /* TASKTYPES_H_ */