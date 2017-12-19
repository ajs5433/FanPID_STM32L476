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
	
	while(!quit){
		checkUserInput();
		echo = TIM4_Echo_read();
		getEcho();
	}
	
	print("\n\rQuit button has been pressed!! \n\rExiting program...");
		
}




void initialize(){
	
	/* Initializing program variables*/	
	quit 				= false;
	print_echo 	= false;
	
	echo 			= 0;		
	min_echo 	= 0; 	
	max_echo 	= 0;
	max		= 0;
	min		= 65535;
	duty	= 0;
	kp		= 0;
	ki		= 0;
	kd		=	0;
	
	interrupt_count			= 0;
	timer_value_current	= 0;
	set_point 			 		= 50;
	
	/* Calling initializing functions*/
	System_Clock_Init();
	UART2_Init();
	setupFanController();
	setupUltrasonicEcho();
	setupUltrasonicTrigger();
	
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


/* Activated with interrupt of Ultrasonic Sensor 
*  if rising edge, store the value, else calculate echo
*  print echo only if necessary
*/

void getEcho(void){
	if((TIM4->SR&TIM_SR_CC1IF) != 0){
		print("metemano\n\r");
		interrupt_count++;
		
		if(interrupt_count%2!=1){
			timer_value_last 		= TIM4->CCR1;	
		}else{
			
			interrupt_count = 0;
			
			timer_value_current = TIM4->CCR1;
			echo_time_on = timer_value_current - timer_value_last;
			
			/*
			According to the datasheet:
			http://www.micropik.com/PDF/HCSR04.pdf
			
			Distance = High_level_time * velocity of sound / 2 ... where:
			velocity of sound = (340m/s)
			high level time		= (PSC/80E6)(counter value <= echo_time_on) 
			*/	
			//echo_time_on = (ULTRASONIC_ECHO_PSC / SYS_FREQ)*(echo_time_on);
			
			/*
			while(!(GPIOB->IDR & GPIO_IDR_IDR_6));
			*/
			
			
			echo = echo_time_on;
		}
		
		if(print_echo){
			print_echo_counter++;
			if(print_echo_counter >= MAX_COUNTER){
				print_echo_counter = 0;
				displayEcho();
			}
		}
		
	}

}


void getMaxEcho(void){
	TIM5->CCR1 	= MIN_DUTY;
	max_echo		= 0;
	temp1 			= 0;
	
	/* max_echo = echo until value does not change for a while*/
	while (temp1<MAX_COUNTER){
		getEcho();
		if(echo>max_echo){
			max_echo = echo;
			temp1 = 0;
		}else{
			temp1++;
		}	
	}
}

void getMinEcho(void){
	TIM5->CCR1 = MAX_DUTY;
	
	/* waiting for fan to take momentum*/
	for(temp1=0;temp1<5000;temp1++);
	
	min_echo 	= MAX_INT16;
	temp1 		= 0;
	
	/* min_echo = echo until value does not change for a while*/
	while (temp1<MAX_COUNTER){
		getEcho();
		if(echo<min_echo){
			min_echo = echo;
			temp1 = 0;
		}else{
			temp1++;
		}	
	}
}

