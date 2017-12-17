#include "stm32l476xx.h"
#include "UART.h"
#include "SysClock.h"

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


uint8_t buffer[20];
