/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Youtube Link: https://youtu.be/23sBCpfNPQU
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

enum states {START, LIGHT1, LIGHT2, LIGHT3, PAUSE} state;
unsigned char light,flag1 = 0;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	DDRC = 0xFF; PORTC=0x00;
	DDRD = 0xFF; PORTD=0x00;
	unsigned char input = 0x00;
	state = START;
	TimerSet(300);
	TimerOn();
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
		state = LIGHT1;
		break;
	
	case LIGHT1:
		flag1 = 0;
		if((input & 0x01) == 0x01){
		light = 0x01;
		state = PAUSE;
		} else {
		state = LIGHT2;
		}	
		break;

	case LIGHT2:
		if((input & 0x01) == 0x01){
		light = 0x02;
		state = PAUSE;
		} else {
		    if(flag1 == 0) {
			state = LIGHT3;
			flag1 = 1;
		    } else {
			state = LIGHT1;
			flag1 = 0;
		    }
		}
		break;

	case LIGHT3:
		if((input & 0x01) == 0x01){
		light = 0x04;
		state = PAUSE;
		} else {
		state = LIGHT2;
		}
		break;

	case PAUSE: 
		if((input & 0x01) == 0x01){
		state = LIGHT1;
		} else {
		state = PAUSE;
		}
		break;

	default:
		break;
	}

	switch(state) {
	case START:
		break;
	case LIGHT1:
		PORTB = 0x01;
		break;
	case LIGHT2:
		PORTB = 0x02;
		break;
	case LIGHT3: 
		PORTB = 0x04;
		break;
	case PAUSE:
		PORTB = light;
		break;
	default:
		break;
	}
}
