/*
 * context.h
 *
 * Created: 05.08.2019 14:35:27
 *  Author: Pascal Romahn
 */ 


#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Task.h"

extern void save_context();
extern void restore_context();
extern void init_context();

extern uint8_t** pxCurrentTCB;
extern pTaskFxn pxAdr;

extern volatile uint32_t sysTick;

#endif /* CONTEXT_H_ */