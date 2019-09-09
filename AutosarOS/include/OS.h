/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 


#ifndef OS_H_
#define OS_H_

extern enum tasks_e currentTask;


// TODO: Return type
// TODO: Application mode
extern void OS_StartOS();

// TODO: Return type
extern void OS_Schedule();

// TODO
extern void OS_Dispatch();

// TODO Docs and check name conformity with OSEK/Autosar
extern void EnableAllInterrupts();
extern void DisableAllInterrupts();

extern void StartupHook();


#endif /* OS_H_ */