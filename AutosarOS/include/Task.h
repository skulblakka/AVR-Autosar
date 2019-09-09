/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 


#ifndef TASK_H_
#define TASK_H_

#include "TaskTypes.h"
#include "TCB.h"

#include <stdint.h>

void __attribute__((naked)) schedule();

// TODO: Return value
extern void OS_ActivateTask(enum tasks_e TaskID);
// TODO: Return value
extern void OS_TerminateTask();

#endif /* TASK_H_ */