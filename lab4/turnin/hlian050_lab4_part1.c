/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void Tick(unsigned char a0);

enum states {START, OFF_RELEASE, ON_PRESS, ON_RELEASE, OFF_PRESS} state;
unsigned char count = 0x00;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	unsigned char a0 = 0x00;
	state = START;
    /* Insert your solution below */
    while (1) {
	a0 = PINA & 0x01;
	Tick(a0);
    }
    return 1;
}

void Tick(unsigned char a0) {
	switch(state){
	case START:
		PORTB = 0x00;
		PORTB = PORTB | 0x01;
		state = OFF_RELEASE;
		break;
	
	case OFF_RELEASE:
		if(a0 == 0x01){
		state = ON_PRESS;
		PORTB = PORTB & 0x00;
		PORTB = PORTB | 0x02;
		} else {
		state = OFF_RELEASE;
		}
		count++;
		break;

	case ON_PRESS:
		if(a0 == 0x01){
		state = ON_PRESS;
		}
		else{
		state = ON_RELEASE;
		}
		break;

	case ON_RELEASE:
		if(a0 == 0x01){
		state = OFF_PRESS;
		PORTB = PORTB & 0x00;
		PORTB = PORTB | 0x01;
		} else {
		state = ON_RELEASE;
		}
		break;

	case OFF_PRESS: 
		if(a0 == 0x01){
		state = OFF_PRESS;
		} else {
		state = OFF_RELEASE;
		}
		break;

	default:
		break;
	}
}
