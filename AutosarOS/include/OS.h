/**
 * @file
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

#include <stdbool.h>

extern enum tasks_e currentTask;


// TODO: Return type
// TODO: Application mode
extern void OS_StartOS();

// TODO: Return type
extern void __attribute__((naked)) OS_Schedule();
extern void OS_Switch();

// TODO Docs and check name conformity with OSEK/Autosar
extern void EnableAllInterrupts();
extern void DisableAllInterrupts();

/**
 * @brief   Force the next rescheduling
 *
 * If set to true the next call of #OS_Schedule will perform a rescheduling
 * even if current task is marked as non-preemptive.
 *
 * This should only be set immediately before #OS_Schedule is called.
 */
extern bool forceSchedule;

#ifdef STARTUPHOOK
/**
 * @brief   Hook function called after Startup of the OS
 */
extern void STARTUPHOOK();
#endif


#endif /* OS_H_ */