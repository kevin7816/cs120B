/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://youtu.be/vej61oJSJ9k
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void Tick(unsigned char input);

enum states {START, MAIN, PRESS1, RELEASE1, PRESS2, RELEASE2} state;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA=0xFF;
	DDRC = 0xFF; PORTC=0x00;
	unsigned char input = 0x00;
	state = START;
    /* Insert your solution below */
    while (1) {
	input = ~PINA;
	Tick(input);
    }
    return 1;
}

void Tick(unsigned char input) {
	switch(state){
	case START:
		state = MAIN;
		break;
	
	case MAIN: 
		if((input & 0x01) == 0x01) {
		state = PRESS1;
		} else {
		state = MAIN;
		}
		break;

	case PRESS1:
		if((input & 0x01) == 0x00){
		state = RELEASE1;
		} else {
		state = PRESS1;
		}
		break;

	case RELEASE1:
		if((input & 0x01) == 0x01){
		state = PRESS2;
		} else {
		state = RELEASE1;
		}
		break;

	case PRESS2:
		if((input & 0x01) == 0x00){
		state = RELEASE2;
		} else {
		state = PRESS2;
		}
		break;

	case RELEASE2: 
		if((input & 0x01) == 0x01){
		state = PRESS1;
		} else {
		state = RELEASE2;
		}
		break;

	default:
		break;
	}

	switch(state) {
	case START:
		break;

	case MAIN: 
		PORTC = 0x00;
		break;

	case PRESS1:
		PORTC = 0x2A;
		break;

	case RELEASE1:
		break;

	case PRESS2:
		PORTC = 0x15;
		break;
	
	case RELEASE2:
		break;

	default:
		break;
	}
}
