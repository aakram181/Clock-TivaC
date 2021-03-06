#include "LCD.h"




/**
* Passes a command to the LCD as unsigned char - list of commands and what they do can be found in the LCD data sheet
* @param cmd is the command to be passed
*/
void LCDCMD(unsigned char cmd) {
		GPIOPinWrite(GPIO_PORTA_BASE,REGISTER_SELECT|READ_WRITE,!REGISTER_SELECT|!READ_WRITE); // RS= 0 , RW = 0
		GPIOPinWrite(GPIO_PORTB_BASE,D0|D1|D2|D3|D4|D5|D6|D7,cmd); // write command
		GPIOPinWrite(GPIO_PORTA_BASE,ENABLE,ENABLE); // E = 1
    SysCtlDelay(10);
    GPIOPinWrite(GPIO_PORTA_BASE,ENABLE,!ENABLE); // E = 0
    SysCtlDelay(5000);

}

/**
* Prints one character on the LCD
* @param data the character to print
*/
void LCDPrintChar (unsigned char data) {
		GPIOPinWrite(GPIO_PORTA_BASE,REGISTER_SELECT|READ_WRITE,REGISTER_SELECT|!READ_WRITE); // RS= 1, RW = 0
		GPIOPinWrite(GPIO_PORTB_BASE,D0|D1|D2|D3|D4|D5|D6|D7,data); //write text
		GPIOPinWrite(GPIO_PORTA_BASE,ENABLE,ENABLE); // E = 1
    SysCtlDelay(10);
		GPIOPinWrite(GPIO_PORTA_BASE,ENABLE|REGISTER_SELECT,!ENABLE|!REGISTER_SELECT); // E = 0
    SysCtlDelay(500);

}

/**
* Prints a string of characters on the LCD by continuously calling LCDPrintChar() on each character in the string
* @param data the character to print
*/
void LCDPrintString(char *text){
	for(int i = 0 ; i < strlen(text); i++){
		LCDPrintChar(text[i]);
	}
}


/**
* Initialized GPIOA and GPIOB and the pins that will be used with the LCD.
* Adds delays between commands as instructed in the LCD's data sheet.
*/
void LCDInit(){
	// Initializing ports and pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)); 	
  GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, (D0|D1|D2|D3|D4|D5|D6|D7));
  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, (REGISTER_SELECT|READ_WRITE|ENABLE));
	
		SysCtlDelay(20000);    
	
		LCDCMD(0x30);		
		SysCtlDelay(50000);    
	
    LCDCMD(0x30);		
		SysCtlDelay(50000);    
	
		LCDCMD(0x30);		
		SysCtlDelay(50000); 
	
		// Prepare LCD Operation and Function  
    LCDCMD(0x38);      // 8 bit 2 lines
    LCDCMD(0x06);      // increment cursor to right
    LCDCMD(0x0F);      // display on, cursor blinking
    LCDCMD(0x01);      // clear display
	
}