#include "UART.h"


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



unsigned char UARTRx(){
  //check for characters in fifo
  while(!UARTCharsAvail(UART0_BASE)){}
  return UARTCharGet(UART0_BASE);
}


void UARTPrintChar(unsigned char c){
      UARTCharPut(UART0_BASE,c);

}





void UARTPrintString(unsigned char *str){
  while(*str)
  {
      UARTCharPut(UART0_BASE, *(str++));
  }
}