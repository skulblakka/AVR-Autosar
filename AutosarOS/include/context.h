/**
 * @file
 * 
 * @brief   Context switching
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 


#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Task.h"
#include "OCB.h"

#include <stdint.h>

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

/**
 * @brief   Stack-Pointer saved in current task control block
 */
extern uint8_t * volatile * ptrCurrentStack;

/**
 * @brief   Function pointer to current task function
 */
extern pTaskFxn ptrCurrentFxnAddr;

#endif /* CONTEXT_H_ */