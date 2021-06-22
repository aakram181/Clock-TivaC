#include "init.h"
#include "LCD.h"
#include "UART.h"


int main(){
	xQueue1 = xQueueCreate(1,8); // Create queue size 1, 8 bytes
	xQueue2 = xQueueCreate(1,8); // Create queue size 1, 8 bytes
	xSemaphore0 = xSemaphoreCreateBinary(); // Create Binary Sempahore
	
	//Create 3 tasks and set all their priorities to 1
	xTaskCreate(Task3, "UART Controller",250,NULL,1,NULL); 
	xTaskCreate(Task2, "LCD_Controller", 250, NULL,1,NULL);
	xTaskCreate(Task1, "Time_Increment", 250, NULL,1,NULL);

	vTaskStartScheduler();
	
	// Should never reach here
	while(1){}
}


/**
* Reads time as characters entered by user through UART until user clicks 'enter'.
* Stores the read characters in the buf[] parameter.
* Reads in hh:mm:ss format
*@params buf[] is the buffer array where the characters entered by user will be stored and later converted to time.
*/
void Read_Time(char buf[]){
	unsigned char c;
	unsigned char k = 0;
	while(c != 0x0d){ 	// While entered character isnt the button 'enter'
		c = UARTRx(); 		// Receive character from UART
		UARTPrintChar(c); // Print char on pc
		buf[k] = c;
		k++;
	}
	buf[k] = '\0'; // add end char
}




	
	
/**
* Converts byte to string of two characters
*
*@params c: Is the byte to be converted
*@params *Txt: Is the array that will hold the converted bytes
*/
void ByteToStr(unsigned char c, char *Txt){
	
	char d1 = c% 10;
	char d2 = (c - d1) /10;
	Txt[1] = d1 + '0';
	Txt[0] = d2 + '0';
	Txt[2] = '\0';
}


/**
* This task is responsible for incrementing the time.
* It first waits for the initial time entered by user
* It blocks itself for one second, then increments the time and sends it to task 2 via queue 1
*/
static void Task1(void *pvParameters){
	AMessage Tim;
	xQueueReceive(xQueue2, &Tim, portMAX_DELAY); 	// Receive initial time via queue 2
	while(1){
		vTaskDelay(pdMS_TO_TICKS(ONE_SECOND)); 				// Block the task for 1 second
		Tim.ss++; 																		// Increment the seconds 
		
		if(Tim.ss == 60) 													   	// If seconds = 60, reset it and add 1 to minutes, if minutes = 60, reset it and add 1 to hours
		{																							// If hours = 24, reset it back to zero
			Tim.ss = 0;
			Tim.mm ++;
			if(Tim.mm == 60){
				Tim.mm = 0;
				Tim.hh++;
				if(Tim.hh == 24)
				{
					Tim.hh = 0;
				}
			}
		}
		xQueueSend(xQueue1, &Tim,0); // Send the incremented time via queue 1
	}
}

/**
* This task is the LCD controller. It received the incremented time from task 1.
* Each time it receives the time through the queue, it prints the time and city on the LCD screen using the LCD functions.
* The part that prints the city on the LCD and sets the timezone is put in a critical section to prevent the ISR from changing the 'selection' variable
* in between the 'LCDPrintString(cities[selection])' and 'Tim.hh = (Tim.hh + timediff[selection])' which might cause an incorrect time printed for the selected city.
*/
static void Task2(void *pvParameters){
	
	
	char Txt[7]; // Will hold time as string 
	AMessage Tim; 
	
	LCDInit(); // Initialize LCD
	LCDCMD(CLEAR_SCREEEN); // Clear LCD
	selection = 0;
	
	while(1){
		xQueueReceive(xQueue1,&Tim, portMAX_DELAY);// Receive the AMessage time data structure sent by task 1 from queue

		LCDCMD(CLEAR_SCREEEN); 	//Clear screen
		LCDCMD(RETURN_HOME);    //Return cursor back to home
		
		taskENTER_CRITICAL(); // Disable interrupts while printing time to prevent 'selection' from being changed
		
		LCDPrintString(cities[selection]); // Display the selection on LCD screen
		LCDCMD(CURSOR_SECOND_LINE);		// Move cursor of screen to second line (preparing to print time)
		Tim.hh = (Tim.hh + timediff[selection]); // Add the time difference of the selection to the london time hours
		
		taskEXIT_CRITICAL();
		
		if(Tim.hh > 23) 
			Tim.hh = Tim.hh - 24; // If time.hours >= 24, subtract 24
		
		ByteToStr(Tim.hh,Txt);  // Convert hours to string and store it in Txt
		
		LCDPrintString(Txt);   // Print hours on LCD


		LCDPrintChar(':');
		
		ByteToStr(Tim.mm,Txt); // Convert minutes to string and store it in Txt
		LCDPrintString(Txt);   // Print minutes on LCD
		LCDPrintChar(':');

		ByteToStr(Tim.ss,Txt);  // Convert seconds to string and store it in Txt
		LCDPrintString(Txt);		// Print seconds on LCD
		
	}
	
}






/**
* This task first initializes UART. It then prints a welcome message and asks the user to enter time in london in hh:mm:ss format.
* Then it displays the list of available cities from the cities[] array, and prompts the user to select one of the cities, and WAITS for the sempahore
* Semaphore is received from UART ISR when the users enters a new selection on the screen. 
* Once it unblocks, it sets the new selection = character entered, and repeats the process again.
*/
static void Task3(void *pvParameters){
	AMessage Tim;
	char buf[10]; // Will hold entered time
	char k;   
	UARTInit(); // Initialize UART
	UARTPrintString("TIME IN DIFFERENT CITIES\r\n");
	UARTPrintString("========================\r\n");
	UARTPrintString("Please enter time in london (hh:mm:ss):  \r\n");
	
	Read_Time(buf); // Now it contains the time in hh:mm:ss format
	
	// Converting the array form of time into integers stored in the Tim data structure.
	Tim.hh = 10 * (buf[0] - '0') + buf[1] - '0'; 
	Tim.mm = 10 * (buf[3] - '0') + buf[4] - '0';
	Tim.ss = 10 * (buf[6] - '0') + buf[7] - '0';
	xQueueSend(xQueue2, &Tim,0);       // Send time to Task 1 via queue 2

	
	while(1){
		UARTPrintString("Select a city to display time:\r\n");

		for(k = 0; k < 10; k++){  // This loop prints the cities from cities array
			UARTPrintChar(k + '0'); 
			UARTPrintString(". ");

			UARTPrintString(cities[k]);
			UARTPrintString("\r\n");
		}
		UARTPrintString("Selection:  ");
		xSemaphoreTake(xSemaphore0,portMAX_DELAY);
		UARTPrintChar(selection);
		UARTPrintString("\n\r");

		selection = selection - '0'; // Entered character is the new city selection

		//selection = Read_From_Keyboard();  // Wait for user to enter city selection
}
}

/**
* Triggered when user enters a character through the UART (on putty screen)
* It unblocks task 3, which is responsible for UART to print the character which will consequently set the new selection as the entered character
*/
void UART_ISR(){
	selection = UARTCharGetNonBlocking(UART0_BASE);
	UARTIntClear(UART0_BASE,UART_INT_RX);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(xSemaphore0,&xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
