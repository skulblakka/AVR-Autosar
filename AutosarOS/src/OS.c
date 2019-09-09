/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 

#include "OS.h"
#include "Task.h"
#include "TCB.h"
#include "assert.h"
#include "context.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/************************************************************************/
/* STATIC VARIABLES                                                     */
/************************************************************************/
/**
 * @brief   Current system tick
 */
static volatile uint32_t sysTick;
static enum tasks_e highestPrioTask;
enum tasks_e currentTask = INVALID_TASK;
/**
 * @brief   Stack-Pointer saved in current task control block
 */
uint8_t** pxCurrentTCB;

/**
 * @brief   Function pointer to current task function
 */
pTaskFxn pxAdr;

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
    
    OS_StartSysTimer();
    
    EnableAllInterrupts();
    
    OS_Schedule();
}

extern void OS_Schedule()
{
    // TODO Check if called from Cat1 ISR
    
    int16_t highestPrio = -1;
    highestPrioTask = INVALID_TASK;
    
    // Enter critical section
    DisableAllInterrupts();
    
    for (uint8_t i = 0; i < TASK_COUNT; i++) {
        if ((TCB_Cfg[i]->curState == PRE_READY || TCB_Cfg[i]->curState == READY || TCB_Cfg[i]->curState == RUNNING)
                && TCB_Cfg[i]->curPrio > highestPrio) {
            highestPrio = TCB_Cfg[i]->curPrio;
            highestPrioTask = (enum tasks_e) i;
        }
    }
    
    EnableAllInterrupts();
    
    if (highestPrioTask != INVALID_TASK) {
        OS_Dispatch();
    }
}

extern void OS_Dispatch()
{
    if (highestPrioTask != currentTask && highestPrioTask != INVALID_TASK) {
        if (currentTask != INVALID_TASK) {
            save_context();
        }
                    
        pxCurrentTCB = &(TCB_Cfg[highestPrioTask]->context);
        pxAdr = TCB_Cfg[highestPrioTask]->taskFxn;
        currentTask = highestPrioTask;
        if (TCB_Cfg[highestPrioTask]->curState == PRE_READY) {
            init_context();
        } else {
            restore_context();
        }
    
        TCB_Cfg[highestPrioTask]->curState = RUNNING;
    }
}

extern void EnableAllInterrupts()
{
    sei(); // Global interrupt enable
}

extern void DisableAllInterrupts()
{
    cli(); // Global interrupt disable
}