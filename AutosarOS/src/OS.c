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
#include <util/atomic.h>

/************************************************************************/
/* STATIC VARIABLES                                                     */
/************************************************************************/
/**
 * @brief   Highest priority task in READY state
 */
static TaskType highestPrioTask;

/**
 * @brief   Saved interrupt states
 *
 * This variable is used to save the interrupts states in OS_ResumeAllInterrupts() and
 * OS_SuspendAllInterrupts() allowing nesting of said functions.
 */
static uint8_t intStates;

/**
 * @brief   Saved OS interrupt states
 *
 * This variable is used to save the interrupts states in OS_ResumeOSInterrupts() and
 * OS_SuspendOSInterrupts() allowing nesting of said functions.
 */
static uint8_t osIntStates;

/**
 * @brief   Currently active application mode
 */
static AppModeType activeApplicationMode;

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
    OS_SET_ERROR_INFO1(OSServiceId_StartOS, &mode, sizeof(mode));

    OS_DisableAllInterrupts();

    activeApplicationMode = mode;

    if (TASK_COUNT > 0) {
        assert(TASK_COUNT <= UINT8_MAX);
        /* Start all tasks configured as autostart */
        for (uint8_t i = 0; i < TASK_COUNT; i++) {
            if (TCB_Cfg[i]->autostart == AUTOSTART && TCB_Cfg[i]->curState == SUSPENDED) {
                TCB_Cfg[i]->curState = PRE_READY;
            }

#if defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 2
            // Set stack top marker
            *(TCB_Cfg[i]->stack - 1) = 0xBE;
#if OS_CONFIG_STACK_MONITORING >= 3
            // Set stack memory for stack monitoring
            memset(TCB_Cfg[i]->stack, 0xBE, TCB_Cfg[i]->stackSize);
#endif /* OS_CONFIG_STACK_MONITORING >= 3 */
#endif /* defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 2 */
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
    Resource_GetInternalResource();

    OS_StartSysTimer();

    OS_EnableAllInterrupts();

    asm volatile ("ret"); // Force return to prevent function epilogue removing non-existing data from task stack
}

extern void OS_ShutdownOS(StatusType error)
{
    OS_SET_ERROR_INFO1(OSServiceId_ShutdownOS, &error, sizeof(error));

    OS_DisableAllInterrupts();

#if defined(OS_CONFIG_HOOK_SHUTDOWN) && OS_CONFIG_HOOK_SHUTDOWN == true
    ShutdownHook(error);
#endif

    assert(false); // We never want to reach this

    while (1);
}

extern void __attribute__((naked)) OS_ScheduleC()
{
    save_context();


#if defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 1
    /* Calculate stack usage */
    if (currentTask != INVALID_TASK) {
        TCB_Cfg[currentTask]->curStackUse = TCB_Cfg[currentTask]->stack + TCB_Cfg[currentTask]->stackSize
                - TCB_Cfg[currentTask]->context;
        if (TCB_Cfg[currentTask]->curStackUse > TCB_Cfg[currentTask]->maxStackUse) {
            TCB_Cfg[currentTask]->maxStackUse = TCB_Cfg[currentTask]->curStackUse;
        }

#if OS_CONFIG_STACK_MONITORING >= 2
        if (*(TCB_Cfg[currentTask]->stack - 1) != 0xBE) {
            if (TCB_Cfg[currentTask]->maxStackUse <= TCB_Cfg[currentTask]->stackSize) {
                TCB_Cfg[currentTask]->maxStackUse = TCB_Cfg[currentTask]->stackSize + 1;
            }
        }
#endif /* OS_CONFIG_STACK_MONITORING >= 2 */

        if (TCB_Cfg[currentTask]->maxStackUse > TCB_Cfg[currentTask]->stackSize) {
            OS_ShutdownOS(E_OS_STACKFAULT);
        }
    }
#endif /* defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 1 */

    if (!isISR && (currentTask == INVALID_TASK || (TCB_Cfg[currentTask]->taskSchedule == PREEMPTIVE || forceScheduling))) {
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
        forceScheduling = false;

        assert(currentTask != INVALID_TASK);

        Resource_GetInternalResource();

        /* Change task state already to prevent changes to SREG */
        TaskStateType prevState = TCB_Cfg[currentTask]->curState;
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
            highestPrioTask = (TaskType) i;
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

extern void OS_ResumeAllInterrupts(void)
{
    OS_SET_ERROR_INFO0(OSServiceId_ResumeAllInterrupts);

    bool enable = (intStates & 0b1);
    intStates = (intStates >> 1);

    if (enable) {
        OS_EnableAllInterrupts();
    }
}
extern void OS_SuspendAllInterrupts(void)
{
    OS_SET_ERROR_INFO0(OSServiceId_SuspendAllInterrupts);

    intStates = (intStates << 1) | ((SREG >> SREG_I) & 0b1);

    OS_DisableAllInterrupts();
}
extern void OS_ResumeOSInterrupts(void)
{
    OS_SET_ERROR_INFO0(OSServiceId_ResumeOSInterrupts);

    bool enable = (osIntStates & 0b1);
    osIntStates = (osIntStates >> 1);

    if (enable) {
        OS_EnableAllInterrupts();
    }
}
extern void OS_SuspendOSInterrupts(void)
{
    OS_SET_ERROR_INFO0(OSServiceId_SuspendOSInterrupts);

    osIntStates = (osIntStates << 1) | ((SREG >> SREG_I) & 0b1);

    OS_DisableAllInterrupts();
}

extern AppModeType OS_GetActiveApplicationMode(void)
{
    OS_SET_ERROR_INFO0(OSServiceId_GetActiveApplicationMode);

    return activeApplicationMode;
}