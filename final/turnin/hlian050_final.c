/*	Author: Kevin Liang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Youtube:https://youtu.be/M5h_zD0CE6I
 *	I implemented the joystick to show user input movement on the LED matrix. All that is left is to hook up a button to shoot at the targets and implement game logic with 
 *	concurrent SMs.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include <util/delay.h>
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

void A2D_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

unsigned char left, right, col1, col2, col3, row1, row2, row3, end, score =0x00;
enum alienStates{ALIENSTART, INIT, LEFT, RIGHT, DOWN, END};
int AlienTick(int state) {
	switch(state) {
		case ALIENSTART:
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
			if((col3 & 0x80) == 0x80){
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
			if((col3 & 0x80) == 0x80){
				state = END;
			}
			if((col2 & 0x80) == 0x80){
                                state = END;
                        }
			if((col1 & 0x80) == 0x80){
                                state = END;
                        }
			break;
		case END:
			state = END;
			break;
		default: 
			break;
	}

	switch(state) {
                case ALIENSTART:
                        break;
                case INIT:
			row1 = 0xE3;
			row2 = 0xE3;
			row3 = 0xE3;
			//PORTD = row;
			col1 = 0x01;
			col2 = 0x02;
			col3 = 0x04;
			//transmit_data(col);
                        break;
                case LEFT:
			row1 = row1 << 1;
			row1 = row1 | (0x01);
			row2 = row2 << 1;
			row2 = row2 | (0x01);
			row3 = row3 << 1;
			row3 = row3 | (0x01);
			//PORTD = row;
			//transmit_data(col);
                        break;
                case RIGHT:
			row1 = row1 >> 1;
			row2 = row2 >> 1;
			row3 = row3 >> 1;
			//PORTD = row;
			//transmit_data(col);
                        break;
                case DOWN:
			//PORTD = row;
			col1 = col1 << 1;
			col2 = col2 << 1;
			col3 = col3 << 1;
			//transmit_data(col);
                        break;
                case END:
			end = 1;
			PORTD = score;	
                        break;
		default:
			break;
        }
	return state;
}

unsigned char movement = 0x00;
enum joystickStates{JOYSTART,SPOT1,SPOT2,SPOT3,SPOT4,SPOT5};
int JoystickTick(int state){

	ADMUX = 0xC0;
	unsigned short input = ADC;
	switch(state){
		case JOYSTART:
			state = SPOT1;
			break;
		case SPOT1:
			if(input > 900){
				state = SPOT2;
			}
			break;
		case SPOT2:
			if(input > 900){
				state = SPOT3;
			} else if(input < 300){
				state = SPOT1;
			}
			break;
		case SPOT3:
			if(input > 900){
				state = SPOT4;
			} else if(input < 300){
				state = SPOT2;
			}
			break;
		case SPOT4:
			if(input > 900){
				state = SPOT5;
			} else if(input < 300){
			        state = SPOT3;
			}
	 		break;
		case SPOT5:
			if(input < 300){
				state = SPOT4;
			}
			break;
		default:
			break;
	}

	switch(state){
		case JOYSTART:
			break;
		case SPOT1:
			movement = 0xFE;
			break;
		case SPOT2:
			movement = 0xFD;
			break;
		case SPOT3:
			movement = 0xFB;
			break;
		case SPOT4:
			movement = 0xF7;
			break;
		case SPOT5:
			movement = 0xEF;
			break;
		default:
			break;
	}
	return state;
}
unsigned char shot, shootRow, shootCol = 0x00;
enum displayStates{START,DISPLAY, DONE};
int DisplayTick(state) {
	
	switch(state){
		case START:
			state = DISPLAY;
			break;
		case DISPLAY:
			if(end ==1){
			       state = DONE;
			} else {	       
			state = DISPLAY;
			}
			break;
		case DONE:
			state = DONE;
	}

	switch(state){
		case START:
			break;
		case DISPLAY:
			PORTD = movement;
			transmit_data(0x80);
			_delay_ms(100);
			if(shot == 1){
				PORTD = shootRow;
				transmit_data(shootCol);
				_delay_ms(100);
			}
			PORTD = row1;
			transmit_data(col1);
			_delay_ms(300);
			PORTD = row2;
			transmit_data(col2);
			_delay_ms(300);
			PORTD = row3;
			transmit_data(col3);
			break;
		case DONE:
			break;
		default:
			break;
	}
	return state;
}

unsigned char elimRow, elimCol = 0x00;
unsigned char moves, hitFlag, input1 = 0x00;
enum shooterStates{SHOOTSTART, SHOOTINIT, PRESS, FIRED, HIT};
int ShooterTick(state) {

	switch(state){
		case SHOOTSTART:
			state = SHOOTINIT;
			break;
		case SHOOTINIT:
			if((input1 & 0x04) == 0x04){
				state = PRESS;
			} else {
				state = SHOOTINIT;
			}
			break;
		case PRESS: 
			state = FIRED;
			break;
		case FIRED:
			if(hitFlag == 1){
				state = HIT;
			} else if (moves == 7) {
				state = SHOOTINIT;
			} else {
				moves++;
				state = FIRED;
			}
			break;
		case HIT:
			state = SHOOTINIT;
			break;

	}

	switch(state){
		case SHOOTSTART:
			score = 0;
			break;
		case SHOOTINIT:
			moves = 0;
			shot = 0;
			hitFlag = 0;
			break;
		case PRESS:
			shootRow = movement;
			shootCol = 0x40;
			shot = 1;
			break;
		case FIRED:
			if(((shootCol & col3) == shootCol) && ((~shootRow & ~row3) == ~shootRow )){
				hitFlag = 1;
				elimRow = (~shootRow & ~row3);
				row3 = (row3 | elimRow);
			} else if (((shootCol & col2) == shootCol) && ((~shootRow & ~row2) == ~shootRow)){
				hitFlag = 1;
				elimRow = (~shootRow & ~row2);
				row2 = (row2 | elimRow);
			} else if (((shootCol & col1) == shootCol) && ((~shootRow & ~row1) == ~shootRow)){
				hitFlag = 1;
				elimRow = (~shootRow & ~row1);
				row1 = (row1 | elimRow);
			} else {
				shootCol = shootCol >> 1;
			}
			break;
		case HIT:
			if(row3 == 0xFF){
				col3 = 0x00;
			}
			if(row2 == 0xFF){
				col2 = 0x00;
			}
			if(row1 == 0xFF){
				col1 = 0x00;
			}
			shot = 0;
			score++;
			break;
	}
	return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	static task task1, task2, task3, task4;
	task *tasks[] = {&task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	const char start = 0;

	task1.state=start;
	task1.period=1000;
	task1.elapsedTime = task1.period;
	task1.TickFct = &AlienTick;

	task2.state=start;
	task2.period=200;
	task2.elapsedTime= task2.period;
	task2.TickFct = &JoystickTick;

	task3.state=start;
	task3.period=200;
	task3.elapsedTime=task3.period;
	task3.TickFct = &ShooterTick;

	task4.state=start;
	task4.period=200;
	task4.elapsedTime=task4.period;
	task4.TickFct = &DisplayTick;

	TimerSet(10);
	TimerOn();
	A2D_init();
	unsigned short i;
	while(1){
	for (i =0; i< numTasks; i++) {
	if(tasks[i]->elapsedTime == tasks[i]->period) {
		tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
		tasks[i]->elapsedTime = 0;
	}
	tasks[i]->elapsedTime += 100;
	}
	input1 = ~PINA;
	while(!TimerFlag);
	TimerFlag=0;
	}
    return 1;
}
