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

#include "OS.h"
#include "Task.h"
#include "OCB.h"
#include "assert.h"
#include "context.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/************************************************************************/
/* STATIC VARIABLES                                                     */
/************************************************************************/
/**
 * @brief   Highest priority task in READY state
 */
static enum tasks_e highestPrioTask;

/**
 * @brief   Current task being executed
 */
enum tasks_e currentTask = INVALID_TASK;

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
extern void OS_StartOS()
{
    DisableAllInterrupts();

    if (TASK_COUNT > 0) {
        assert(TASK_COUNT <= UINT8_MAX);
        /* Start all tasks configured as autostart */
        for (uint8_t i = 0; i < TASK_COUNT; i++) {
            if (TCB_Cfg[i]->autostart == AUTOSTART && TCB_Cfg[i]->curState == SUSPENDED) {
                TCB_Cfg[i]->curState = PRE_READY;
            }
        }
    }

#ifdef STARTUPHOOK
    STARTUPHOOK();
#endif

    /* Switch to first task */
    OS_Switch();
    init_context();
    TCB_Cfg[currentTask]->curState = RUNNING;
    TCB_Cfg[currentTask]->curNumberOfActivations += 1;

    OS_StartSysTimer();

    EnableAllInterrupts();

    asm volatile ("ret"); // Force return to prevent function epilogue removing non-existing data from task stack
}

extern void __attribute__((naked)) OS_Schedule()
{
    save_context();

    /* Calculate stack usage */
    if (currentTask != INVALID_TASK) {
        TCB_Cfg[currentTask]->curStackUse = TCB_Cfg[currentTask]->stack + TCB_Cfg[currentTask]->stackSize
                - TCB_Cfg[currentTask]->context;
        if (TCB_Cfg[currentTask]->curStackUse > TCB_Cfg[currentTask]->maxStackUse) {
            TCB_Cfg[currentTask]->maxStackUse = TCB_Cfg[currentTask]->curStackUse;
        }
    }

    if (!isISR && (currentTask == INVALID_TASK || TCB_Cfg[currentTask]->taskSchedule == PREEMPTIVE)) {
        // Enter critical section
        DisableAllInterrupts();

        if (currentTask != INVALID_TASK) {
            if (TCB_Cfg[currentTask]->curState == RUNNING) {
                TCB_Cfg[currentTask]->curState = READY;
            }
        }

        OS_Switch();

        // Reset scheduling state
        needScheduling = 0;

        assert(currentTask != INVALID_TASK);

        /* Change task state already to prevent changes to SREG */
        OsTaskState prevState = TCB_Cfg[currentTask]->curState;
        TCB_Cfg[currentTask]->curState = RUNNING;

        if (prevState == PRE_READY) {
            TCB_Cfg[currentTask]->context = TCB_Cfg[currentTask]->stack + TCB_Cfg[currentTask]->stackSize;
            init_context();
        } else {
            restore_context();
        }

        /***********************************************/
        /* NO CHANGE TO SREG MUST OCCUR AT THIS POINT! */
        /***********************************************/

        // Leave critical section
        EnableAllInterrupts();
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

extern void EnableAllInterrupts()
{
    sei(); // Global interrupt enable
}

extern void DisableAllInterrupts()
{
    cli(); // Global interrupt disable
}