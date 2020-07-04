/**
 * @file
 *
 * @brief       Operating System Control Block
 *
 * This contains the operating system control blocks generated during compilation.
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef OCB_H_
#define OCB_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "Types.h"

#include <avr/io.h>
#include <util/delay.h>



#define OS_CONFIG_GEN_FUNC_DECL
#include "AppCfg.h"
#undef OS_CONFIG_GEN_FUNC_DECL

/**
 * @brief   Is currently ISR context?
 */
extern volatile bool isISR;

/**
 * @brief   Priority of current Cat 2 ISR (zero if not in Cat 2 ISR)
 */
extern volatile uint8_t isCat2ISR;

/**
 * @brief   Task currently being executed
 */
extern volatile TaskType currentTask;

/**
 * @brief   Current task control blocks
 */
extern volatile struct task_s* TCB_Cfg[];

/**
 * @brief   Current system tick
 */
extern volatile uint32_t sysTick;

/**
 * @brief   Scheduling needed during timer interrupt
 *
 * If set to 0 the system timer will not trigger rescheduling.
 */
extern volatile uint8_t needScheduling;

/**
 * @brief   Block scheduling
 *
 * While set to a value different from zero all scheduling will be blocked.
 */
extern volatile uint8_t blockScheduling;

/**
 * @brief   Force the next rescheduling
 *
 * If set to true the next call of OS_Schedule() will perform a rescheduling
 * even if current task is marked as non-preemptive.
 *
 * This should only be set immediately before OS_Schedule() is called.
 */
extern bool forceScheduling;

/**
 * @brief   SysTick must be evaluated during timer interrupt
 *
 * This indicates whether or not the change in SysTick value must be evaluated
 * during the timer ISR. The value should be zero if no subsystem (like alarms)
 * require evaluation of the sysTick counter.
 */
extern volatile uint8_t needSysTickEval;

/**
 * @brief   Array used as SystemStack (value points to stack top)
 */
extern uint8_t OS_SystemStack[];

/**
 * @brief   Pointer to SystemStack base
 */
extern uint8_t* const OS_SystemStackPtr;

/**
 * @brief   Current resource control blocks
 */
extern volatile struct resource_s* Res_Cfg[];

/**
 * @brief   Resource queue for resources taken by Cat2 ISRs
 */
extern struct resource_s* volatile isrResourceQueue;

/**
 * @brief   Internal resource for tasks with no internal resource defined
 */
extern volatile struct internalResource_s IntResourceNULL_s;

/**
 * @brief   Current counter control blocks
 */
extern volatile struct counter_s* Counter_Cfg[];

/**
 * @brief   Current alarm control blocks
 */
extern volatile struct alarm_s* Alarm_Cfg[];

/**
 * @brief   Current schedule table control blocks
 */
extern struct scheduleTable_s* ScheduleTable_Cfg[];

#if (defined(OS_CONFIG_HOOK_ERROR) && OS_CONFIG_HOOK_ERROR == true)
/**
 * @brief   Block error hook from being executed
 *
 * If set to true the error block will not be executed. This is used to prevent recursive
 * calls of the hook.
 */
extern volatile bool blockErrorHook;
#endif /* (defined(OS_CONFIG_HOOK_ERROR) && OS_CONFIG_HOOK_ERROR == true) */

#if defined(OS_CONFIG_MAX_ERROR_PARAM) && OS_CONFIG_MAX_ERROR_PARAM >= 0
/**
 * @brief   Error information
 *
 * This structure is used to save information used during error handling. Usage and structure
 * depend on #OS_CONFIG_MAX_ERROR_PARAM.
 */
extern volatile struct errorInfo_s errorInfo;
#endif /* defined(OS_CONFIG_MAX_ERROR_PARAM) && OS_CONFIG_MAX_ERROR_PARAM >= 0 */

#if defined(OS_CONFIG_EXTENDED) && OS_CONFIG_EXTENDED == true
#define OS_EXTENDED true
#else /* defined(OS_CONFIG_EXTENDED) && OS_CONFIG_EXTENDED == true */
#define OS_EXTENDED false
#endif /* defined(OS_CONFIG_EXTENDED) && OS_CONFIG_EXTENDED == true */

#endif /* OCB_H_ */