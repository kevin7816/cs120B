/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Youtube Link :https://youtu.be/Jpgg6m1rcHc
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned char max = 0xFF;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char output = 0x00;
	ADC_init();
    /* Insert your solution below */
    while (1) {
	unsigned short myShort = ADC;
	if( myShort >= max/2){
		output = 0x01;
	} else {
		output = 0x00;
	}
	PORTB = output;
    }
    return 1;
}
