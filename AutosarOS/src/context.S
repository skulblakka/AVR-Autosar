
/**
 * @file
 *
 * @brief       Context switching
 *
 * @date        2019-08-05
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include <avr/io.h>

.section .bss
#if defined(__AVR_3_BYTE_PC__)
retAddr:    .skip 3
#else /* defined(__AVR_3_BYTE_PC__) */
retAddr:    .skip 2
#endif /* defined(__AVR_3_BYTE_PC__) */
tempReg:    .skip 1

.global needScheduling
.global sysTick


.section .text

.global save_context
save_context:
    sts     tempReg, r0                 // Save r0

    /* Get return address */
    pop     r0
    sts     retAddr, r0
    pop     r0
    sts     retAddr + 1, r0
#if defined(__AVR_3_BYTE_PC__)
    pop     r0
    sts     retAddr + 2, r0
#endif /* defined(__AVR_3_BYTE_PC__) */

    lds r0, tempReg                     // Restore r0

    push    r0                          // Push R0 before overwrite with SREG
    in      r0, _SFR_IO_ADDR(SREG)
    cli                                 // Disable interrupts now that we saved SREG
    push    r0                          // Push SREG
    push    r1
    clr     r1                          // Compiler generated code assumes R1 is set zero
    push    r2
    push    r3
    push    r4
    push    r5
    push    r6
    push    r7
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
    push    r16
    push    r17
    push    r18
    push    r19
    push    r20
    push    r21
    push    r22
    push    r23
    push    r24
    push    r25
    push    r26
    push    r27
    push    r28
    push    r29
    push    r30
    push    r31
    lds     r26, ptrCurrentStack        // Load address for storing current SP
    lds     r27, ptrCurrentStack + 1
    in      r0, _SFR_IO_ADDR(SPL)       // Store SP
    st      x+, r0
    in      r0, _SFR_IO_ADDR(SPH)
    st      x+, r0

    /* Push return address */
#if defined(__AVR_3_BYTE_PC__)
    lds     r0, retAddr + 2
    push    r0
#endif /* defined(__AVR_3_BYTE_PC__) */
    lds     r0, retAddr + 1
    push    r0
    lds     r0, retAddr
    push    r0

    ret


.global restore_context
restore_context:
    /* Get return address */
    pop     r0
    sts     retAddr, r0
    pop     r0
    sts     retAddr + 1, r0
#if defined(__AVR_3_BYTE_PC__)
    pop     r0
    sts     retAddr + 2, r0
#endif /* defined(__AVR_3_BYTE_PC__) */

    lds     r26, ptrCurrentStack        // Load address containing SP
    lds     r27, ptrCurrentStack + 1
    ld      r28, x+                     // Load SP
    ld      r29, x+
    out     _SFR_IO_ADDR(SPL), r28      // Restore SP
    out     _SFR_IO_ADDR(SPH), r29

    pop     r31
    pop     r30
    pop     r29
    pop     r28
    pop     r27
    pop     r26
    pop     r25
    pop     r24
    pop     r23
    pop     r22
    pop     r21
    pop     r20
    pop     r19
    pop     r18
    pop     r17
    pop     r16
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     r7
    pop     r6
    pop     r5
    pop     r4
    pop     r3
    pop     r2
    pop     r1
    pop     r0                          // Status reg was stored between r1 and r0
    out     _SFR_IO_ADDR(SREG), r0      // Restore status reg
    pop     r0

    sts     tempReg, r0                 // Save r0

    /* Push return address */
#if defined(__AVR_3_BYTE_PC__)
    lds     r0, retAddr + 2
    push    r0
#endif /* defined(__AVR_3_BYTE_PC__) */
    lds     r0, retAddr + 1
    push    r0
    lds     r0, retAddr
    push    r0

    lds     r0, tempReg                 // Restore r0

    ret


.global init_context
init_context:
    /* Get return address */
    pop     r21
    pop     r22
#if defined(__AVR_3_BYTE_PC__)
    pop     r23
#endif /* defined(__AVR_3_BYTE_PC__) */

    /* Switch to new task stack */
    lds     r26, ptrCurrentStack        // Load new SP address
    lds     r27, ptrCurrentStack + 1
    ld      r24, x+
    ld      r25, x
    out     _SFR_IO_ADDR(SPL), r24      // Change to new SP
    out     _SFR_IO_ADDR(SPH), r25

    clr     r1                          // Make sure R1 is cleared

    /* Place return address to taskFxn */
    lds     r24, ptrCurrentFxnAddr      // Load address of taskFxn
    lds     r25, ptrCurrentFxnAddr + 1
    push    r24                         // Push "return" to taskFxn
    push    r25
#if defined(__AVR_3_BYTE_PC__)
	push    r1                          // Push extra byte
#endif /* defined(__AVR_3_BYTE_PC__) */

    /* Push return address */
#if defined(__AVR_3_BYTE_PC__)
    push    r23
#endif /* defined(__AVR_3_BYTE_PC__) */
    push    r22
    push    r21

    ret


.global TIMER0_OVF_vect
TIMER0_OVF_vect:
    /* Push r0 and SREG */
    push    r0
    in      r0, _SFR_IO_ADDR(SREG)
    push    r0

    /* Push work registers r24 to r27 */
    push    r24
    push    r25
    push    r26
    push    r27

    /* Load sysTick (uint32_t) */
    lds     r24, sysTick
    lds     r25, sysTick + 1
    lds     r26, sysTick + 2
    lds     r27, sysTick + 3

    /* Increment sysTick by 1 */
    adiw    r24, 1
    adc     r26, r1
    adc     r27, r1

    /* Store sysTick */
    sts     sysTick, r24
    sts     sysTick + 1, r25
    sts     sysTick + 2, r26
    sts     sysTick + 3, r27

    /* Restore work registers r24 to r27 */
    pop     r27
    pop     r26
    pop     r25
    pop     r24

    /* Check if sysTick change must be handled */
    lds     r0, needSysTickEval
    tst     r0
    breq    noSysTickEval

    /* Restore r0 and SREG */
    pop     r0
    out     _SFR_IO_ADDR(SREG), r0
    pop     r0

    /* Save current context */
    call    save_context

    /* Set marker at stack top */
    ldi     r24, 0xBE
    sts     OS_SystemStack, r24

    /* Switch to system stack */
    lds     r24, OS_SystemStackPtr
    lds     r25, OS_SystemStackPtr + 1
    out     _SFR_IO_ADDR(SPL), r24      // Change to new SP
    out     _SFR_IO_ADDR(SPH), r25

    /* Block scheduling */
    ldi     r24, 1
    lds     r2, blockScheduling
    sts     blockScheduling, r24

    /* Call subsystem functions to handle sysTick */
    call    Alarm_evaluateSysTickAlarm
    call    ScheduleTable_handleSysTick

    /* Release scheduling */
    sts     blockScheduling, r2

    /* Check marker at stack top indicating overrun */
    lds     r24, OS_SystemStack
    cpi     r24, 0xBE
    breq    noError
    call    OS_ShutdownOSStackOverrun

noError:
    /* Restore context */
    call    restore_context

    /* Push r0 and SREG */
    push    r0
    in      r0, _SFR_IO_ADDR(SREG)
    push    r0

noSysTickEval:
    /* Test if rescheduling is needed */
    lds     r0, needScheduling
    tst     r0
    breq    noScheduling

    /* Restore r0 and SREG */
    pop     r0
    out     _SFR_IO_ADDR(SREG), r0
    pop     r0

    call OS_Schedule

    reti

noScheduling:
    /* Restore r0 and SREG */
    pop     r0
    out     _SFR_IO_ADDR(SREG), r0
    pop     r0

    reti