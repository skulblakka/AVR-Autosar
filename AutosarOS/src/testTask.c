/*
 * testTask.c
 *
 * Created: 05.08.2019 16:41:20
 *  Author: Pascal Romahn
 */ 

#include "Task.h"
#include "context.h"

#include <avr/io.h>
#include <util/delay.h>

#define OS_CONFIG_GEN_ENUM
#include "AppCfg.h"
#undef OS_CONFIG_GEN_ENUM

#define OS_CONFIG_GEN_FUNC_DECL
#include "AppCfg.h"
#undef OS_CONFIG_GEN_FUNC_DECL

#define OS_CONFIG_GEN_DATA_STRUCT
#include "AppCfg.h"
#undef OS_CONFIG_GEN_DATA_STRUCT

#define OS_CONFIG_GEN_TCB
#include "AppCfg.h"
#undef OS_CONFIG_GEN_TCB

static volatile uint32_t delay = 1000;

volatile uint8_t* pxAdr;
volatile uint8_t** pxCurrentTCB;


TASK(Idle) 
{
    while (1);
}

TASK(T1)
{
    while (1) {
        PORTB &= ~(1 << 1);   // turn LED on
        _delay_ms(delay);
        PORTB |= (1 << 1);  // turn LED off
        _delay_ms(delay);
    }        
}

TASK(T2)
{
    while (1) {
        PORTB &= ~(1 << 2);   // turn LED on
        _delay_ms(delay);
        PORTB |= (1 << 2);  // turn LED off
        _delay_ms(delay);
    }
}

TASK(T3)
{
    while(1);
}

void __attribute__((naked)) schedule()
{
    //TCB_Cfg[1]->taskFxn();
    static uint8_t i = 0;
    if (i == 0) {
        pxCurrentTCB = &(TCB_Cfg[1]->context);
        pxAdr = TCB_Cfg[1]->taskFxn;
        init_context();
    } else if (i == 1) {
        save_context();
        pxCurrentTCB = &(TCB_Cfg[2]->context);
        pxAdr = TCB_Cfg[2]->taskFxn;
        init_context();
    } else if (i == 2) {
        save_context();
        pxCurrentTCB = &(TCB_Cfg[1]->context);
        pxAdr = TCB_Cfg[1]->taskFxn;
        restore_context();
    } else if (i == 128) {
        save_context();
        pxCurrentTCB = &(TCB_Cfg[2]->context);
        pxAdr = TCB_Cfg[2]->taskFxn;
        restore_context();
    }
    if (i < 255)
        i++;
    else 
        i = 2;
    
    
    asm volatile("ret");
}