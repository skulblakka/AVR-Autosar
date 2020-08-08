/**
 * @file
 *
 * @brief       Application configuration for hook test procedure 1
 *
 * @date        2020-07-16
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#define OS_CONFIG_HOOK_STARTUP      true
#define OS_CONFIG_HOOK_SHUTDOWN     true
#define OS_CONFIG_HOOK_PRE_TASK     true
#define OS_CONFIG_HOOK_POST_TASK    true
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
    OS_CONFIG_TASK_DEF(Idle,    0,  128,     1,    AUTOSTART, BASIC,     PREEMPTIVE,     NULL, 0b00)
    OS_CONFIG_TASK_DEF(Task1,   1,  256,     1, NO_AUTOSTART, BASIC, NON_PREEMPTIVE,     NULL, 0b00)
    OS_CONFIG_TASK_DEF(Task2,   2,  256,     1, NO_AUTOSTART, BASIC, NON_PREEMPTIVE,     NULL, 0b00)
OS_CONFIG_TASK_END

OS_CONFIG_INT_BEGIN

#if defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true
    OS_CONFIG_INT_DEF(TIMER2_OVF_vect, 0)
#endif /* defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true */

OS_CONFIG_INT_END

OS_CONFIG_RESOURCE_BEGIN
OS_CONFIG_RESOURCE_END

OS_CONFIG_COUNTER_BEGIN
    OS_CONFIG_COUNTER_DEF(Counter1, 16, 1, 1, SOFTWARE, 1)
OS_CONFIG_COUNTER_END

OS_CONFIG_ALARM_BEGIN
    OS_CONFIG_ALARM_DEF(Alarm1, Counter1, false, 0b10, ALARM_ACTION_TASK, Task2, 1, 1)
OS_CONFIG_ALARM_END

OS_CONFIG_SCHEDULE_TABLE_BEGIN
OS_CONFIG_SCHEDULE_TABLE_END