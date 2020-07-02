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

/**
 * @brief   Schedule task
 *
 * This function calls OS_ScheduleC() unless scheduling is currently blocked. This is basically the
 * entry point to the scheduler and should be called if scheduling is required or wanted.
 */
extern void OS_Schedule(void);

/**
 * @brief   Task switch
 *
 * This function performs the actual scheduling. It selects a new task to be executed and switches
 * the current context accordingly. It should not be called directly and instead be used with
 * OS_Schedule().
 */
extern void __attribute__((naked)) OS_ScheduleC(void);

/**
 * @brief   Switch to new task
 *
 * Select the task with the highest priority that is ready and change the state for the context
 * switch.
 */
extern void OS_Switch(void);

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
 * @brief   Resume all interrupts
 *
 * This service resumes all interrupts disabled by the previous call to OS_SuspendAllInterrupts().
 *
 * Nesting is supported by this call.
 */
extern void OS_ResumeAllInterrupts(void);

/**
 * @brief   Suspend all interrupts
 *
 * This service suspends all interrupts. It is the counterpart to OS_ResumeAllInterrupts().
 *
 * Nesting is supported by this call
 */
extern void OS_SuspendAllInterrupts(void);

/**
 * @brief   Resume OS interrupts
 *
 * This service resumes OS interrupts disabled by the privious call to OS_SuspendOSInterrupts().
 *
 * Nesting is supported by this call.
 *
 * @note    Because of hardware limitations there is no difference between this function and
 *          OS_ResumeAllInterrupts(). However the correct counterpart must be used.
 *
 * @warning Nesting is only supported up to 8 levels!
 */
extern void OS_ResumeOSInterrupts(void);

/**
 * @brief   Suspend OS interrupts
 *
 * This service suspends all OS interrupts. It is the counterpart to OS_ResumeOSInterrupts().
 *
 * Nesting is supported by this call.
 *
 * @note    Because of hardware limitation there is no difference between this function and
 *          OS_SuspendAllInterrupts(). However the correct counterpart must be used.
 *
 * @warning Nesting is only supported up to 8 levels!
 */
extern void OS_SuspendOSInterrupts(void);

/**
 * @brief   Get currently active application mode
 *
 * @return  Current application mode
 */
extern AppModeType OS_GetActiveApplicationMode(void);

/**
 * @brief   Internal function for protection hook handling
 * 
 * This function is called when a protection error is detected. It will call the configured
 * protection hook if available and handle its return value.
 * 
 * @param   error       Type of error that triggered the protection hook
 */
extern void OS_ProtectionHookInternal(StatusType error);

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
extern void PreTaskHook(void);
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
extern void PostTaskHook(void);
#endif

#if (defined(OS_CONFIG_HOOK_ERROR) && OS_CONFIG_HOOK_ERROR == true) || defined(__DOXYGEN__)
/**
 * @brief   PostTask hook function
 *
 * This hook function is called when a system service return StatusType not equal E_OK.
 *
 * @warning This function is executed with interrupts disabled and must not activate them!
 */
extern void ErrorHook(void);
#endif

#if (defined(OS_CONFIG_HOOK_PROTECTION) && OS_CONFIG_HOOK_PROTECTION == true) || defined(__DOXYGEN__)
/**
 * @brief   ProtectionHook function
 * 
 * This hook function is called when an error is detected by the protection facilities (e.g.
 * stack monitoring).
 *
 * @warning This function is executed with interrupts disabled and must not activate them!
 * 
 * @param   fatalError      Type of error that triggered the protection hook
 * 
 * @return  Action the OS shall take after the protection hook
 */
extern ProtectionReturnType ProtectionHook(StatusType fatalError);
#endif

#endif /* OS_H_ */