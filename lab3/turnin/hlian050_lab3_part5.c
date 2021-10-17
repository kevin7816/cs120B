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
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFE; PORTB = 0x00;
	unsigned char input,temp,bit,output = 0x00;
    /* Insert your solution below */
    while (1) {
	output = 0x00;
	input = PIND;
	bit = PINB & 0x01;
	temp = input & 0x80;
	if(temp == 0x80){
	output = output | 0x02;
	}
	else{
	input = input << 1;
	input = input + bit;
	    if(input >= 0x46){
		output = output | 0x02;
	    } else if(input > 0x05 && input < 0x46){
		output = output | 0x04;
	    } 
	}
	PORTB = output;

    }
    return 1;
}
