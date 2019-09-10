/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 

#include "Task.h"
#include "OS.h"

{

extern void OS_ActivateTask(enum tasks_e TaskID)
{
    // TODO: Handle multiple activations
    if (TCB_Cfg[TaskID]->curState == SUSPENDED) {
        TCB_Cfg[TaskID]->curState = PRE_READY;
        
        OS_Schedule();
    } else if (TCB_Cfg[TaskID]->curState != RUNNING) {
        TCB_Cfg[TaskID]->curState = READY;
        
        OS_Schedule();
    }
}

extern void OS_TerminateTask()
{
    TCB_Cfg[currentTask]->curState = SUSPENDED;
    OS_Schedule();
}