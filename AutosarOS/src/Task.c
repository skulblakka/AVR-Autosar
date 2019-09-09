/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 

#include "Task.h"
#include "OS.h"


void __attribute__((naked)) schedule()
{
    //static uint8_t i = 0;
    //if (i == 0) {
        //pxCurrentTCB = &(TCB_Cfg[1]->context);
        //pxAdr = TCB_Cfg[1]->taskFxn;
        //init_context();
    //} else if (i == 1) {
        //save_context();
        //pxCurrentTCB = &(TCB_Cfg[2]->context);
        //pxAdr = TCB_Cfg[2]->taskFxn;
        //init_context();
    //} else if (i == 2) {
        //save_context();
        //pxCurrentTCB = &(TCB_Cfg[1]->context);
        //pxAdr = TCB_Cfg[1]->taskFxn;
        //restore_context();
    //} else if (i == 128) {
        //save_context();
        //pxCurrentTCB = &(TCB_Cfg[2]->context);
        //pxAdr = TCB_Cfg[2]->taskFxn;
        //restore_context();
    //}
    //if (i < 255)
        //i++;
    //else 
        //i = 2;
    //
    //
    //asm volatile("ret");
}

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