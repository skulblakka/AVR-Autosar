/*
 * context.h
 *
 * Created: 05.08.2019 14:35:27
 *  Author: Pascal Romahn
 */ 


#ifndef CONTEXT_H_
#define CONTEXT_H_

extern void save_context();
extern void restore_context();
extern void init_context();

extern volatile uint8_t* pxCurrentTCB;
extern volatile uint8_t* pxAdr;



#endif /* CONTEXT_H_ */