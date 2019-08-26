/*
 * AppCfg.h
 *
 * Created: 06.08.2019 15:58:25
 *  Author: Pascal Romahn
 */ 

#include "CfgGenMacros.h"

OS_CONFIG_TASK_BEGIN
    OS_CONFIG_TASK_DEF(Idle, 0, 128,    1, AUTOSTART,   BASIC,      PREEMPTIVE)     // Idle Task stack will also be used as kernel stack
    OS_CONFIG_TASK_DEF(T1,   5, 64,     1, AUTOSTART,   BASIC,      NON_PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T2,   6, 64,     1, AUTOSTART,   BASIC,      NON_PREEMPTIVE)
    OS_CONFIG_TASK_DEF(T3,   8, 64,     1, AUTOSTART,   EXTENDED,   PREEMPTIVE)
OS_CONFIG_TASK_END
