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
	unsigned char temp = 0x00;
	unsigned char output = 0x00;
    /* Insert your solution below */
    while (1) {
	output = 0x00;
	temp = PINA;
	if(temp == 0x0F || temp == 0x0E || temp == 0x0D){
	output = 0x3F;
	}
	else if(temp == 0x0C || temp == 0x0B || temp == 0x0A){
        output = 0x3E;
        }
	else if(temp == 0x09 || temp == 0x08 || temp == 0x07){
        output = 0x3C;
        }
	else if(temp == 0x06 || temp == 0x05){
        output = 0x38;
        }
	else if(temp == 0x04 || temp == 0x03){
        output = 0x70;
        }
	else if(temp == 0x02 || temp == 0x01){
        output = 0x60;
        }
	else{
	output = 0x40;
	}
	PORTC = output;
    }
    return 1;
}
