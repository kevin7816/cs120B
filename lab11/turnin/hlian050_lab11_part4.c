/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Youtube: https://youtu.be/fQcuwfRdhiE
 */
#include <avr/io.h>
#include <timer.h>
#include <keypad.h>
#include "io.h"
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char output, temp = 0x00;

enum keypadStates{START, WAIT};
int KeypadTick(int state) {
	temp = GetKeypadKey();
	switch(state) {
		case START:
			state = WAIT; 
			break;
		case WAIT:
			state = WAIT;	
			break;
		default:	
			break;
	}

	switch(state) {
		case START:	
			break;
		case WAIT:	
			switch(temp) {
				case '\0':
				output = 0x1F;
				break;
				case '1':
				output = 0x01;
				break;
				case '2':
				output = 0x02;
				break;
				case '3':
				output = 0x03;
				break;
				case '4':
				output = 0x04;
				break;
				case '5':
				output = 0x05;
				break;
				case '6':
				output = 0x06;
				break;
				case '7':
				output = 0x07;
				break;
				case '8':
				output = 0x08;
				break;
				case '9':
				output = 0x09;
				break;
				case 'A':
				output = 0x0A;
				break;
				case 'B':
				output = 0x0B;
				break;
				case 'C':
				output = 0x0C;
				break;
				case 'D':
				output = 0x0D;
				break;
				case '*':
				output = 0x0E;
				break;
				case '0':
				output = 0x00;
				break;
				case '#':
				output = 0x0F;
				break;
				default:
				output = 0x1B;
				break;
			}
			PORTB = output;
			break;
				
		default:
			break;
	}
	return state;
}

enum DisplayStates { DISPLAYSTART, DISPLAYWAIT };
unsigned char count = 1;
int DisplayTick(int state) {
	switch(state) {
		case DISPLAYSTART:
			if(output == 0x1F){
			state = DISPLAYSTART;
			} else {
			state = DISPLAYWAIT;
			}
			break;
		case DISPLAYWAIT:
			state = DISPLAYSTART;
			break;
		default:
			break;
	}

	switch(state) {
		case DISPLAYSTART:
			break;
		case DISPLAYWAIT:
			LCD_Cursor(count);
			LCD_WriteData(temp);
			if(count > 16){
			count = 1;
			} else {
			count++;
			}
			break;
		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    LCD_init();
    LCD_DisplayString(1,"CONGRATULATIONS!");
    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = 0;

    task1.state = start;
    task1.period = 20;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadTick;

    task2.state = start;
    task2.period = 20;
    task2.elapsedTime = task2.period;
    task2.TickFct = &DisplayTick;

    TimerSet(20);
    TimerOn();

    unsigned short i;
    while (1) {
	for (i = 0; i < numTasks; ++i) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
