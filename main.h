#include "stm32l476xx.h"
#include "UART.h"
#include "SysClock.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* Program Variables */
int 	string;
char 	input;
int n;
int i;
void initialize(void);
uint8_t buffer[20];
