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
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char num1 = 0x00;
	unsigned char num2 = 0x00;
	unsigned char temp = 0x00;
	unsigned char output = 0x00;
	unsigned char i = 0x00;
	unsigned char first, second = 0x00;
    /* Insert your solution below */
    while (1) {
	output = 0x00;
	num1 = PINA;
	num2 = PINB;
	for (i=0;i<8;i++){
	     temp = num1 & 0x01;
	     if(temp == 0x01){
		output = output + 0x01;
	     }
	     num1 = num1 >> 1;
	     first++;
	}

	for (i=0;i<8;i++){
	    temp = num2 & 0x01;
	    if(temp == 0x01){
		output = output + 0x01;
	    }
	    num2 = num2 >> 1;
	    second++;
	}
	PORTC = output;
    }
    return 1;
}
