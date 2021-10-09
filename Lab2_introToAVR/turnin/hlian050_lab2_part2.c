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
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    unsigned char spot1 = 0x00;
    unsigned char spot2 = 0x00;
    unsigned char spot3 = 0x00;
    unsigned char spot4 = 0x00;
    unsigned char cntavail;
    while (1) {
	cntavail = 0x04;
	spot1 = PINA & 0x01;
	spot2 = PINA & 0x02;
	spot3 = PINA & 0x04;
	spot4 = PINA & 0x08;

	if(spot1 == 0x01){
	cntavail--;
	}
	if(spot2 ==0x02){
	cntavail--;
	}
	if(spot3 == 0x04){
	cntavail--;
	}
	if(spot4 == 0x08){
	cntavail--;
	}
	PORTC = cntavail;
    }
    return 0;
}
