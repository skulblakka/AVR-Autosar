/**
 * @file
 *
 * @brief       Application code for hook test procedure 1
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

volatile bool startupHooksCalled = false;
volatile bool errorHookCalled = false;

TASK(Idle)
{
    assert(startupHooksCalled == true);

    StatusType stat = ActivateTask(Task1);
    assert(stat == E_OK);

    while (1);
}

TASK(Task1)
{
    StatusType stat = ActivateTask(Task2);
    assert(stat == E_OK);

    stat = Schedule();
    assert(stat == E_OK);

    stat = TerminateTask();
    assert(stat == E_OK);
}

TASK(Task2)
{
    assert(errorHookCalled == false);

    TickType tick = 0;
    StatusType stat = GetAlarm(Alarm1, &tick);
    assert(stat == E_OS_NOFUNC && errorHookCalled == true);

    stat = TerminateTask();
    assert(stat == E_OK);
}

extern void StartupHook(void)
{
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off

    DDRD  = 0x00;   // PD as input
    PORTD = 0xFF;   // enable PU on PD

#if defined (__AVR_ATmega32__)

#elif defined (__AVR_ATmega128__) || defined (__AVR_ATmega1284__)
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

    startupHooksCalled = true;
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
    static uint8_t count = 0;

    if (count == 1) {
        TaskType task;
        StatusType stat = GetTaskID(&task);
        assert(stat == E_OK && task == Task1);

        TaskStateType state = SUSPENDED;
        stat = GetTaskState(Task1, &state);
        assert(stat == E_OK && state == RUNNING);
        stat = GetTaskState(Task2, &state);
        assert(stat == E_OK && state == SUSPENDED);
    } else if (count == 2) {
        TaskType task;
        StatusType stat = GetTaskID(&task);
        assert(stat == E_OK && task == Task2);

        TaskStateType state = SUSPENDED;
        stat = GetTaskState(Task1, &state);
        assert(stat == E_OK && state == READY);
        stat = GetTaskState(Task2, &state);
        assert(stat == E_OK && state == RUNNING);
    } else if (count == 3) {
        TaskType task;
        StatusType stat = GetTaskID(&task);
        assert(stat == E_OK && task == Task1);

        TaskStateType state = SUSPENDED;
        stat = GetTaskState(Task1, &state);
        assert(stat == E_OK && state == RUNNING);
        stat = GetTaskState(Task2, &state);
        assert(stat == E_OK && state == SUSPENDED);
    }

    count += 1;
}

extern void PostTaskHook(void)
{
    static uint8_t count = 0;

    if (count == 1) {
        TaskType task;
        StatusType stat = GetTaskID(&task);
        assert(stat == E_OK && task == Task1);

        TaskStateType state = SUSPENDED;
        stat = GetTaskState(Task1, &state);
        assert(stat == E_OK && state == RUNNING);
        stat = GetTaskState(Task2, &state);
        assert(stat == E_OK && state == READY);
    } else if (count == 2) {
        TaskType task;
        StatusType stat = GetTaskID(&task);
        assert(stat == E_OK && task == Task2);

        TaskStateType state = SUSPENDED;
        stat = GetTaskState(Task1, &state);
        assert(stat == E_OK && state == READY);
        stat = GetTaskState(Task2, &state);
        assert(stat == E_OK && state == RUNNING);
    } else if (count == 3) {
        TaskType task;
        StatusType stat = GetTaskID(&task);
        assert(stat == E_OK && task == Task1);

        TaskStateType state = SUSPENDED;
        stat = GetTaskState(Task1, &state);
        assert(stat == E_OK && state == RUNNING);
        stat = GetTaskState(Task2, &state);
        assert(stat == E_OK && state == SUSPENDED);
    }

    count += 1;
}

extern void ErrorHook(StatusType error)
{
    errorHookCalled = true;
}

ISR(TIMER2_OVF_vect)
{

}