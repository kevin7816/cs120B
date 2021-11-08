/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Youtube link: https://youtu.be/fTEQTl1CV_4
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M=0;
unsigned long _avr_timer_cntcurr = 0;

enum States1{START, ON, OFF}state1;
enum States2{START2, LIGHT0, LIGHT1, LIGHT2}state2;
enum States3{START3, OUTPUT}state3;
enum States4{START4, SOUND, QUITE}state4;

unsigned char out1 = 0x00;
unsigned char out2 = 0x00;
unsigned char out3 = 0x00;
unsigned long i, j, input, count = 0x00;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 =0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;

}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA= 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	tick1();
	tick2();
	tick4();
	tick3();
	while(!TimerFlag);
	TimerFlag=0;
    }
    return 1;
}

void tick1(){
	switch(state1){
	    case START: 
		state1 = ON;
		break;
	
	    case ON: 
		if(i >= 1000){
			i = 0;
			state1 = OFF;
		}
		break;

	    case OFF:
		if(i >= 1000){
			i = 0;
			state1 = ON;
		}
		break;

	    default:
		break;
	}

	switch(state1){
		case START:
		i = 0;
		break;
		case ON:
		i++;
		out1 = 0x08;
		break;
		case OFF:
		i++;
		out1 = 0x00;
		break;
		default:
		break;
	}
}

void tick2(){
	switch(state2){
		case START2: 
		state2= LIGHT0;
		break;
		case LIGHT0:
		if(j >= 300){
		j = 0;
		state2 = LIGHT1;
		}
		break;
		case LIGHT1:
		if(j >= 300){
		j = 0;
		state2 = LIGHT2;
		}
		break;
		case LIGHT2:
		if(j >= 300){
		j = 0;
		state2 = LIGHT0;
		}
		break;
		default:
		break;
	}

	switch(state2){
		case START2:
			j = 0;
			break;
		case LIGHT0:
			j++;
			out2 = 0x01;
			break;
		case LIGHT1:
			j++;
			out2 = 0x02;
			break;
		case LIGHT2:
			j++;
			out2 = 0x04;
			break;
		default:
			break;
	}
}

void tick3(){
	switch(state3){
		case START3:
			state3 = OUTPUT;
			break;
		case OUTPUT:
			state3 = OUTPUT;
			break;
		default:
			break;
	}

	switch(state3){
		case START3:
			PORTB = 0x00;
			break;
		case OUTPUT:
			PORTB = out3 | out1 | out2;
			break;
		default:
			break;
	}
}

void tick4(){
	unsigned char flag = 0x00;
	input = ~PINA & 0x04;
	switch(state4){
		case START4:
			if((input & 0x04)== 0x04){
			state4 = SOUND;
			}
			break;
		case SOUND:
			if((input & 0x00) == 0x00){
			state4 = QUITE;
			}
			break;
		case QUITE:
			if((input & 0x04) == 0x00){
			state4 = SOUND;
			}
		default:
			break;
	}

	switch(state4){
		case START4:
			count = 0;
			out3 = 0;
			break;
		case SOUND: 
			if(count < 2){
			   if(flag == 0){
				out3 = 0x10;
				flag = 1;
			   } else {
				out3 = 0x00;
				flag = 1;
			   }
			  count++; 
			} else {
			  count = 0;
			}
			break;
		case QUITE:
			count = 0;
			out3 = 0;
			break;
		default:
			break;
	}
}
