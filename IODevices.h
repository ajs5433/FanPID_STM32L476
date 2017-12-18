#include "stm32l476xx.h"

#define MAX_DUTY 									(9950)
#define MIN_DUTY 									(0)

#define ULTRASONIC_TRIGGER_PSC		(800-1)
#define ULTRASONIC_ECHO_PSC				(80)
#define FAN_PSC										(64000-1)


void setupFanController(void);
void setupUltrasonicTrigger(void);
void setupUltrasonicEcho(void);

