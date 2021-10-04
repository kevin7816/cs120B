/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    unsigned char seat1 = 0x00;
    unsigned char seat2 = 0x00;
    unsigned char seat3 = 0x00;
    unsigned char output = 0x00;;
    while (1) {
	seat1 = PINA;
	seat2 = PINB;
	seat3 = PINC;
	output = (seat1 + seat2) + seat3;
	output = output >> 2;

	if(output > 0x8C){
	output = output | 0x01;
	} else {
	output = output & 0xFE;
	}
	if(seat1 - seat3 > 80 || seat3 - seat1 > 80){
	output = output | 0x02;
	} else {
	output = output & 0xFD;
	}
	PORTD = output;
    }
    return 0;
}
