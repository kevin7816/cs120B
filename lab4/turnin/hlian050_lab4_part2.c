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

enum states {START, ADD, SUB, RESET, CONTROL} state;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	unsigned char input = 0x00;
	state = START;
	PORTC = 0x07;
    /* Insert your solution below */
    while (1) {
	input = PINA;
	Tick(input);
    }
    return 1;
}

void Tick(unsigned char input) {
	switch(state){
	case START:
		PORTC = 0x07;
		state = CONTROL;
		break;
	
	case CONTROL:
		if(input == 0x03){
		PORTC = 0x00;
		state = RESET;
		} else if(input == 0x01 && PORTC < 9){
		PORTC++;	
		state = ADD;
		} else if(input == 0x02 && PORTC > 0){
		PORTC--;
		state = SUB;
		} else {
		break;
		}
		break;

	case ADD:
		if(input == 0x03){
		PORTC = 0;
		state = RESET;
		} else {
		if (input != 0x01){
		state = CONTROL;
		} else {
		state = ADD;
		}
		}
		break;

	case SUB:
		if(input == 0x03){
		PORTC =0;
		state = RESET;
		} else {
		if(input != 0x02){
		state = CONTROL;
		} else {
		state = SUB;
		}
		}
		break;

	case RESET: 
		if(input == 0x03){
		state = RESET;
		} else {
		state = CONTROL;
		}
		break;

	default:
		break;
	}
}
