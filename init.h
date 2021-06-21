/**
 * This header file contains the library includes, function prototypes, and the definitions that
 * belong to the Main module. This file also defines all the global variables that will be shared among different functions.
 * It will be included in main.c.
 * 
 */
#include <stdint.h>
#include "FreeRTOS.h"
#include "driverlib/pin_map.h" 
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <stdio.h>
#include "inc/hw_memmap.h" 
#include "inc/hw_types.h" 
#include "inc/hw_gpio.h"
#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "stdint.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "semphr.h"
#define ONE_SECOND 1000/3

const static char cities[10][10] = {"London", "Paris", "Madrid", "Rome", "Athens", "Ankara", "Istanbul","Cairo", "Moscow", "Tehran"}; // Cities to print time
const static int timediff[] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 4}; // Time difference in each city compared to London
xQueueHandle xQueue1 ; // Will be shared between task 1 and 3
xQueueHandle xQueue2 ; // Will be shared between task 1 and 2
SemaphoreHandle_t xSemaphore0; // Used to give from ISR to change time

unsigned int selection; // City selection
static void Task2(void *pvParameters);
static void Task3(void *pvParameters); 
static void Task1(void *pvParameters);
void Read_Time(char *);

typedef struct Message // The time data structure
{
unsigned char hh;
unsigned char mm;
unsigned char ss;
} AMessage;