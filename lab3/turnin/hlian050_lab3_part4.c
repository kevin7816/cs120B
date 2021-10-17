/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab 3  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xff; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char temp = 0x00;
    /* Insert your solution below */
    while (1) {
	temp = PINA;
	PORTB = temp;
	PORTC = temp;
	PORTB = PORTB >> 4;
	PORTC = PORTC << 4;
    }
    return 1;
}
