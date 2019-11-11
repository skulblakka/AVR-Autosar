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
 */
extern void save_context();

/**
 * @brief   Restore context of current task
 */
extern void restore_context();

/**
 * @brief   Initialize context of current task
 *
 * This function is used to initialize the context of the current task.
 */
extern void init_context();

#endif /* CONTEXT_H_ */