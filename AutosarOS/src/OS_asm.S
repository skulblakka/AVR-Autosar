/**
 * @file
 *
 * @brief       Assembler implementation for OS
 *
 * @date        2020-06-17
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include <avr/io.h>

.section .text

.global OS_Schedule
OS_Schedule:
    /* Push r24 and SREG */
    push    r24
    in      r24, _SFR_IO_ADDR(SREG)
    push    r24

    /* Test if scheduling is blocked */
    lds     r24, blockScheduling
    tst     r24
    brne    blocked

    /* Test if scheduling blocked by ISR */
    lds     r24, isISR
    tst     r24
    brne    blocked

    /* Restore r24 and SREG */
    pop     r24
    out     _SFR_IO_ADDR(SREG), r24
    pop     r24

    call    OS_ScheduleC

    ret

blocked:
    /* Set needScheduling flag */
    ldi     r24, 1
    sts     needScheduling, r24

    /* Restore r24 and SREG */
    pop     r24
    out     _SFR_IO_ADDR(SREG), r24
    pop     r24

    ret