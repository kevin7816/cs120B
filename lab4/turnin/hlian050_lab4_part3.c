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

void Tick(unsigned char input);

enum states {START, X, Y, PRESS, RELEASED} state;
unsigned char count1, count2, count3, count4 = 0x00;
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	unsigned char input,temp = 0x00;
	state = PRESS;
    /* Insert your solution below */
    while (1) {
	input = PINA;
	Tick(input);
	temp = input & 0x80;
	if(temp == 0x80){
	PORTB = 0x00;
	}
    }
    return 1;
}

void Tick(unsigned char input) {
	switch(state){
	case START:
		state = PRESS;
		break;
	
	case PRESS:
		if(input == 0x04){
		state = RELEASED;
		} else {
		state = PRESS;
		}
		break;

	case RELEASED:
		if(input == 0x00){
		state = Y;
		count1++;
		} else {
		count2++;
		state = RELEASED;
		}
		break;

	case Y:
		if(input == 0x02){
		PORTB = 0x01;
		count3++;
		}
		count4++;
		state = PRESS;
		break;

	case X: 
		state = PRESS;
		break;

	default:
		break;
	}
}
