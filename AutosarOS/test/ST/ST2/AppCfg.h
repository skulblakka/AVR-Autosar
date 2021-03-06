/**
 * @file
 *
 * @brief       Application configuration for schedule tables test procedure 2
 *
 * @date        2020-07-20
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

OS_CONFIG_INTERNAL_RESOURCE_BEGIN
OS_CONFIG_INTERNAL_RESOURCE_END

OS_CONFIG_SYSTEM_STACK(256)

OS_CONFIG_TASK_BEGIN
    OS_CONFIG_TASK_DEF(Idle,  0,  256,     1,    AUTOSTART,      BASIC,      PREEMPTIVE,     NULL,    0b00)
    OS_CONFIG_TASK_DEF(Task1, 1,  256,     1,    AUTOSTART,      BASIC,      PREEMPTIVE,     NULL,    0b00)
    OS_CONFIG_TASK_DEF(Task2, 2,  256,     1, NO_AUTOSTART,   EXTENDED,  NON_PREEMPTIVE,     NULL,    0b01)
    OS_CONFIG_TASK_DEF(Task3, 3,  256,     1, NO_AUTOSTART,   EXTENDED,      PREEMPTIVE,     NULL,    0b10)
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
OS_CONFIG_ALARM_END

OS_CONFIG_SCHEDULE_TABLE_BEGIN
    /* Schedule table ST1 */
    OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(ST1, Counter1, false, false, 2)
        OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(3)
            /* Expiry point at 0 */
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN( 0, 1, 0)
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
                    OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task2)
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
            /* Expiry point at 1 */
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN( 1, 0, 1)
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
                    OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task2, 0b01)
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
            /* Expiry point at 2 */
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN( 2, 1, 1)
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
                    OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task3)
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
                    OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task3, 0b10)
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
        OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
    OS_CONFIG_SCHEDULE_TABLE_DEF_END
    
    /* Schedule table ST2 */
    OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(ST2, Counter1, false, false, 0)
        OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(1)
            /* Expiry point at 0 */
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN( 0, 2, 2)
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
                    OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task2)
                    OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task3)
                OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
                    OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task2, 0b01)
                    OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task3, 0b10)
                OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
            OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
        OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
    OS_CONFIG_SCHEDULE_TABLE_DEF_END
OS_CONFIG_SCHEDULE_TABLE_END