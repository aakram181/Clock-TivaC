#include <stdio.h>
#include <stdint.h>
#include "stdbool.h"
#include "stdint.h"
#include "driverlib/pin_map.h" 
#include "inc/hw_memmap.h" 
#include "inc/hw_types.h" 
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"



/*** PORTD***/
#define REGISTER_SELECT GPIO_PIN_2
#define READ_WRITE GPIO_PIN_3
#define ENABLE GPIO_PIN_6
/**PORT B***/
#define D0 GPIO_PIN_0
#define D1 GPIO_PIN_1
#define D2 GPIO_PIN_2
#define D3 GPIO_PIN_3
#define D4 GPIO_PIN_4
#define D5 GPIO_PIN_5
#define D6 GPIO_PIN_6
#define D7 GPIO_PIN_7

/**COMMANDS***/
#define CLEAR_SCREEEN 1 
#define CURSOR_SECOND_LINE 0xC0
#define RETURN_HOME 0x02

void LCDCMD(unsigned char);
void LCDPrintChar (unsigned char);
void LCDPrintString(char *);
void LCDInit(void);