#include "IODevices.h"

void setupFanController(void){
	///////////////////////////////////////////////////////////////////////////////////////////////
	///                     							ULTRASONIC SENSOR         											      ///
	///////////////////////////////////////////////////////////////////////////////////////////////

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	  	//ENABLING PORT A GPIO CLOCK
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;	  	
	TIM2->PSC = 800-1;													// One clock would be 10us
	TIM2->ARR = 6550; 													// Period is 50ms
	//TIM2->ARR = 5000; 													// Period is 50ms
	TIM2->CCR1 = 1;															// For one clock
	
	TIM2->CCMR1 |=  0x70; 										
  TIM2->CCMR1 |= TIM_CCMR1_OC1PE;  					
	TIM2->CR1 |= TIM_CR1_ARPE ;								
	TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;	
	
	//INICIALIZING TIMERS
	TIM2->CR1 |= TIM_CR1_CEN;							//ENABLING COUNTERS 
	TIM2->EGR |= TIM_EGR_UG;							//UPDATE BIT		

}

void setupUltrasonicTrigger(void){
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	///                     							  FAN CONTROLLER          											      ///
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	  	//ENABLING PORT A GPIO CLOCK					A3     A2     A1      A0
	GPIOA->MODER = 0xBFE;			   							//ALTERNATE FUCTION AF1 //802         10     00     00      10            <-10 is Alternate Function
	//GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR6); 	//Pull up or pull down, do not make any difference
	//GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR6_1; 
	
	GPIOA->AFR[0]=0x111112;										//SETTING AF1 AS THE SELECTED ALTERNATE FUNC
	
	//CLOCK AND TIMERS
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;	  	//ENABLING TIME CLOCK
	TIM5->PSC = 64000-1;											//80MHz / 64000 = 1.25 MHz;  	
	TIM5->ARR = 100000;   										//SETTING THE FREQUENCY
	TIM5->CCR1 = 1940;												//SETTING SERVO 1 POSITION 1 DEFAULT

	//PWM						
	TIM5->CCMR1 |=  0x70; 							      			//6 SETTING PWM MODE-2
  TIM5->CCMR1 |= TIM_CCMR1_OC1PE;  								//ENABLING PRELOAD REGISTER
	TIM5->CR1 |= TIM_CR1_ARPE ;						    			//SETTING THE APRE TO 1 AND SINCHRONIZE WITH PSC
	TIM5->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;		// - CAPTURE/COMPARE ENABLE - 0 ACTIVE HIGH 1 ACTIVE LOW - SIGNAL OUTPUT ( 0 NOT ACTIVE)   
	//TIM2->BDTR |= TIM_BDTR_OSSI | TIM_BDTR_OSSR | TIM_BDTR_MOE;				//this was set up but then commented because TIM2 doesnt have BDTR register on the reference manual	
	
	TIM5->CR1 |= TIM_CR1_CEN;							
	TIM5->EGR |= TIM_EGR_UG;					
}


/*
void setupUltrasonicEcho(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;				// *

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;				// ENABLE CLOCK FOR TIMER 4
	TIM4->PSC = 160-1;                          // SET PSC
	// TIM4->EGR  |= TIM_EGR_UG;                // LOAD PSC
	// TIM4->CCER&= ~(0xFFFFFFFF);
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;							// SET DIRECTION AS INPUT
	TIM4->CCMR1 |= 0x01;                        // SELECT ACTIVE INPUT AS CH1
	TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;							// DISABLE DIGITAL FILTERING

	TIM4->CCER|= (1<<1 | 1<<3);      						// SELECT BOTH RISING AND FALLING EDGES
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);					// PROGRAM INPUT PSC TO CAPTURE EACH TRANSITION
	
	// CHECK FROM HERE I THINK
	TIM4->CCER	|= TIM_CCCER_CC1E;							// ENABLE CAPTURE OF COUNTER
	TIM4->DIER |= TIM_DIER_CC1DE;
	
	TIM4->CCMR1 &= ~(TIM_CCER_CC1E);						// ENABLE 

	TIM4->CCER|= 0x01;
	TIM4->CR1 |= TIM_CR1_CEN;
	
	
}
*/

void setupUltrasonicEcho(void){
		RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN;
																								// set pin PB6 in alternate function mode
    GPIOB-> MODER &= ~(0X03<<(2*6));  					//CLEAR 12TH AND 13TH BIT
    GPIOB-> MODER |= 0X02<<(2*6);
    GPIOB-> AFR[0] |= 0X2<<(4*6);								// PIN PB6 AS AF TIM4
    RCC-> APB1ENR1 |= RCC_APB1ENR1_TIM4EN;			// ENABLE CLOCK FOR TIMER 4
    
    TIM4-> PSC=80;                              //prescaler
    TIM4->EGR  |= TIM_EGR_UG;                   //Load new prescaler
    TIM4->CCER&= ~(0XFFFFFFFF);
    TIM4->CCMR1 |= 0X1;                         //select ch1
    TIM4->CCER |= (1<<1 | 1<<3);      					//To select only rising edges
    TIM4->CCER|= 0X1;
    TIM4->CR1 |= TIM_CR1_CEN;
}

	
