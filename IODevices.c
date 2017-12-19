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

/*
void setupUltrasonicEcho(void){
	
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN;
																							// set pin PB6 in alternate function mode
	GPIOB-> MODER &= ~(0X03<<(2*6));  					//CLEAR 12TH AND 13TH BIT
	GPIOB-> MODER |= 0X02<<(2*6);
	GPIOB-> AFR[0] |= 0X2<<(4*6);								// PIN PB6 AS AF TIM4
	
	//RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;				// 
	
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


*/



void setupUltrasonicEcho(void){
	  RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN;        // Enable GPIOB clock 
    GPIOB->MODER   &=   ~(0x03 << 12);     		// Clear bit 12 & 13 Alternate function mode 
    GPIOB->MODER   |=   (0x02 << 12);         // set as Alternate function mode 
    GPIOB->OSPEEDR &=   ~(0x03<< 12);         // 40 MHz  speed 
    GPIOB->OSPEEDR |=   (0x03<< 12);          // 40 MHz  speed 
    GPIOB->PUPDR &=         ~(0X3<<12);       // NO PULL-UP PULL-DOWN 
    GPIOB->OTYPER &=        ~(1<<6);          // PUSH-PULL 
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6;  			// Clear pin 6 for alternate function
    GPIOB->AFR[0] |=        0x2 << (4*6);     // set PB pin 6 as AF2 (TIM4_CH1) 

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;      // ENABLE TIM4 CLOCK
    TIM4->PSC = 15;                           // SET APPROPRAIT PRESCALER TO SLOW DOWN THE CLOCK                                              
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;           // CLEAR CAPTURE/COMPARE REGISTER
    TIM4->CCMR1 |= 0X1;                       // SELECT CH1 INPUTE CAPTURE 
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;           // DISABLE DIGITAL FILTERING
    TIM4->CCER |= (1<<1 | 1<<3);              // SELECT BOTH RISING AND FALLING EDGE DETECTION CC1P & CC1NP
    TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);       // INPUT PRESCALER 0 TO CAPTURE EACH VALID EDGE
    TIM4->CCER |= TIM_CCER_CC1E;              // ENABLE COUNTER CAPTURE
    TIM4->DIER |= TIM_DIER_CC1IE;             // ENABLE CH1 CAPTURE/COMPARE INTERRUPT
    TIM4->DIER |= TIM_DIER_CC1DE;   
    TIM4->DIER |= TIM_DIER_UIE;								// UPDATE INTERRUPT ENABLE
    TIM4->CR1 &= ~TIM_CR1_DIR;                // Set downcounting counter direction
    TIM4->CR1 |= TIM_CR1_CEN;                 // Enable the counter
    NVIC_SetPriority(TIM4_IRQn, 1);          	// SET PRIORITY TO 1
    NVIC_EnableIRQ(TIM4_IRQn);  	
}

void TIM4_IRQHandler(void){

    if ((TIM4->SR & TIM_SR_UIF) != 0){                  // Check the update event flag
        overflow++;                                 	// if UIF = 1, increment overflow counter
        TIM4->SR &= ~TIM_SR_UIF;                        // clear UIF
    }
    if ((TIM4->SR & TIM_SR_CC1IF) != 0){                // Check capture event flag 
    newcounter = TIM4->CCR1;                                        // read capture value, store as newcounter
    timespan = (newcounter - lastcounter)+(65536 * overflow);   // calculate the total pulse width
    lastcounter = newcounter;                               // save the value of newcounter as lastcounter to be used for the next cycle
    overflow = 0;                                                       // clear overflow counter
    }

}
