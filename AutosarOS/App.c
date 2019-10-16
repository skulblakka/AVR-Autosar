/**
 * @file
 *
 * @brief   User application code
 *
 * This file contains code provided by the user.
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */

#include "assert.h"

#include "OCB.h"
#include "Task.h"

#include <avr/io.h>
#include <util/delay.h>

TASK(Idle)
{
    while (1);
}

TASK(T1)
{
    while (1) {
        PORTB &= ~(1 << 1);   // turn LED on
        _delay_ms(1000);
        PORTB |= (1 << 1);  // turn LED off
        _delay_ms(1000);
    }
}

TASK(T2)
{
    static uint8_t t = 0;
    while (1) {
        PORTB &= ~(1 << 2);   // turn LED on
        _delay_ms(1000);
        PORTB |= (1 << 2);  // turn LED off
        _delay_ms(1000);
        if (t++ % 3 == 0) {
            OS_ActivateTask(T3);
        } else if (t == 20) {
            OS_TerminateTask();
        }
    }
}

TASK(T3)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 3);   // turn LED on
        _delay_ms(1000);
        PORTB |= (1 << 3);  // turn LED off
        _delay_ms(1000);
    }

    enum tasks_e taskID = INVALID_TASK;
    OS_GetTaskID(&taskID);
    assert(taskID == T3);

    OsTaskState state = SUSPENDED;
    OS_GetTaskState(T2, &state);
    assert(state == READY);
    OS_GetTaskState(T3, &state);
    assert(state == RUNNING);

    OS_TerminateTask();
}

TASK(T4)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 4);   // turn LED on
        _delay_ms(1000);
        PORTB |= (1 << 4);  // turn LED off
        _delay_ms(1000);
    }

    OS_TerminateTask();
}

TASK(T5)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 5);   // turn LED on
        _delay_ms(1000);
        PORTB |= (1 << 5);  // turn LED off
        _delay_ms(1000);
    }

    OS_TerminateTask();
}


extern void StartupHook()
{
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off

    DDRD  = 0x00;   // PD as input
    PORTD = 0xFF;   // enable PU on PD
    GICR  = 1 << INT0 | 1 << INT1;                              // Enable INT0 and INT1
    MCUCR = 1 << ISC01 | 0 << ISC00 | 1 << ISC11 | 0 << ISC10;  // Trigger INT0 and INT1 on falling edge

    uint8_t t = 0;

    while (t < 6) {
        uint8_t r = PORTB;
        r ^= (1 << 7);
        PORTB = r;
        _delay_ms(50);
        t++;
    }
}

ISR(INT0_vect)
{
    assert(isISR == true && isCat2ISR == true);
    OS_ActivateTask(T4);
}

ISR(INT1_vect)
{
    assert(isISR == true && isCat2ISR == true);
    OS_ActivateTask(T5);
}