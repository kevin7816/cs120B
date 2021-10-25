/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Youtube Link: https://youtu.be/r14Tvd1gQyg
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M =1;
unsigned long _avr_timer_cntcurr =0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A=125;
	TIMSK1 = 0x02;
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag=1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr==0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void Tick(unsigned char input);

enum states {START, ADD, SUB, RESET, CONTROL} state;
unsigned char i = 0;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	unsigned char input = 0x00;
	state = START;
	TimerSet(100);
	TimerOn();
	PORTB = 0x07;
    /* Insert your solution below */
    while (1) {
	input = ~PINA;
	Tick(input);
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}

void Tick(unsigned char input) {
	switch(state){
	case START:
		PORTB = 0x07;
		state = CONTROL;
		break;
	
	case CONTROL:
		i = 0;
		if(input == 0x03){
		PORTB = 0x00;
		state = RESET;
		} else if(input == 0x01 && PORTB < 9){
		PORTB++;	
		state = ADD;
		} else if(input == 0x02 && PORTB > 0){
		PORTB--;
		state = SUB;
		} else {
		break;
		}
		break;

	case ADD:
		if(input == 0x03){
		PORTB = 0;
		state = RESET;
		} else {
		if (input != 0x01){
		state = CONTROL;
		} else {
		state = ADD;
		i++;
		}
		}
		if(i == 10){
		if(PORTB != 9){
		PORTB++;
		}
		i = 0;
		}
		break;

	case SUB:
		if(input == 0x03){
		PORTB =0;
		state = RESET;
		} else {
		if(input != 0x02){
		state = CONTROL;
		} else {
		state = SUB;
		i++;
		}
		}
		if(i == 10){
		if(PORTB != 0){
		PORTB--;
		}
		i = 0;
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
