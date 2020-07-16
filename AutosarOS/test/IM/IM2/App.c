/**
 * @file
 *
 * @brief       Application code for interrupt management test procedure 2
 *
 * @date        2020-07-16
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "assert.h"

#include "OS_API.h"

#include <avr/io.h>
#include <util/delay.h>

#if defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true
#define DELAY_MS(ms)
#else
#define DELAY_MS(ms)    _delay_ms(ms)
#endif /* defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true */

volatile bool isrFlag = false;

TASK(Idle)
{
    while (1);
}

TASK(Task1)
{
    while (!isrFlag) {
        ;
    }

    StatusType stat = TerminateTask();
    assert(stat == E_OK);
}

TASK(Task2)
{
    StatusType stat = TerminateTask();
    assert(stat == E_OK);
}

extern void StartupHook(void)
{
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off

    DDRD  = 0x00;   // PD as input
    PORTD = 0xFF;   // enable PU on PD

#if defined (__AVR_ATmega32__)
    GICR  = 1 << INT0;                // Enable INT0 and INT1
    MCUCR = 1 << ISC01 | 0 << ISC00;  // Trigger INT0 and INT1 on falling edge
#elif defined (__AVR_ATmega128__) || defined (__AVR_ATmega1284__)
    EICRA = 1 << ISC01 | 0 << ISC00;  // Trigger INT0 and INT1 on falling edge
    EIMSK |= 1 << INT0;               // Enable INT0 and INT1

    /* Timer 2 */
#if defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true
#if defined (__AVR_ATmega128__)
    TCCR2 = (1 << CS20);                                        // Enable Timer2 with Prescaler 1
    TIMSK |= 1 << TOIE2;                                        // Enable Overflow Interrupt (Timer2)
#else /* defined (__AVR_ATmega128__) */
    TCCR2B = (1 << CS20);                                       // Enable Timer2 with Prescaler 1
    TIMSK2 |= 1 << TOIE2;                                       // Enable Overflow Interrupt (Timer2)
#endif /* defined (__AVR_ATmega128__) */
#endif /* defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true */
#else /* defined (__AVR_ATmega32__) */
#error Unknown CPU defined!
#endif /* defined (__AVR_ATmega32__) */

    uint8_t t = 0;

    while (t < 6) {
        uint8_t r = PORTB;
        r ^= (1 << 7);
        PORTB = r;
        DELAY_MS(50);
        t++;
    }
}

extern void ShutdownHook(StatusType error)
{
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off

    for (uint8_t i = 0; i < 11; i++) {
        PORTB ^= 0xFF;
        DELAY_MS(1000);
    }
}

extern void PreTaskHook(void)
{
    TaskType task;
    GetTaskID(&task);
    TaskStateType state = SUSPENDED;
    GetTaskState(task, &state);
}

extern void PostTaskHook(void)
{
    TaskType task;
    GetTaskID(&task);
    TaskStateType state = SUSPENDED;
    GetTaskState(task, &state);
}

extern void ErrorHook(void)
{

}

ISR(INT0_vect)
{
    assert(isISR && isCat2ISR);

    StatusType stat = ActivateTask(Task2);
    assert(stat == E_OK);

    isrFlag = true;
}

ISR(TIMER2_OVF_vect)
{

}