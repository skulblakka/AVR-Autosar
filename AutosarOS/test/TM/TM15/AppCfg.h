/**
 * @file
 *
 * @brief       Application configuration for task management test procedure 15
 *
 * @date        2020-07-13
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#define OS_CONFIG_HOOK_STARTUP      true
#define OS_CONFIG_HOOK_SHUTDOWN     true
#define OS_CONFIG_HOOK_PRE_TASK     false
#define OS_CONFIG_HOOK_POST_TASK    false
#define OS_CONFIG_HOOK_ERROR        true
#define OS_CONFIG_HOOK_PROTECTION   false

#define OS_CONFIG_EXTENDED          true

#define OS_CONFIG_MAX_ERROR_PARAM   3

#define OS_CONFIG_STACK_MONITORING  3

// Custom macro for enabling simulator specific code
#define OS_CONFIG_SIM               false

OS_CONFIG_SYSTEM_STACK(128)

OS_CONFIG_INTERNAL_RESOURCE_BEGIN
OS_CONFIG_INTERNAL_RESOURCE_END

OS_CONFIG_TASK_BEGIN
    OS_CONFIG_TASK_DEF(Idle,    0,  128,     1, AUTOSTART,    BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task1,   8,  128,     1, AUTOSTART,    BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task2,   7,  128,     1, NO_AUTOSTART, BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task3,   6,  128,     1, NO_AUTOSTART, BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task4,   5,  128,     1, NO_AUTOSTART, BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task5,   4,  128,     1, NO_AUTOSTART, BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task6,   3,  128,     1, NO_AUTOSTART, BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task7,   2,  128,     1, NO_AUTOSTART, BASIC,  PREEMPTIVE,     NULL,    0b000)
    OS_CONFIG_TASK_DEF(Task8,   1,  128,     1, NO_AUTOSTART, BASIC,  PREEMPTIVE,     NULL,    0b000)
OS_CONFIG_TASK_END

OS_CONFIG_INT_BEGIN

#if defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true
    OS_CONFIG_INT_DEF(TIMER2_OVF_vect, 0)
#endif /* defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true */

OS_CONFIG_INT_END

OS_CONFIG_RESOURCE_BEGIN
OS_CONFIG_RESOURCE_END

OS_CONFIG_COUNTER_BEGIN
OS_CONFIG_COUNTER_END

OS_CONFIG_ALARM_BEGIN
OS_CONFIG_ALARM_END

OS_CONFIG_SCHEDULE_TABLE_BEGIN
OS_CONFIG_SCHEDULE_TABLE_END