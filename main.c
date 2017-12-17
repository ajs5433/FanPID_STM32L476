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
	
	print("\n\rQuit button has been pressed!! \n\rExiting program...");
	
}





void initialize(){
	/* Calling initializing functions*/
	System_Clock_Init();
	UART2_Init();
	initPWMs();
	
	/* Initializing program variables*/	
	quit 				= false;
	print_echo 	= false;
	
	echo 	= 0;
	max		= 0;
	min		= 65535;
	duty	= 0;
	kp		= 0;
	ki		= 0;
	kd		=	0;
	
	set_point = 50;
	
	/* Clear Screen*/
	print("\n\r\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nSTARTING PROGRAM!!\n\r");
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
				print("Print echo changed! \n\r");
				print_echo = ~print_echo;
				break;
			default:
				print("Input not recognized, press H for Help\n\r");
				break;
		}
	}
}


void displayEcho(){
	n = sprintf((char *)buffer, "Current Echo: %d\n\r", echo);
	USART_Write(USART2,buffer , n);
}

void displayHelp(){
	print("\n\rHelp button selected!\n\r");
	print("Press H - for Help\n\r");
	print("Press E - to display Echo\n\r");
	print("Press P - to Print current values\n\r");
	print("Press S - to select new Set Point\n\r");
	print("Press + - to Increase Set Point\n\r");
	print("Press - - to Decrease Set Point\n\r");
	print("Press Q - to Quit!\n\r");
}
void printCurrentValues(){
	print("\n\rPrinting Values!\n\r");
	n = sprintf((char *)buffer, "Set Point : %d \tCurrent Echo: %d\n\r", set_point, echo);
	USART_Write(USART2,buffer , n);
	n = sprintf((char *)buffer, "Min distance: %d \tMax distance: %d\n\r", min, max);
	USART_Write(USART2,buffer , n);
	n = sprintf((char *)buffer, "kP: %d \tkI: %d\tkD: %d\n\r", kp, ki, kd);
	USART_Write(USART2,buffer , n);
}
void changeSetPoint(){
	n = sprintf((char *)buffer,"\n\rCurrent Set Point is: %d\n\r", set_point);
	USART_Write(USART2,buffer , n);
	
	// Check if user wants to change setpoint
	while(1){
		print("Would you like to change the Set Point?\n\rY - Yes \t N - No\n\r");
		input_key = USART_Read(USART2);
		if(input_key!='Y' && input_key!='N' && input_key!='y' && input_key!='n')
			print("Input not recognized, try again\n\r");
		else
			break;
	}
	
	if(input_key == 'Y'||input_key == 'y'){
		print("New set point: ");
		temp1 = USART_Read(USART2) - '0';
		temp2 = USART_Read(USART2) - '0';
		set_point = temp1 *10 + temp2;
		print("\n\rSet Point Successfully changed!\n\r");
	}else
		print("\n\rKeeping old value\n\r");
	
	n = sprintf((char *)buffer,"Set Point: %d\n\r", set_point);
	USART_Write(USART2,buffer , n);
	
}


void increaseSetPoint(void){
	set_point+=SP_STEP;
	
	if(set_point > SP_MAX)
			set_point = SP_MAX;
	
	n = sprintf((char *)buffer, "Increasing Set Point!\n\r New Value: %d\n\r", set_point);
	USART_Write(USART2,buffer , n);

}

void decreaseSetPoint(void){
	set_point-=SP_STEP;
	
	if(set_point < SP_MIN)
			set_point = SP_MIN;
	
	n = sprintf((char *)buffer, "Decreasing Set Point!\n\r New Value: %d\n\r", set_point);
	USART_Write(USART2,buffer , n);

}

