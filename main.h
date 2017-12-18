#define 	SP_STEP			(100)
#define 	SP_MAX			(1000)
#define 	SP_MIN			(0)
#define 	MAX_COUNTER	(2500000)
#define 	MAX_INT16		(65535)
#define 	SYS_FREQ		(80000000)

#include "stm32l476xx.h"
#include "UART.h"
#include "SysClock.h"
#include "IODevices.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* Program Variables */
bool 	quit;
bool  print_echo;

int 	print_echo_counter;
int 	min_echo;
int 	max_echo;

int 	n;													
int 	i;													
char 	input_key;

/* PID variables*/
int echo;
int max;
int min;
int duty;
int set_point;
int kp, ki, kd;

int temp1, temp2;			
int timer_value_last;
int timer_value_current;
int interrupt_count;

float echo_time_on;									// time the echo signal from the ultrasonic sensor is on, Echo Time On

/* Setup functions*/
void initialize(void);						
void getMaxEcho(void);
void getMinEcho(void);

void print(char* string);					
void checkUserInput(void);				

/* User input functions*/
void displayEcho(void);
void displayHelp(void);
void printCurrentValues(void);
void changeSetPoint(void);
void increaseSetPoint(void);
void decreaseSetPoint(void);


void getEcho(void);

uint8_t buffer[20];
