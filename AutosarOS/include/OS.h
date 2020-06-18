/**
 * @file
 *
 * @brief       Operating system control
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef OS_H_
#define OS_H_

#include "Types.h"
#include "OCB.h"

#include <stdbool.h>


/**
 * @brief   Start operating system
 *
 * This service starts the operating system in the specified application mode. The default
 * application mode is #OSDEFAULTAPPMODE.
 *
 * @note    Calls to this functions are only allowed outside of the operating system. This call does not
 *          return.
 *
 * @note    This implementation only supports the default mode.
 *
 * @param   mode        Application mode
 */
extern void OS_StartOS(AppModeType mode);

/**
 * @brief   Shutdown operating system
 *
 * This service shuts down the operating system and does not return. If a ShutdownHook is configured
 * it will be called before the system is terminated.
 *
 * @param   error       Error occurred
 */
extern void OS_ShutdownOS(StatusType error);

// TODO: Return type
extern void OS_Schedule(void);
extern void __attribute__((naked)) OS_ScheduleC();
extern void OS_Switch();

/**
 * @brief   Enable all interrupts
 *
 * This service enables all interrupts. It is the counterpart to OS_DisableAllInterrupts().
 *
 * Nesting is not supported by this call.
 */
extern void OS_EnableAllInterrupts(void);

/**
 * @brief   Disable all interrupts
 *
 * This service disables all interrupts. It is the counterpart to OS_EnableAllInterrupts().
 *
 * Nesting is not supported by this call.
 */
extern void OS_DisableAllInterrupts(void);

/**
 * @brief   Force the next rescheduling
 *
 * If set to true the next call of OS_Schedule() will perform a rescheduling
 * even if current task is marked as non-preemptive.
 *
 * This should only be set immediately before OS_Schedule() is called.
 */
extern bool forceSchedule;

#if defined(OS_CONFIG_HOOK_STARTUP) && OS_CONFIG_HOOK_STARTUP == true
/**
 * @brief   Hook function for startup
 *
 * This hook function is called after system startup and before the scheduler is running.
 *
 * @warning This function is executed with interrupts disabled and must not activate them!
 */
extern void StartupHook(void);
#endif

#if defined(OS_CONFIG_HOOK_SHUTDOWN) && OS_CONFIG_HOOK_SHUTDOWN == true
/**
 * @brief   Hook function for shutdown
 *
 * This hook function is called before the system is shutdown.
 *
 * @warning This function is executed with interrupts disabled and must not activate them!
 *
 * @param   error       Error passed to OS_ShutdownOS()
 */
extern void ShutdownHook(StatusType error);
#endif

#if (defined(OS_CONFIG_HOOK_PRE_TASK) && OS_CONFIG_HOOK_PRE_TASK == true) || defined(__DOXYGEN__)
/**
 * @brief   PreTask hook function
 *
 * This hook function is called after a new task has entered the running state but before
 * it is executed. The ID of the task can be evaluated using OS_GetTaskID().
 *
 * @warning This function is executed with interrupts disabled and must not activate them!
 */
extern void PreTaskHook();
#endif

#if (defined(OS_CONFIG_HOOK_POST_TASK) && OS_CONFIG_HOOK_POST_TASK == true) || defined(__DOXYGEN__)
/**
 * @brief   PostTask hook function
 *
 * This hook function is called before a task leaves the running state but before
 * a new task is selected. The ID of the task can be evaluated using OS_GetTaskID().
 *
 * @warning This function is executed with interrupts disabled and must not activate them!
 */
extern void PostTaskHook();
#endif

#if (defined(OS_CONFIG_HOOK_ERROR) && OS_CONFIG_HOOK_ERROR == true) || defined(__DOXYGEN__)
/**
 * @brief   PostTask hook function
 *
 * This hook function is called when a system service return StatusType not equal E_OK.
 *
 * @warning This function is executed with interrupts disabled and must not activate them!
 */
extern void ErrorHook();
#endif

#endif /* OS_H_ */