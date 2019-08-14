/*
 * AutosarOS.c
 *
 * Created: 04.06.2019 18:43:30
 * Author : Pascal Romahn
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "assert.h"

#include "Task.h"

static volatile uint16_t delay = 30;


int main(void)
{
    
    // Blue D1; White D0
    DDRD  = 0x00;   // PD as input
    PORTD = 0xFF;   // enable PU on PD
    GICR  = 1 << INT0 | 1 << INT1;                              // Enable INT0 and INT1
    MCUCR = 1 << ISC01 | 0 << ISC00 | 1 << ISC11 | 0 << ISC10;  // Trigger INT0 and INT1 on falling edge
    
    DDRB  = 0xFF;   // PB as output
    PORTB = 0xFF;   // keep all LEDs off
    
    TCCR0 = 1 << CS01 | 1 << CS00;      // Enable Timer0 with Prescaler 1024
    TIMSK |= 1 << TOIE0;                // Enable Overflow Interrupt (Timer0)
    
    sei();  //Enable Global Interrupt
    
    while (1) 
    {
        static uint8_t t = 0;
        static int8_t d = 1;
        //PORTB &= ~(1 << t);   // turn LED on
        //_delay_ms(delay);   
        //PORTB |= (1 << t);  // turn LED off 
        //_delay_ms(delay);   
        //
        //t = t + d;
        //if (t == 0)
            //d = 1;
        //else if (t == 7)
            //d = -1;
        
       assert(d == 1 || d == -1);
       assert(t >= 0 && t < 8);
    }
}

ISR(INT0_vect) 
{
    delay = delay * 2;
}

ISR(INT1_vect)
{
    delay = delay / 2;
}