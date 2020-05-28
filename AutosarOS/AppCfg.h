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

OS_CONFIG_INTERNAL_RESOURCE_BEGIN
    OS_CONFIG_INTERNAL_RESOURCE_DEF(IntRes1, 9)
    OS_CONFIG_INTERNAL_RESOURCE_DEF(IntRes2, 9)
OS_CONFIG_INTERNAL_RESOURCE_END

OS_CONFIG_TASK_BEGIN
    OS_CONFIG_TASK_DEF(Idle, 0,  64,     1, AUTOSTART,      BASIC,      PREEMPTIVE,     NULL,    0x00)
    OS_CONFIG_TASK_DEF(T1,   4,  64,     1, AUTOSTART,      BASIC,      PREEMPTIVE,     NULL,    0x00)
    OS_CONFIG_TASK_DEF(T2,   7,  64,     1, AUTOSTART,      BASIC,      PREEMPTIVE,     IntRes1, 0x00)
    OS_CONFIG_TASK_DEF(T3,   9,  64,     1, NO_AUTOSTART,   EXTENDED,   NON_PREEMPTIVE, NULL,    0x01)
    OS_CONFIG_TASK_DEF(T4,  10,  64,     3, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE, NULL,    0x00)
    OS_CONFIG_TASK_DEF(T5,   5,  64,     2, NO_AUTOSTART,   BASIC,      NON_PREEMPTIVE, NULL,    0x00)
    OS_CONFIG_TASK_DEF(T6,   7,  64,     2, NO_AUTOSTART,   BASIC,      PREEMPTIVE,     IntRes1, 0x00)
OS_CONFIG_TASK_END

OS_CONFIG_INT_BEGIN
    OS_CONFIG_INT_DEF(INT0_vect, 11)
    OS_CONFIG_INT_DEF(INT1_vect, 0)
OS_CONFIG_INT_END

OS_CONFIG_RESOURCE_BEGIN
    OS_CONFIG_RESOURCE_DEF(Res1, 8)
    OS_CONFIG_RESOURCE_DEF(Res2, 8)
    OS_CONFIG_RESOURCE_DEF(Res3, 8)
    OS_CONFIG_RESOURCE_DEF(Res4, 6)
OS_CONFIG_RESOURCE_END