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
const static char cities[10][10] = {"London", "Paris", "Madrid", "Rome", "Athens", "Ankara", "Istanbul","Cairo", "Moscow", "Tehran"};
const static int timediff[] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 4};
xQueueHandle xQueue1 ;
xQueueHandle xQueue2 ;
SemaphoreHandle_t xSemaphore0;

unsigned int selection;
static void Task2(void *pvParameters);
static void Task3(void *pvParameters); 
static void Task1(void *pvParameters);
typedef struct Message
{
unsigned char hh;
unsigned char mm;
unsigned char ss;
} AMessage;