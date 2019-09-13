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
        if (t++ == 3) {
            OS_ActivateTask(T3);
        } else if (t == 10) {
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

    OS_TerminateTask();
}


extern void StartupHook()
{
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off
}