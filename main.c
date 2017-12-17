/*********************************************************************************
* Project Name:      PID Control Algorithm - Performance of Real-Time Systems
* Version:           2.0
*
* Device Used:       STM32L476GV
* Software Used:     Keil µVision
* Authors:           Alberto Santos-Castro
                     Graham Home
*********************************************************************************/

#include "main.h"


int main(void){
	
	initialize();
	
	while(!quit)
		checkUserInput();
	
}


void initialize(){
	/* Calling initializing functions*/
	System_Clock_Init();
	UART2_Init();
	
	/* Initializing program variables*/	
	quit 				= false;
	print_echo 	= false;
}

void print(char *string){
	n = sprintf((char *)buffer, string);
	USART_Write(USART2,buffer , n);
}

void checkUserInput(){

	if((USART2->ISR & USART_ISR_RXNE))
	{
		input_key = (uint8_t)(USART2->RDR & 0xFF);

		switch(input_key){
			case('e'):
			case('E'):
				displayEcho();
				break;
			case('h'):
			case('H'):
				displayHelp();
				break;
			case('P'):
			case('p'):
				printCurrentValues();
				break;
			case('s'):
			case('S'):
				changeSetPoint();
				break;
			case('q'):
			case('Q'):
				quit = true;
				break;
			case('='):
			case('+'):
				increaseSetPoint();
				break;
			case('-'):
				decreaseSetPoint();
				break;
			case(13):
				print_echo = ~print_echo;
				break;
			default:
				break;
		}
	}
}


void displayEcho(){}
void displayHelp(){}
void printCurrentValues(){}
void changeSetPoint(){}
void increaseSetPoint(){}
void decreaseSetPoint(){}
