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

enum states {START, X, Y, PRESS, RELEASED, UNLOCKED} state;
unsigned char count1, count2, count3, count4, xflag, yflag= 0x00;
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
		state = X;
		count1++;
		} else if (input ==0x04){
		count2++;
		state = RELEASED;
		} else {
		state = PRESS;			
		}
		break;

	case Y:
		if(input == 0x02 && yflag == 0){
		state = Y;
		yflag++;
		} else if (input == 0x00 && yflag == 1){
		state = X;
		yflag = 0;
		}
		else {
		count4++;
		state = PRESS;
		}
		break;
			
	case UNLOCKED: 
		if( PORTB == 0x01){
		PORTB =	0x00;
		} else {
		PORTB = 0x01;	
		}
		state = PRESS;

	case X: 
		if(input == 0x00 && xflag == 1){
		xflag++;
		state = Y;
		} else if( input == 0x01 && xflag == 0){
		xflag++;
		state = X;
		}
		else if(input == 0x01 && xflag == 2){
		xflag++;
		state = X;
		}
		else if(input == 0x00 && xflag == 3){
		xflag = 0;
		state = UNLOCKED;
		} else {
		state = PRESS;
		}
		break;

	default:
		break;
	}
}
