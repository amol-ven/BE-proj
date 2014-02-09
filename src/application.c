#include <LPC17xx/LPC17xx.h>
#include "inc/lcd.c"

//#include "inc/led/led.c"

#define MAX_PROCESSES 5

#include "Kernel_headers/KernelIncludes.h"
uint32_t taskstack0[200];	// LED
uint32_t taskstack1[200];	//
uint32_t taskstack2[200];	//
uint32_t taskstack3[200];	//
uint32_t taskstack4[200];	//

A_MBOX mybox;
A_MUTEX uartmut;

void _delay(uint32_t del) 
{
    volatile uint32_t i=0;
    while(i++<=del);
}

void function0(void)
{
	uint16_t j=0;
	A_setTaskName("zero");
	lcd_putstring(0,"    A_Kernel    ");
	lcd_putstring(1,"  Real Time OS  ");
	
	uint32_t i=0;
	A_taskSleep(500000000);
	lcd_clear();
	
	char* str=NULL;
	do
	{
		str=A_checkMail(&mybox);
	}
	while(str==NULL);
	
	
	while(1)
	{	
		//lcd_clear();
		A_mutexAcquire(&uartmut);
		
	
		lcd_gotoxy(0,0);
		lcd_putuint(i++);
		A_mutexRelease(&uartmut);
		//_delay(100000);
		UARTSend(0, str, *(str-3) );
		A_taskSleep(10000000);
		
	//	A_taskSleep(30000000);
	}
}

void function1(void)
{
	A_setTaskName("one");
	
	A_sendMail("MAILBOX", &mybox);
	uint32_t i=0;
	char c[]="1";
	while(1)
	{
		LPC_GPIO0->FIOSET = (1<<27);
		A_taskSleep(40000000);
		LPC_GPIO0->FIOCLR = (1<<27);
		A_taskSleep(40000000);
	}
}

void function2(void)
{
	uint16_t j=0;
	A_setTaskName("two");
	
	while(1)
	{	
		
		LPC_GPIO0->FIOSET = (1<<28);
		A_taskSleep(50000000);
		LPC_GPIO0->FIOCLR = (1<<28);
		A_taskSleep(50000000);
		
		
		
	}
}

void function3(void)
{
	A_setTaskName("three");
	int i;
	i=0;
	while(1)
	{

		
		LPC_GPIO2->FIOSET = (1<<11);
		A_taskSleep(30000000);
		LPC_GPIO2->FIOCLR = (1<<11);
		A_taskSleep(40000000);
	}
}
void function4(void)
{
	
	A_setTaskName("four");
	
	while(1)
	{
	
		LPC_GPIO2->FIOSET = (1<<9);
		A_taskSleep(40000000);
		LPC_GPIO2->FIOCLR = (1<<9);
		A_taskSleep(40000000);
	}
}



int main (void) 
{ 
    LPC_SC->PCONP = 0xFFFFFFFF; // power up GPIO
    LPC_GPIO2->FIODIR = 0xFFFFFFFF; // puts P1.29 into output mode.
	LPC_GPIO2->FIOPIN = 0xFFFFFFFF;
	LPC_GPIO0->FIODIR = 0xFFFFFFFF; //put port0 in output mode
	LPC_GPIO0->FIOPIN = 0;
	
	init_lcd();
		
	char line[] = "myline";
	
	
	asm volatile ("mov R6, %0" :  : "r"(43) : );   //move integer into register!
	A_mutexInit(&uartmut);
	A_MBoxInit(&mybox, 0);
	
	A_init();
	A_taskCreate(function0, &taskstack0[199], 0x000FFFFF);
	A_taskCreate(function1, &taskstack1[199], 0x000FFFFF);
	A_taskCreate(function2, &taskstack2[199], 0x000FFFFF);
	A_taskCreate(function3, &taskstack3[199], 0x000FFFFF);
	A_taskCreate(function4, &taskstack4[199], 0x000FFFFF);
	
	A_start();
	while(1)
	{
		
	}
}	
