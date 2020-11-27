/**
 * @file
 *
 * @brief       Context switching
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Task.h"
#include "OCB.h"

#include <stdint.h>

/**
 * @brief   Stack-Pointer saved in current task control block
 */
extern uint8_t* volatile* ptrCurrentStack;

/**
 * @brief   Function pointer to current task function
 */
extern pTaskFxn ptrCurrentFxnAddr;

/**
 * @brief   Save context of current task
 *
 * This function is used to save the current task context by saving the current processor state
 * onto the stack. The stack pointer is saved into the task context #ptrCurrentStack points to.
 *
 * Upon entry the current stack pointer points to the return address to OS_ScheduleC() and the
 * return address to the caller of OS_ScheduleC().
 * ```
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +  Return to   | Return to caller | ...
 * + OS_ScheduleC | of OS_ScheduleC  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * ^
 * Stack Pointer
 * ```
 *
 * The return address to OS_ScheduleC() is removed from the stack. Afterwards all registers are copied
 * onto the stack and the stack pointer is saved into #ptrCurrentStack. Afterwards the return address to
 * OS_ScheduleC() is again added to the stack.
 * ```
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * +  Return to   | R31 | .. | R2 | R1 | SREG | R0 | Return to caller | ...
 * + OS_ScheduleC |     |    |    |    |      |    | of OS_ScheduleC  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * ^              ^
 * Stack Pointer  *ptrCurrentStack
 * ```
 *
 * After the functions returns the current stack looks like this:
 * ```
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * + R31 | .. | R2 | R1 | SREG | R0 | Return to caller | ...
 * +     |    |    |    |      |    | of OS_ScheduleC  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * ^
 * Stack Pointer
 * *ptrCurrentStack
 * ```
 */
extern void save_context(void);

/**
 * @brief   Restore context of current task
 *
 * This function is used to restore the current task context by restoring the processor state
 * from the stack of the currently selected task.
 *
 * Upon entry both task stacks look like this. It is assumed that the old/previous task was saved via
 * save_context() beforehand.
 * ```
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * Old   +  Return to   | R31 | .. | R2 | R1 | SREG | R0 | Return to caller | ...
 * task  + OS_ScheduleC |     |    |    |    |      |    | of OS_ScheduleC  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *       ^
 *       Stack Pointer
 *
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * New   + R31 | .. | R2 | R1 | SREG | R0 | Return to caller | ...
 * task  +     |    |    |    |      |    | of OS_ScheduleC  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *       ^
 *       *ptrCurrentStack
 * ```
 *
 * The return address to OS_ScheduleC() is removed from the current stack and the stack pointer is
 * changed to the value #ptrCurrentStack points to. Afterwards the processor state is restored and the
 * return address to OS_ScheduleC() is again added to the new stack.
 * ```
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * Old   + R31 | .. | R2 | R1 | SREG | R0 | Return to caller | ...
 * task  +     |    |    |    |      |    | of OS_ScheduleC  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * New   +  Return to   | Return to caller | ...
 * task  + OS_ScheduleC | of OS_ScheduleC  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *       ^
 *       Stack Pointer
 * ```
 */
extern void restore_context(void);

/**
 * @brief   Initialize context of current task
 *
 * This function is used to initialize the context of the current task.
 *
 * Upon entry the current stack pointer points to the return address to OS_ScheduleC().
 * ```
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * Old   +  Return to   | Return to caller | ...
 * task  + OS_ScheduleC | of OS_ScheduleC  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *       ^
 *       Stack Pointer
 * ```
 *
 * The return address to OS_ScheduleC() is removed from the current stack and the stack pointer is changed to
 * the value #ptrCurrentStack points to. Afterwards the value #ptrCurrentFxnAddr points to and the return address to
 * OS_ScheduleC() are added to the stack.
 * ```
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * New   +  Return to   | Return to |
 * task  + OS_ScheduleC |  taskFxn  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-
 *       ^                          ^
 *       Stack Pointer              *ptrCurrentStack
 * ```
 */
extern void init_context(void);

#endif /* CONTEXT_H_ */