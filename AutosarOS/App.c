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

#include "OS_API.h"

#include <avr/io.h>
#include <util/delay.h>

#if defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true
#define DELAY_MS(ms)
#else
#define DELAY_MS(ms)    _delay_ms(ms)
#endif /* defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true */

TASK(Idle)
{
    while (1);
}

TASK(T1)
{
    while (1) {
        PORTB &= ~(1 << 1);   // turn LED on
        DELAY_MS(1000);
        PORTB |= (1 << 1);  // turn LED off
        DELAY_MS(1000);

        StatusType stat = IncrementCounter(C7);
        assert(stat == E_OK);
    }
}

TASK(T2)
{
    static uint8_t t = 0;

    ActivateTask(T6);

    SetRelAlarm((AlarmType) 128, 123, 321);

    AppModeType mode = GetActiveApplicationMode();
    assert(mode == OSDEFAULTAPPMODE);

    SuspendAllInterrupts();
    ResumeAllInterrupts();

    SuspendOSInterrupts();
    ResumeOSInterrupts();

    if (t == 0) {
        StatusType stat;
        for (uint64_t i = 0; i < 10; i++) {
            stat = IncrementCounter(C1);
            assert(stat == E_OK);
        }

        TickType tick = 0;
        TickType eTick = 0;
        stat = GetCounterValue(C1, &tick);
        assert(stat == E_OK && tick == 10);
        stat = IncrementCounter(C1);
        assert(stat == E_OK);
        stat = GetCounterValue(C1, &tick);
        assert(stat == E_OK && tick == 0);

        stat = SetAbsAlarm(Alarm9, 3, 5);
        assert(stat == E_OK);
        stat = GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 3);

        for (uint64_t i = 0; i < 5; i++) {
            stat = IncrementCounter(C1);
            assert(stat == E_OK);
        }

        tick = 0;
        stat = GetElapsedValue(C1, &tick, &eTick);
        assert(stat == E_OK && tick == 5 && eTick == 5);

        stat = GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 3);

        stat = CancelAlarm(Alarm9);
        assert(stat == E_OK);
        stat = SetAbsAlarm(Alarm9, 1, 3);
        assert(stat == E_OK);
        stat = GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 7);

        for (uint64_t i = 0; i < 7; i++) {
            stat = IncrementCounter(C1);
            assert(stat == E_OK);
        }

        tick = 5;
        stat = GetElapsedValue(C1, &tick, &eTick);
        assert(stat == E_OK && tick == 1 && eTick == 7);

        stat = GetAlarm(Alarm9, &tick);
        assert(stat == E_OK && tick == 3);

        stat = CancelAlarm(Alarm9);
        assert(stat == E_OK);

        AlarmBaseType info;
        GetAlarmBase(Alarm5, &info);
        assert(info.type == HARDWARE);
        assert(info.maxallowedvalue == UINT32_MAX);
        assert(info.mincycle == 1);
        assert(info.ticksperbase == 57);
        assert(info.secondspertick == 0.017778);
    }

    while (1) {
        StatusType stat;

        /* These tests should succeed */
        stat = GetResource(Res1);
        assert(stat == E_OK);
        stat = GetResource(Res2);
        assert(stat == E_OK);
        stat = GetResource(Res3);
        assert(stat == E_OK);
        /* Request resource again => should fail */
        stat = GetResource(Res3);
        assert(stat == E_OS_ACCESS);
        /* Request resource with ceiling priority below static task priority => should fail */
        stat = GetResource(Res4);
        assert(stat == E_OS_ACCESS);
        /* Request resource with invalid ID => should fail */
        stat = GetResource(64);
        assert(stat == E_OS_ID);

        /* These tests should succeed */
        stat = ReleaseResource(Res3);
        assert(stat == E_OK);
        stat = ReleaseResource(Res2);
        assert(stat == E_OK);
        stat = ReleaseResource(Res1);
        assert(stat == E_OK);
        /* Release same resource again => should fail */
        stat = ReleaseResource(Res1);
        assert(stat == E_OS_NOFUNC);


        PORTB &= ~(1 << 2);   // turn LED on
        DELAY_MS(1000);
        PORTB |= (1 << 2);  // turn LED off
        DELAY_MS(1000);
        if (t++ % 3 == 0) {
            ActivateTask(T3);
        } else if (t == 20) {
            ChainTask(T2);
        } else if (t == 30) {
            TerminateTask();
        }
    }
}

TASK(T3)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 3);   // turn LED on
        DELAY_MS(1000);

        WaitEvent(0x01);
        EventMaskType ev = 0;
        GetEvent(T3, &ev);
        assert(ev == 0x01);
        ClearEvent(0x01);

        PORTB |= (1 << 3);  // turn LED off
        DELAY_MS(1000);

        Schedule();
    }

    TaskType taskID = INVALID_TASK;
    GetTaskID(&taskID);
    assert(taskID == T3);

    TaskStateType state = SUSPENDED;
    GetTaskState(T3, &state);
    assert(state == RUNNING);

    TerminateTask();
}

TASK(T4)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 4);   // turn LED on
        DELAY_MS(1000);
        PORTB |= (1 << 4);  // turn LED off
        DELAY_MS(1000);
    }

    if (ChainTask(T5) != E_OK) {
        TerminateTask();
    }
}

TASK(T5)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 5);   // turn LED on
        DELAY_MS(1000);
        PORTB |= (1 << 5);  // turn LED off
        DELAY_MS(1000);
    }

    TerminateTask();
}

TASK(T6)
{
    for (uint8_t i = 0; i < 3; i++) {
        PORTB &= ~(1 << 6);   // turn LED on
        DELAY_MS(1000);
        PORTB |= (1 << 6);  // turn LED off
        DELAY_MS(1000);
    }

    TerminateTask();
}

TASK(T7)
{
    CancelAlarm(Alarm3);

    StatusType stat = SetAbsAlarm(Alarm3, 10, 0);
    assert(stat == E_OK);

    WaitEvent(0x01);
    EventMaskType ev = 0;
    GetEvent(T7, &ev);
    assert(ev == 0x01);
    ClearEvent(0x01);

    stat = SetAbsAlarm(Alarm3, 15, 5);
    assert(stat == E_OK);

    WaitEvent(0x01);
    ev = 0;
    GetEvent(T7, &ev);
    assert(ev == 0x01);
    ClearEvent(0x01);

    WaitEvent(0x01);
    ev = 0;
    GetEvent(T7, &ev);
    assert(ev == 0x01);
    ClearEvent(0x01);

    stat = CancelAlarm(Alarm3);
    assert(stat == E_OK);

    stat = SetRelAlarm(Alarm8, 6840, 0);
    assert(stat == E_OK);

    WaitEvent(0x02);
    ev = 0;
    GetEvent(T7, &ev);
    assert(ev == 0x02);
    ClearEvent(0x02);

    stat = SetRelAlarm(Alarm3, 3, 0);
    assert(stat == E_OK);

    for (uint8_t i = 0; i < 5; i++) {
        PORTB &= ~(1 << 7);   // turn LED on
        DELAY_MS(1000);

        WaitEvent(0x01);
        ev = 0;
        GetEvent(T7, &ev);
        assert(ev == 0x01);
        ClearEvent(0x01);

        stat = CancelAlarm(Alarm3);
        stat = SetRelAlarm(Alarm3, 10, 15);
        assert(stat == E_OK);

        PORTB |= (1 << 7);  // turn LED off
        DELAY_MS(1000);

        WaitEvent(0x01);
        ev = 0;
        GetEvent(T7, &ev);
        assert(ev == 0x01);
        ClearEvent(0x01);
    }

    TerminateTask();
}

TASK(T8)
{
    static TickType tick = 0;
    static TickType currentTick = 0;
    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    WaitEvent(0b01);
    EventMaskType ev = 0;
    GetEvent(T8, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T8, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    tick += 5;
    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 5;
    tick += 3;

    TerminateTask();
}

TASK(T9)
{
    static TickType tick = 0;
    static TickType currentTick = 0;
    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    WaitEvent(0b01);
    EventMaskType ev = 0;
    GetEvent(T9, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T9, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    tick += 10;
    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 3;

    TerminateTask();
}

TASK(T10)
{
    static TickType tick = 0;
    static TickType currentTick = 0;
    StatusType stat;

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    /* Enable ST2 */
    stat = StartScheduleTableRel(ST2, 3);
    assert(stat == E_OK);

    /* Running ST2 */

    tick += 3;

    WaitEvent(0b01);
    EventMaskType ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 5;

    /* Queue ST3 */
    stat = NextScheduleTable(ST2, ST3);
    assert(stat == E_OK);

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 3;

    /* Switch to ST3 */

    tick += 1;

    WaitEvent(0b01);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 2;

    /* Queue ST2 */
    stat = NextScheduleTable(ST3, ST2);
    assert(stat == E_OK);

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 5;

    /* Back to ST2 */

    WaitEvent(0b01);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 5;

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 3;

    /* Enable ST3 */
    currentTick += 5;
    currentTick = currentTick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    stat = StartScheduleTableAbs(ST3, currentTick);
    assert(stat == E_OK);

    tick += 2;

    /* Back to ST3 */

    tick += 1;

    WaitEvent(0b01);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 2;

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    //tick += 5;

    /* Enable ST2 */
    tick += Counter_Cfg[C5]->maxallowedvalue;
    stat = StartScheduleTableAbs(ST2, currentTick);
    assert(stat == E_OK);

    /* Back to ST2 */

    WaitEvent(0b01);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    tick += 5;

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    tick = tick % (Counter_Cfg[C5]->maxallowedvalue + 1);
    GetCounterValue(C5, &currentTick);
    assert(tick == currentTick);

    //tick += 3;

    //GetCounterValue(C5, &currentTick);
    //stat = StartScheduleTableAbs(ST3, currentTick);
    //assert(stat == E_OK);
    //

    /* Start ST4 */
    stat = StartScheduleTableRel(ST4, 50);
    assert(stat == E_OK);

    /* Running ST4 */

    WaitEvent(0b01);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    /* Start ST5 */
    stat = StartScheduleTableRel(ST5, 5);
    assert(stat == E_OK);

    /* Running ST5 */

    WaitEvent(0b01);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    /* Start ST6 */
    stat = StartScheduleTableRel(ST6, 1);
    assert(stat == E_OK);

    /* Running ST6 */

    WaitEvent(0b01);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b01);
    ClearEvent(0b01);

    WaitEvent(0b10);
    ev = 0;
    GetEvent(T10, &ev);
    assert(ev == 0b10);
    ClearEvent(0b10);

    stat = StopScheduleTable(ST6);
    assert(stat == E_OK);

    ScheduleTableStatusType status;
    stat = GetScheduleTableStatus(ST6, &status);
    assert(stat == E_OK && status == SCHEDULETABLE_STOPPED);

    TerminateTask();
}

extern void StartupHook(void)
{
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off

    DDRD  = 0x00;   // PD as input
    PORTD = 0xFF;   // enable PU on PD

#if defined (__AVR_ATmega32__)
    GICR  = 1 << INT0 | 1 << INT1;                              // Enable INT0 and INT1
    MCUCR = 1 << ISC01 | 0 << ISC00 | 1 << ISC11 | 0 << ISC10;  // Trigger INT0 and INT1 on falling edge
#elif defined (__AVR_ATmega128__) || defined (__AVR_ATmega1284__)
    EICRA = 1 << ISC11 | 0 << ISC10 | 1 << ISC01 | 0 << ISC00;  // Trigger INT0 and INT1 on falling edge
    EIMSK |= 1 << INT1 | 1 << INT0;                             // Enable INT0 and INT1

    /* Timer 1 */
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 14400;                                              // Set compare-match value for 1Hz
    TCCR1B |= (1 << WGM12);                                     // Enable CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);                        // Set prescaler to 1024

#if defined (__AVR_ATmega128__)
    TIMSK |= (1 << OCIE1A);                                     // Enable interrupt on compare match
#else /* #if defined (__AVR_ATmega128__) */
    TIMSK1 |= (1 << OCIE1A);                                    // Enable interrupt on compare match
#endif /* #if defined (__AVR_ATmega128__) */

    /* Timer 2 */
#if defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true
#if defined (__AVR_ATmega128__)
    TCCR2 = (1 << CS20);                                        // Enable Timer2 with Prescaler 1
    TIMSK |= 1 << TOIE2;                                       // Enable Overflow Interrupt (Timer2)
#else /* #if defined (__AVR_ATmega128__) */
    TCCR2B = (1 << CS20);                                       // Enable Timer2 with Prescaler 1
    TIMSK2 |= 1 << TOIE2;                                       // Enable Overflow Interrupt (Timer2)
#endif /* #if defined (__AVR_ATmega128__) */
#endif /* defined (OS_CONFIG_SIM) && OS_CONFIG_SIM == true */
#else
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
}

extern void PostTaskHook(void)
{
    TaskType task;
    GetTaskID(&task);
}

extern void ErrorHook(void)
{
    //OSServiceIdType id = OSErrorGetServiceId();
    //AlarmType alarm = OSError_SetRelAlarm_AlarmID();
    //TickType inc = OSError_SetRelAlarm_increment();
    //TickType cyc = OSError_SetRelAlarm_cycle();

    //assert(id == OSServiceId_SetRelAlarm);
    //assert(alarm == 128);
    //assert(inc == 123);
    //assert(cyc == 321);
}

ISR(INT0_vect)
{
    assert(isISR && isCat2ISR);
    ActivateTask(T4);

    GetResource(Res1);
    GetResource(Res2);
    GetResource(Res3);

    ReleaseResource(Res3);
    ReleaseResource(Res2);
    ReleaseResource(Res1);
}

ISR(INT1_vect)
{
    assert(isISR && isCat2ISR);

    SetEvent(T3, 0b11);
}

ISR(TIMER1_COMPA_vect)
{
    IncrementCounter(C2);
    IncrementCounter(C6);
}

ALARMCALLBACK(AlarmCb)
{
    ActivateTask(T7);
}

ISR(TIMER2_OVF_vect)
{

}