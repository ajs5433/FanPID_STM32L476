#include "IODevices.h"


void setupFanController(void){
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	///                     						FAN CONTROLLER - PA0        											      ///
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	  	//ENABLING PORT A GPIO CLOCK					A3     A2     A1      A0
	GPIOA->MODER = 0x802;			   							//ALTERNATE FUCTION AF1 //802         10     00     00      10            <-10 is Alternate Function
	GPIOA->AFR[0]=0x111112;										//SETTING AF2 AS THE SELECTED ALTERNATE FUNC
	
	//GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR6); 	//Pull up or pull down, do not make any difference
	GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR6_1; 
	
	//CLOCK AND TIMERS
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;	  	//ENABLING TIME CLOCK
	TIM5->PSC = 4-1;												//80MHz / 4000 = 20 kHz;  	
	TIM5->ARR = 1000;   										//SETTING THE FREQUENCY
	TIM5->CCR1 = 500;												//SETTING SERVO 1 POSITION 1 DEFAULT
	
	TIM4->CCMR1 |= 0x01;                      // SELECT ACTIVE INPUT AS CH1
	
	//PWM						
	TIM5->CCMR1 |=  0x70; 							      			//6 SETTING PWM MODE-2
  TIM5->CCMR1 |= TIM_CCMR1_OC1PE;  								//ENABLING PRELOAD REGISTER
	TIM5->CR1 	|= TIM_CR1_ARPE ;						    		//SETTING THE APRE TO 1 AND SINCHRONIZE WITH PSC
	TIM5->CCER 	|= TIM_CCER_CC1E | TIM_CCER_CC1P;		// - CAPTURE/COMPARE ENABLE - 0 ACTIVE HIGH 1 ACTIVE LOW - SIGNAL OUTPUT ( 0 NOT ACTIVE)   
	//TIM2->BDTR |= TIM_BDTR_OSSI | TIM_BDTR_OSSR | TIM_BDTR_MOE;				//this was set up but then commented because TIM2 doesnt have BDTR register on the reference manual	
	
	TIM5->CR1 |= TIM_CR1_CEN;							
	TIM5->EGR |= TIM_EGR_UG;					
}


void setupUltrasonicTrigger(void){
	///////////////////////////////////////////////////////////////////////////////////////////////
	///                     						ULTRASONIC SENSOR - PA5      											      ///
	///////////////////////////////////////////////////////////////////////////////////////////////

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	  	//ENABLING PORT A GPIO CLOCK
	GPIOA->MODER = 0x802;			   							//ALTERNATE FUCTION AF2
	GPIOA->AFR[0]= 0x111112;										
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR6);
	
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;	  	
	TIM2->PSC 	= 80-1;//(TIM_FREQ)/(CNT_FREQ);  	
	TIM2->ARR 	= 65500; 											//SETTING THE FREQUENCY
	TIM2->CCR1 	=    10;												//SETTING SERVO 1 POSITION 1 DEFAULT
	
	TIM2->CCMR1 |=  0x70; 										
  TIM2->CCMR1 |= TIM_CCMR1_OC1PE;  					
	TIM2->CR1 	|= TIM_CR1_ARPE ;								
	TIM2->CCER 	|= TIM_CCER_CC1E | TIM_CCER_CC1P;	
	
	//INICIALIZING TIMERS
	TIM2->CR1 |= TIM_CR1_CEN;							//ENABLING COUNTERS 
	TIM2->EGR |= TIM_EGR_UG;							//UPDATE BIT		

}

/**/
void setupUltrasonicEcho(void){
	
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN;
																							// set pin PB6 in alternate function mode
	GPIOB-> MODER &= ~(0X03<<(2*6));  					//CLEAR 12TH AND 13TH BIT
	GPIOB-> MODER |= 0X02<<(2*6);
	GPIOB-> AFR[0] |= 0X2<<(4*6);								// PIN PB6 AS AF TIM4
	
	//RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;				// *
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;				// ENABLE CLOCK FOR TIMER 4
	TIM4->PSC = 160-1;                          // SET PSC
	// TIM4->EGR  |= TIM_EGR_UG;                // LOAD PSC
	// TIM4->CCER&= ~(0xFFFFFFFF);
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;							// SET DIRECTION AS INPUT
	TIM4->CCMR1 |= 0x01;                        // SELECT ACTIVE INPUT AS CH1
	TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;							// DISABLE DIGITAL FILTERING
	
	TIM4->CCER|= (1<<1 | 1<<3);      						// SELECT BOTH RISING AND FALLING EDGES
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);					// PROGRAM INPUT PSC TO CAPTURE EACH TRANSITION
		
	TIM4->CCER	|= TIM_CCER_CC1E;								// ENABLE CAPTURE OF COUNTER
	TIM4->DIER	|= TIM_DIER_CC1IE;							// ENABLE RELATED INTERRUPTS

	TIM4->CR1 |= TIM_CR1_CEN;

	//NVIC_SetPriority(TIM4_IRQn,1);
	//NVIC_EnableIRQ(TIM4_IRQn);
	
}




/*

void setupUltrasonicEcho(void){
		
	
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN;
																								// set pin PB6 in alternate function mode
    GPIOB-> MODER &= ~(0X03<<(2*6));  					//CLEAR 12TH AND 13TH BIT
    GPIOB-> MODER |= 0X02<<(2*6);
    GPIOB-> AFR[0] |= 0X2<<(4*6);								// PIN PB6 AS AF TIM4
    RCC-> APB1ENR1 |= RCC_APB1ENR1_TIM4EN;			// ENABLE CLOCK FOR TIMER 4
    
    TIM4->PSC= ULTRASONIC_ECHO_PSC;             //prescaler
    TIM4->EGR  |= TIM_EGR_UG;                   //Load new prescaler
    TIM4->CCER&= ~(0XFFFFFFFF);
    TIM4->CCMR1 |= 0X1;                         //select ch1
    TIM4->CCER |= (1<<1 | 1<<3);      					//To select only rising edges
    TIM4->CCER|= 0X1;
    TIM4->CR1 |= TIM_CR1_CEN;
	
	
	
	//GPIO PIN AND CONFIGURATION -------------------------------------
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //|| RCC_AHB2ENR_GPIOAEN);
	GPIOB-> MODER &= ~(0X03<<(2*6));  					//CLEAR 12TH AND 13TH BIT
	GPIOB-> MODER |= 0X02<<(2*6);
	GPIOB-> AFR[0] |= 0X2<<(4*6);			//Alternate Fucntion 2
	//GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR6); 		//Check for pin.
	
	//Enable clock
	RCC-> APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	TIM4->PSC = 80;
	//TIM4->ARR = 200-1; 											
	//TIM4->CCR1 = 1940;	
	
	//LOAD NEW PRESCALER
	TIM4->EGR  |= TIM_EGR_UG;
	
	//SET THE DIRECTION AS INPUT AND SELECT THE ACTIVE INPUT
	//TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |=0x1;
	
	//PROGRAM THE INPUT FILTER DURATION
	//TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
		
	//TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;	

	TIM4->CCER&= ~(0XFFFFFFFF);
	//EDGE OF THE ACTIVE TRANSITION
	TIM4->CCER |= (1<<1 | 1<<3);
	//ENABLE CAPTURE OF THE COUNTER
	TIM4->CCER |= TIM_CCER_CC1E;
	
	//PROGRAM THE INPUT PRESCALER
	//TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);
		
	//ENABLE RELATED INTERRUPTS
	//TIM4->DIER |= TIM_DIER_CC1IE;
	
	//CC1DE: CAPTURE/COMPARE DMA REQUEST ENABLE FOR CHANNEL 1
	//TIM4->DIER |= TIM_DIER_CC1DE;
	
	//ENABLE THE COUNTER
	TIM4->CR1 |= TIM_CR1_CEN;
	
	//SET PRIORITY TO 1
	//NVIC_SetPriority(TIM4_IRQn, 1);
	
	//ENABLE TIM4 INTERRUPT IN NVIC	
	//NVIC_EnableIRQ(TIM4_IRQn);					
	
}
*/
