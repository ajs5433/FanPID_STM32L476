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
	
	
	while(1){
	if((USART2->ISR & USART_ISR_RXNE)) // Wait until RXNE (RX not empty) bit is set
	{
			// USART resets the RXNE flag automatically after reading DR
			//printStr("Got new char\r\n");
			input = (uint8_t)(USART2->RDR & 0xFF);
		
			n = sprintf((char *)buffer, "something pressed! yet\n\r");
			USART_Write(USART2,buffer , n);
	
	}
		
	//int n;
	n = sprintf((char *)buffer, "nothing pressed yet\n\r");
	
	USART_Write(USART2,buffer , n);
	
	//int i;
	for (i=0;i<50000;i++);
}
	/*
  // Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
	
  // wait until HSI is ready
  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 ) {;}
	
  // Select HSI as system clock source 
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;  //01: HSI16 oscillator used as system clock

  // Wait till HSI is used as system clock source 
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}
  
  // Enable the clock to GPIO Port B	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;   

  // MODE: 00: Input mode, 01: General purpose output mode
  //       10: Alternate function mode, 11: Analog mode (reset state)
  GPIOB->MODER &= ~(0x03<<(2*2)) ;   // Clear bit 13 and bit 12
  GPIOB->MODER |= (1<<4);
		
  GPIOB->ODR |= GPIO_ODR_ODR_2;
  
  // Dead loop & program hangs here
  while(1);
	*/
}

void initialize(){
	/* Calling initializing functions*/
	UART2_Init();
	
	/* Initializing program variables*/	
}

