#include "UART.h"

/**
* Enables UART0 along with GPIOA Pins 0 and 1.
* Configures UART with baud rate 6900, length 8, 1 stop bit, and no parse bits
* Configures Pins 0 and 1 as UART Rx and Tx pins
* Registers the priorirt for UART recieve with ISR "UART_ISR"
* Sets priority to interrupt as '5', which is greater than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 
*/
void UARTInit(){
  //clock to port A and uart0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){ }
  
  UARTDisable(UART0_BASE);
  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  UARTEnable(UART0_BASE);
	UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX7_8,UART_FIFO_RX1_8);
  //Configure pins
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  
  //Enable Alternate functions
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  
  UARTFIFOEnable(UART0_BASE);
	UARTIntEnable(UART0_BASE,UART_INT_RX);
	UARTIntRegister(UART0_BASE,UART_ISR);
	IntPrioritySet(INT_UART0_TM4C123 ,5<<5);
}


/**
* Checks for characters and returns them from UART0
* @return character received from UART
*/
unsigned char UARTRx(){
  //check for characters in fifo
  while(!UARTCharsAvail(UART0_BASE)){}
  return UARTCharGet(UART0_BASE);
}

/**
* Sends character through UART to be printed on screen
* @params unsigned char c is the character passed to be sent 
*/
void UARTPrintChar(unsigned char c){
      UARTCharPut(UART0_BASE,c);

}




/**
* Sends a string through UART to be printed on screen by continuously calling UARTPrintChar on every character in the string.
* @params unsigned char *str is the string passed to be sent 
*/
void UARTPrintString(unsigned char *str){
  while(*str)
  {
      UARTCharPut(UART0_BASE, *(str++));
  }
}