/**
 * @file
 *
 * @brief       User application code
 *
 * This file contains code provided by the user.
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "assert.h"

#include "OCB.h"
#include "Task.h"
#include "Resource.h"
#include "Events.h"
#include "Counter.h"
#include "Alarm.h"

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

    OS_ActivateTask(T6);

    if (t == 0) {
        StatusType stat;
        for (uint64_t i = 0; i < 10; i++) {
            stat = Counter_IncrementCounter(C1);
            assert(stat == E_OK);
        }

        TickType tick = 0;
        TickType eTick = 0;
        stat = Counter_GetCounterValue(C1, &tick);
        assert(stat == E_OK && tick == 10);
        stat = Counter_IncrementCounter(C1);
        assert(stat == E_OK);
        stat = Counter_GetCounterValue(C1, &tick);
        assert(stat == E_OK && tick == 0);

        stat = Alarm_SetAbsAlarm(Alarm9, 3, 5);
        assert(stat == E_OK);
        stat = Alarm_GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 3);

        for (uint64_t i = 0; i < 5; i++) {
            stat = Counter_IncrementCounter(C1);
            assert(stat == E_OK);
        }

        tick = 0;
        stat = Counter_GetElapsedValue(C1, &tick, &eTick);
        assert(stat == E_OK && tick == 5 && eTick == 5);

        stat = Alarm_GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 3);

        stat = Alarm_CancelAlarm(Alarm9);
        assert(stat == E_OK);
        stat = Alarm_SetAbsAlarm(Alarm9, 1, 3);
        assert(stat == E_OK);
        stat = Alarm_GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 7);

        for (uint64_t i = 0; i < 7; i++) {
            stat = Counter_IncrementCounter(C1);
            assert(stat == E_OK);
        }

        tick = 5;
        stat = Counter_GetElapsedValue(C1, &tick, &eTick);
        assert(stat == E_OK && tick == 1 && eTick == 7);

        stat = Alarm_GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 3);

        stat = Alarm_CancelAlarm(Alarm9);
        assert(stat == E_OK);

        AlarmBaseType info;
        Alarm_GetAlarmBase(Alarm5, &info);
        assert(info.type == HARDWARE);
        assert(info.maxallowedvalue == UINT32_MAX);
        assert(info.mincycle == 1);
        assert(info.ticksperbase == 57);
        assert(info.secondspertick == 0.017778);
    }

    while (1) {
        StatusType stat;

        /* These tests should succeed */
        stat = OS_GetResource(Res1);
        assert(stat == E_OK);
        stat = OS_GetResource(Res2);
        assert(stat == E_OK);
        stat = OS_GetResource(Res3);
        assert(stat == E_OK);
        /* Request resource again => should fail */
        stat = OS_GetResource(Res3);
        assert(stat == E_OS_ACCESS);
        /* Request resource with ceiling priority below static task priority => should fail */
        stat = OS_GetResource(Res4);
        assert(stat == E_OS_ACCESS);
        /* Request resource with invalid ID => should fail */
        stat = OS_GetResource(64);
        assert(stat == E_OS_ID);

        /* These tests should succeed */
        stat = OS_ReleaseResource(Res3);
        assert(stat == E_OK);
        stat = OS_ReleaseResource(Res2);
        assert(stat == E_OK);
        stat = OS_ReleaseResource(Res1);
        assert(stat == E_OK);
        /* Release same resource again => should fail */
        stat = OS_ReleaseResource(Res1);
        assert(stat == E_OS_NOFUNC);


        PORTB &= ~(1 << 2);   // turn LED on
        _delay_ms(1000);
        PORTB |= (1 << 2);  // turn LED off
        _delay_ms(1000);
        if (t++ % 3 == 0) {
            OS_ActivateTask(T3);
        } else if (t == 20) {
            OS_ChainTask(T2);
        } else if (t == 30) {
            OS_TerminateTask();
        }
    }
}

TASK(T3)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 3);   // turn LED on
        _delay_ms(1000);

        Events_WaitEvent(0x01);
        EventMaskType ev = 0;
        Events_GetEvent(T3, &ev);
        assert(ev == 0x01);
        Events_ClearEvent(0x01);

        PORTB |= (1 << 3);  // turn LED off
        _delay_ms(1000);

        Task_Schedule();
    }

    enum tasks_e taskID = INVALID_TASK;
    OS_GetTaskID(&taskID);
    assert(taskID == T3);

    OsTaskState state = SUSPENDED;
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

    if (OS_ChainTask(T5) != E_OK) {
        OS_TerminateTask();
    }
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

TASK(T6)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 6);   // turn LED on
        _delay_ms(1000);
        PORTB |= (1 << 6);  // turn LED off
        _delay_ms(1000);
    }

    OS_TerminateTask();
}

TASK(T7)
{
    Alarm_CancelAlarm(Alarm3);

    StatusType stat = Alarm_SetAbsAlarm(Alarm3, 10, 0);
    assert(stat == E_OK);

    Events_WaitEvent(0x01);
    EventMaskType ev = 0;
    Events_GetEvent(T7, &ev);
    assert(ev == 0x01);
    Events_ClearEvent(0x01);

    stat = Alarm_SetAbsAlarm(Alarm3, 15, 5);
    assert(stat == E_OK);

    Events_WaitEvent(0x01);
    ev = 0;
    Events_GetEvent(T7, &ev);
    assert(ev == 0x01);
    Events_ClearEvent(0x01);

    Events_WaitEvent(0x01);
    ev = 0;
    Events_GetEvent(T7, &ev);
    assert(ev == 0x01);
    Events_ClearEvent(0x01);

    stat = Alarm_CancelAlarm(Alarm3);
    assert(stat == E_OK);

    stat = Alarm_SetRelAlarm(Alarm8, 6840, 0);
    assert(stat == E_OK);

    Events_WaitEvent(0x02);
    ev = 0;
    Events_GetEvent(T7, &ev);
    assert(ev == 0x02);
    Events_ClearEvent(0x02);

    stat = Alarm_SetRelAlarm(Alarm3, 3, 0);
    assert(stat == E_OK);

    for (uint8_t i = 0; i < 5; i++) {
        PORTB &= ~(1 << 7);   // turn LED on
        _delay_ms(1000);

        Events_WaitEvent(0x01);
        ev = 0;
        Events_GetEvent(T7, &ev);
        assert(ev == 0x01);
        Events_ClearEvent(0x01);

        stat = Alarm_CancelAlarm(Alarm3);
        stat = Alarm_SetRelAlarm(Alarm3, 10, 15);
        assert(stat == E_OK);

        PORTB |= (1 << 7);  // turn LED off
        _delay_ms(1000);

        Events_WaitEvent(0x01);
        ev = 0;
        Events_GetEvent(T7, &ev);
        assert(ev == 0x01);
        Events_ClearEvent(0x01);
    }

    OS_TerminateTask();
}

extern void StartupHook()
{
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off

    DDRD  = 0x00;   // PD as input
    PORTD = 0xFF;   // enable PU on PD

#if defined (__AVR_ATmega32__)
    GICR  = 1 << INT0 | 1 << INT1;                              // Enable INT0 and INT1
    MCUCR = 1 << ISC01 | 0 << ISC00 | 1 << ISC11 | 0 << ISC10;  // Trigger INT0 and INT1 on falling edge
#elif defined (__AVR_ATmega128__)
    EICRA = 1 << ISC11 | 1 << ISC10 | 1 << ISC01 | 1 << ISC00;  // Trigger INT0 and INT1 on falling edge
    EIMSK |= 1 << INT1 | 1 << INT0;                             // Enable INT0 and INT1

    /* Reset counter 1 */
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 14400;                                              // Set compare-match value for 1Hz
    TCCR1B |= (1 << WGM12);                                     // Enable CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);                        // Set prescaler to 1024
    TIMSK |= (1 << OCIE1A);                                     // Enable interrupt on compare match
#endif

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
    assert(isISR && isCat2ISR);
    OS_ActivateTask(T4);

    OS_GetResource(Res1);
    OS_GetResource(Res2);
    OS_GetResource(Res3);

    OS_ReleaseResource(Res3);
    OS_ReleaseResource(Res2);
    OS_ReleaseResource(Res1);
}

ISR(INT1_vect)
{
    assert(isISR && isCat2ISR);

    Events_SetEvent(T3, 0b11);
}

ISR(TIMER1_COMPA_vect)
{
    Counter_IncrementCounter(C2);
}

ALARMCALLBACK(AlarmCb)
{
    OS_ActivateTask(T7);
}