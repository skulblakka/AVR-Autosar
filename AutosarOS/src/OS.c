/**
 * @file
 *
 * @brief       Implementation of operating system control functions
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "OS.h"
#include "assert.h"
#include "context.h"
#include "Task.h"
#include "Resource.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/************************************************************************/
/* STATIC VARIABLES                                                     */
/************************************************************************/
/**
 * @brief   Highest priority task in READY state
 */
static enum tasks_e highestPrioTask;

/************************************************************************/
/* EXTERNAL VARIABLES                                                   */
/************************************************************************/
/**
 * @brief   Stack-Pointer saved in current task control block
 */
uint8_t* volatile* ptrCurrentStack;

/**
 * @brief   Function pointer to current task function
 */
pTaskFxn ptrCurrentFxnAddr;

bool forceSchedule;

/************************************************************************/
/* STATIC FUNCTIONS                                                     */
/************************************************************************/
/**
 * @brief   Start the system tick timer
 */
static void OS_StartSysTimer()
{
    TCCR0 = 1 << CS02 | 1 << CS00;      // Enable Timer0 with Prescaler 1024
    TIMSK |= 1 << TOIE0;                // Enable Overflow Interrupt (Timer0)
}

/************************************************************************/
/* EXTERN FUNCTIONS                                                     */
/************************************************************************/
extern void OS_StartOS(AppModeType mode)
{
    OS_DisableAllInterrupts();

    if (TASK_COUNT > 0) {
        assert(TASK_COUNT <= UINT8_MAX);
        /* Start all tasks configured as autostart */
        for (uint8_t i = 0; i < TASK_COUNT; i++) {
            if (TCB_Cfg[i]->autostart == AUTOSTART && TCB_Cfg[i]->curState == SUSPENDED) {
                TCB_Cfg[i]->curState = PRE_READY;
            }
        }
    }

#if defined(OS_CONFIG_HOOK_STARTUP) && OS_CONFIG_HOOK_STARTUP == true
    StartupHook();
#endif

    /* Switch to first task */
    OS_Switch();
    init_context();
    TCB_Cfg[currentTask]->curState = RUNNING;
    TCB_Cfg[currentTask]->curNumberOfActivations += 1;
    OS_GetInternalResource();

    OS_StartSysTimer();

    OS_EnableAllInterrupts();

    asm volatile ("ret"); // Force return to prevent function epilogue removing non-existing data from task stack
}

extern void OS_ShutdownOS(StatusType error)
{
    OS_DisableAllInterrupts();

#if defined(OS_CONFIG_HOOK_SHUTDOWN) && OS_CONFIG_HOOK_SHUTDOWN == true
    ShutdownHook(error);
#endif

    while (1);
}

extern void __attribute__((naked)) OS_ScheduleC()
{
    save_context();

    /* Calculate stack usage */
    if (currentTask != INVALID_TASK) {
        TCB_Cfg[currentTask]->curStackUse = TCB_Cfg[currentTask]->stack + TCB_Cfg[currentTask]->stackSize
                - TCB_Cfg[currentTask]->context;
        if (TCB_Cfg[currentTask]->curStackUse > TCB_Cfg[currentTask]->maxStackUse) {
            TCB_Cfg[currentTask]->maxStackUse = TCB_Cfg[currentTask]->curStackUse;
        }

        assert(TCB_Cfg[currentTask]->maxStackUse <= TCB_Cfg[currentTask]->stackSize);
    }

    if (!isISR && (currentTask == INVALID_TASK || (TCB_Cfg[currentTask]->taskSchedule == PREEMPTIVE || forceSchedule))) {
        // Enter critical section
        OS_DisableAllInterrupts();

        if (currentTask != INVALID_TASK) {
            if (TCB_Cfg[currentTask]->curState == RUNNING) {
                TCB_Cfg[currentTask]->curState = READY;
            }

#if defined(OS_CONFIG_HOOK_PRE_TASK) && OS_CONFIG_HOOK_PRE_TASK == true
            PreTaskHook();
#endif
        }

        OS_Switch();

        // Reset scheduling state
        needScheduling = 0;
        forceSchedule = false;

        assert(currentTask != INVALID_TASK);

        OS_GetInternalResource();

        /* Change task state already to prevent changes to SREG */
        OsTaskState prevState = TCB_Cfg[currentTask]->curState;
        TCB_Cfg[currentTask]->curState = RUNNING;

#if defined(OS_CONFIG_HOOK_POST_TASK) && OS_CONFIG_HOOK_POST_TASK == true
        PostTaskHook();
#endif

        if (prevState == PRE_READY) {
            TCB_Cfg[currentTask]->context = TCB_Cfg[currentTask]->stack + TCB_Cfg[currentTask]->stackSize - 1;
            init_context();
        } else {
            restore_context();
        }

        /***********************************************/
        /* NO CHANGE TO SREG MUST OCCUR AT THIS POINT! */
        /***********************************************/

        // Leave critical section
        OS_EnableAllInterrupts();
    } else {
        // Reschedule during system timer interrupt
        needScheduling = 1;

        restore_context();

        /***********************************************/
        /* NO CHANGE TO SREG MUST OCCUR AT THIS POINT! */
        /***********************************************/
    }

    asm volatile ("ret");
}

extern void OS_Switch()
{
    int16_t highestPrio = -1;
    highestPrioTask = INVALID_TASK;

    for (uint8_t i = 0; i < TASK_COUNT; i++) {
        if ((TCB_Cfg[i]->curState == PRE_READY || TCB_Cfg[i]->curState == READY || TCB_Cfg[i]->curState == RUNNING)
                && TCB_Cfg[i]->curPrio >= highestPrio) {
            highestPrio = TCB_Cfg[i]->curPrio;
            highestPrioTask = (enum tasks_e) i;
        }
    }

    currentTask = highestPrioTask;
    ptrCurrentStack = &(TCB_Cfg[currentTask]->context);
    ptrCurrentFxnAddr = TCB_Cfg[currentTask]->taskFxn;
}

extern inline void OS_EnableAllInterrupts(void)
{
    sei(); // Global interrupt enable
}

extern inline void OS_DisableAllInterrupts(void)
{
    cli(); // Global interrupt disable
}