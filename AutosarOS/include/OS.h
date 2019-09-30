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
extern void __attribute__((naked)) OS_Schedule();
extern void OS_Switch();

// TODO Docs and check name conformity with OSEK/Autosar
extern void EnableAllInterrupts();
extern void DisableAllInterrupts();

#ifdef STARTUPHOOK
/**
 * @brief   Hook function called after Startup of the OS
 */
extern void STARTUPHOOK();
#endif


#endif /* OS_H_ */