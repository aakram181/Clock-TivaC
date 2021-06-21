/**
 * This header file contains the library includes, function prototypes, and the definitions that
 * belong to the UART module. It will be included in UART.c
 */


#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h" 
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "stdio.h"
#include "utils/uartstdio.h"
#include "driverlib/interrupt.h"

#include "utils/uartstdio.h"

#define GPIO_PA0_U0RX 0x00000001
#define GPIO_PA1_U0TX 0x00000401

unsigned char UARTRx(void);
void UARTPrintString(unsigned char *str);
void UARTPrintChar(unsigned char);
void UARTInit(void);
extern void UART_ISR(void);

