/**
 * @file
 *
 * @brief       User application configuration
 *
 * This file contains the configuration for the user application.
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "CfgGenMacros.h"

#define OS_CONFIG_HOOK_STARTUP      true
#define OS_CONFIG_HOOK_SHUTDOWN     true
#define OS_CONFIG_HOOK_PRE_TASK     false
#define OS_CONFIG_HOOK_POST_TASK    false
#define OS_CONFIG_HOOK_ERROR        true

#define OS_CONFIG_EXTENDED          true

#define OS_CONFIG_MAX_ERROR_PARAM   3

#define OS_CONFIG_STACK_MONITORING  3

// Custom macro for enabling simulator specific code
#define OS_CONFIG_SIM               false

OS_CONFIG_INTERNAL_RESOURCE_BEGIN
    OS_CONFIG_INTERNAL_RESOURCE_DEF(IntRes1, 9)
    OS_CONFIG_INTERNAL_RESOURCE_DEF(IntRes2, 9)
OS_CONFIG_INTERNAL_RESOURCE_END

OS_CONFIG_TASK_BEGIN
    OS_CONFIG_TASK_DEF(Idle, 0,   64,     1, AUTOSTART,      BASIC,      PREEMPTIVE,     NULL,    0x00)
    OS_CONFIG_TASK_DEF(T1,   4,  256,     1, AUTOSTART,      BASIC,      PREEMPTIVE,     NULL,    0x00)
    OS_CONFIG_TASK_DEF(T2,   7,  256,     1, AUTOSTART,      BASIC,      PREEMPTIVE,     IntRes1, 0x00)
    OS_CONFIG_TASK_DEF(T3,   9,  256,     1, NO_AUTOSTART,   EXTENDED,   NON_PREEMPTIVE, NULL,    0x01)
    OS_CONFIG_TASK_DEF(T4,  10,  256,     3, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE, NULL,    0x00)
    OS_CONFIG_TASK_DEF(T5,   5,  256,     2, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE, NULL,    0x00)
    OS_CONFIG_TASK_DEF(T6,   7,  256,     2, NO_AUTOSTART,   BASIC,      PREEMPTIVE,     IntRes1, 0x00)
    OS_CONFIG_TASK_DEF(T7,  10,  256,     3, NO_AUTOSTART,   EXTENDED,   PREEMPTIVE,     NULL,    0x03)
OS_CONFIG_TASK_END

OS_CONFIG_INT_BEGIN
    OS_CONFIG_INT_DEF(INT0_vect, 11)
    OS_CONFIG_INT_DEF(INT1_vect, 11)
    OS_CONFIG_INT_DEF(TIMER1_COMPA_vect, 15)

#if defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true
    OS_CONFIG_INT_DEF(TIMER2_OVF_vect, 20)
#endif /* defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true */

OS_CONFIG_INT_END

OS_CONFIG_RESOURCE_BEGIN
    OS_CONFIG_RESOURCE_DEF(Res1, 8)
    OS_CONFIG_RESOURCE_DEF(Res2, 8)
    OS_CONFIG_RESOURCE_DEF(Res3, 8)
    OS_CONFIG_RESOURCE_DEF(Res4, 6)
OS_CONFIG_RESOURCE_END

OS_CONFIG_COUNTER_BEGIN
    OS_CONFIG_COUNTER_DEF(C1, 10, 1, 10, SOFTWARE, 1)
    OS_CONFIG_COUNTER_DEF(C2, 90, 5,  1, SOFTWARE, 1)
    OS_CONFIG_COUNTER_DEF(C3, 15, 1,  1, SOFTWARE, 1)
    OS_CONFIG_COUNTER_DEF(C4, 10, 1,  1, SOFTWARE, 1)
OS_CONFIG_COUNTER_END

OS_CONFIG_ALARM_CALLBACK(AlarmCb);

OS_CONFIG_ALARM_BEGIN
    OS_CONFIG_ALARM_DEF(Alarm1,             C3, true,  0x01,    ALARM_ACTION_EVENT,      T7,    1,    1)
    OS_CONFIG_ALARM_DEF(Alarm2,             C2, true,  0x00,     ALARM_ACTION_TASK,      T7,   30,   60)
    OS_CONFIG_ALARM_DEF(Alarm3,             C2, false, 0x00,  ALARM_ACTION_COUNTER,      C3,    0,    0)
    OS_CONFIG_ALARM_DEF(Alarm4,             C2, true,  0x00, ALARM_ACTION_CALLBACK, AlarmCb,    6,   12)
    OS_CONFIG_ALARM_DEF(Alarm5, SYSTEM_COUNTER, true,  0x00,  ALARM_ACTION_COUNTER,      C4,  570,  570)
    OS_CONFIG_ALARM_DEF(Alarm6, SYSTEM_COUNTER, true,  0x00,     ALARM_ACTION_TASK,      T7,  285,  570)
    OS_CONFIG_ALARM_DEF(Alarm7, SYSTEM_COUNTER, true,  0x00, ALARM_ACTION_CALLBACK, AlarmCb, 2568, 5130)
    OS_CONFIG_ALARM_DEF(Alarm8, SYSTEM_COUNTER, false, 0x02,    ALARM_ACTION_EVENT,      T7,    0,    0)
    OS_CONFIG_ALARM_DEF(Alarm9,             C1, false, 0x02,     ALARM_ACTION_TASK,      T7,    0,    0)
OS_CONFIG_ALARM_END