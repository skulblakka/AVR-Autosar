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

OS_CONFIG_TASK_BEGIN
    OS_CONFIG_TASK_DEF(Idle, 0, 128,     1, AUTOSTART,      BASIC,      PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T1,   4, 128,     1, AUTOSTART,      BASIC,      PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T2,   7, 128,     1, AUTOSTART,      BASIC,      PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T3,   8, 128,     1, NO_AUTOSTART,   EXTENDED,   NON_PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T4,   9, 128,     3, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T5,   5, 128,     2, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE)
OS_CONFIG_TASK_END

OS_CONFIG_INT_BEGIN
    OS_CONFIG_INT_DEF(INT0_vect)
    OS_CONFIG_INT_DEF(INT1_vect)
OS_CONFIG_INT_END

OS_CONFIG_RESOURCE_BEGIN
    OS_CONFIG_RESOURCE_DEF(Res1, 8, true)
    OS_CONFIG_RESOURCE_DEF(Res2, 8, true)
    OS_CONFIG_RESOURCE_DEF(Res3, 8, true)
OS_CONFIG_RESOURCE_END