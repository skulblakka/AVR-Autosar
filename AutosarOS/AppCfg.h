/**
 * @file
 *
 * @brief   User application configuration
 * 
 * This file contains the configuration for the user application.
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 

#include "CfgGenMacros.h"

OS_CONFIG_TASK_BEGIN
    OS_CONFIG_TASK_DEF(Idle, 0, 128,     1, AUTOSTART,      BASIC,      PREEMPTIVE)     // Idle Task stack will also be used as kernel stack
    OS_CONFIG_TASK_DEF(T1,   4, 128,     1, AUTOSTART,      BASIC,      PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T2,   7, 128,     1, AUTOSTART,      BASIC,      PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T3,   8, 128,     1, NO_AUTOSTART,   EXTENDED,   NON_PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T4,   9, 128,     1, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T5,   5, 128,     1, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE)
OS_CONFIG_TASK_END

OS_CONFIG_INT_BEGIN
    OS_CONFIG_INT_DEF(INT0_vect)
    OS_CONFIG_INT_DEF(INT1_vect)
OS_CONFIG_INT_END