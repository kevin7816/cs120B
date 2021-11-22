/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Youtube: https://youtu.be/Z-NP0wR48hk
 *
 *	I hooked up my LED Matrix to the microcontroller and shift register. I have the display being the basic movement of the targets for my game. 
 *	I still need to implement the joystick for player movement, add a button to fire at the moving targets, keep score of how many targets destoryed and display it on LCD screen.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#include "timer.h"

#endif

void transmit_data(unsigned char data) {
	int i;
	for(i = 0;i<8;i++){
	PORTC = 0x08;
	PORTC |= ((data >> i) & 0x01);
	PORTC |= 0x02;
	}
	PORTC |= 0x04;
	PORTC = 0x00;
}
unsigned char left, right, col, row =0x00;
enum displayStates{START, INIT, LEFT, RIGHT, DOWN, END};
int DisplayTick(int state) {
	switch(state) {
		case START:
			state = INIT;
			break;
		case INIT:
			state = RIGHT;
			break;
		case LEFT:
			if(left >= 1){
				state = DOWN;
			} else {
				state = LEFT;
				left++;
			}
			if((col & 0x80) == 0x80){
				state = END;
			}
			break;
		case RIGHT:
			if(right >= 1){
				state = DOWN;
			} else {
				state = RIGHT;
				right++;
			}
			break;
		case DOWN:
			if(right >= 1){
				right = 0;
				state = LEFT;
			} else if (left >= 1){
				left = 0;
				state = RIGHT;
			}
			break;
		case END:
			break;
		default: 
			break;
	}

	switch(state) {
                case START:
                        break;
                case INIT:
			row = 0xE3;
			PORTD = row;
			col = 0x07;
			transmit_data(col);
                        break;
                case LEFT:
			row = row << 1;
			row = row | (0x01);
			PORTD = row;
			transmit_data(col);
                        break;
                case RIGHT:
			row = row >> 1;
			PORTD = row;
			transmit_data(col);
                        break;
                case DOWN:
			PORTD = row;
			col = col << 1;
			transmit_data(col);
                        break;
                case END:
                        break;
		default:
			break;
        }
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	const char start = 0;

	task1.state=start;
	task1.period=500;
	task1.elapsedTime = task1.period;
	task1.TickFct = &DisplayTick;

	TimerSet(500);
	TimerOn();
	unsigned short i;
	while(1){
	for (i =0; i< numTasks; i++) {
	if(tasks[i]->elapsedTime == tasks[i]->period) {
		tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
		tasks[i]->elapsedTime = 0;
	}
	tasks[i]->elapsedTime += 500;
	}
	while(!TimerFlag);
	TimerFlag=0;
	}
    return 1;
}
