#define 	SP_STEP		(100)
#define 	SP_MAX		(1000)
#define 	SP_MIN		(0)

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
int calculated_echo;

void initialize(void);						
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
