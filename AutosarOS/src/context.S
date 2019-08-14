
/*
 * context.S
 *
 * Created: 05.08.2019 15:02:08
 *  Author: Pascal Romahn
 */ 

 #include <avr/io.h>

.section .text

.global save_context
save_context:
    push    r0                      // Push R0 before overwrite with SREG
    in      r0, _SFR_IO_ADDR(SREG)
    cli                             // Disable interrupts now that we saved SREG
    push    r0                      // Push SREG
    push    r1
    clr     r1                      // Compiler generated code assumes R1 is set zero
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
    lds     r26, pxCurrentTCB       // Load address for storing current SP
    lds     r27, pxCurrentTCB + 1
    in      r0, _SFR_IO_ADDR(SPL)   // Store SP
    st      x+, r0
    in      r0, _SFR_IO_ADDR(SPH)
    st      x+, r0


.global restore_context
restore_context:
    lds     r26, pxCurrentTCB       // Load address containing SP
    lds     r27, pxCurrentTCB + 1
    ld      r28, x+                 // Load SP
    ld      r29, x+
    out     _SFR_IO_ADDR(SPL), r28  // Restore SP
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
    pop     r0                      // Status reg was stored between r1 and r0
    out     _SFR_IO_ADDR(SREG), r0  // Restore status reg
    pop     r0


.global init_context
init_context:
    /* Get return address to schedule() */
    pop     r2
    pop     r3

    /* Get return address to ISR */
    pop     r4
    pop     r5

    /* Get registers pushed in ISR */
    pop     r25
    pop     r24
    pop     r23
    pop     r22
    pop     r21
    pop     r20
    pop     r19
    pop     r18
    pop     r7
    pop     r6

    /* Get return address outside of ISR context (will be destroyed) */
    pop     r28
    pop     r29

    /* Switch to new task stack */
    lds     r28, pxCurrentTCB       // Load new SP address
    lds     r29, pxCurrentTCB + 1
    out     _SFR_IO_ADDR(SPL), r28  // Change to new SP
    out     _SFR_IO_ADDR(SPH), r29

    /* Place return address to taskFxn */
    lds     r26, pxAdr              // Load address of taskFxn
    lds     r27, pxAdr + 1
    push    r26                     // Push "return" to taskFxn
    push    r27

    /* Push registers saved in ISR onto new stack */
    push    r6
    push    r7
    push    r18
    push    r19
    push    r20
    push    r21
    push    r22
    push    r23
    push    r24
    push    r25

    /* Place return address to ISR */
    push    r5
    push    r4

    /* Place return address to schedule() */
    push    r3
    push    r2
    ret

.global TIMER0_OVF_vect
TIMER0_OVF_vect:
    /* Push r0 and SREG */
    push    r0
    in      r0, _SFR_IO_ADDR(SREG)
    push    r0

    /* Push work registers r18 to r25 */
    push    r18
    push    r19
    push    r20
    push    r21
    push    r22
    push    r23
    push    r24
    push    r25

    /* Load sysTick (uint32_t) */
    lds     r22, sysTick
    lds     r23, sysTick + 1
    lds     r24, sysTick + 2
    lds     r25, sysTick + 3

    /* Increment sysTick by 1 */
    ldi     r19, 1
    add     r22, r19
    adc     r23, r1
    adc     r24, r1
    adc     r25, r1

    /* Store sysTick */
    sts     sysTick, r22
    sts     sysTick + 1, r23
    sts     sysTick + 2, r24
    sts     sysTick + 3, r25

    /* Perform modulo (2x32bit integer) calculation */
    //ldi     r18, lo8(100)
    //ldi     r19, 0
    //ldi     r20, 0
    //ldi     r21, 0
    //call __udivmodsi4

    /* Perform modulo (integer mod 2^i) calculation */
    //andi    r23, 0x03
    //clr     r24
    //clr     r25

    /* Compare with 0 */
    //subi    r22, 0
    //cpc     r23, r1
    //cpc     r24, r1
    //cpc     r25, r1
    //brne skip
    call schedule
skip:
    /* Restore work registers */
    pop r25
    pop r24
    pop r23
    pop r22
    pop r21
    pop r20
    pop r19
    pop r18

    /* Restore SREG and r0 */
    pop    r0
    out    _SFR_IO_ADDR(SREG), r0
    pop    r0

    reti