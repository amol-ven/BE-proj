void SysTick_Handler(void)
{	
	//lcd_putuint(ALL_PROCESSES);
	//LPC_GPIO2->FIOSET =0xffffffff;
	//SCB->ICSR|=(1<<SCB_ICSR_PENDSVSET_Pos);		// Trigger Pend SV
	//A_SYSCLK += SysTick->VAL;
	//lcd_putstring(1, "updated @ SYSTICK Handler");
	A_SYSCLK = A_SYSCLK+A_systickReloadValue[A_currentTask];
	//lcd_putuint(A_currentTask);
	//asm volatile ("PUSH {LR}");
	//delay(10000000);
	//lcd_putuint(A_currentTask);
	//asm volatile ("POP {LR}");
	//A_SYSCLK++;
	A_TriggerPendSV();
	
	
}

void PendSV_Handler(void) __attribute__ ( (naked) );
void PendSV_Handler(void)
{
//Store Multiple Registers Decrement Before
	//LPC_GPIO2->FIOSET =0xffffffff;
	asm volatile ("STMDB SP!, {R4-R11}");
//	asm volatile ("push {R13}");  Stack Pointer  -> Do not Push
	
	//Record where MSP was left
	asm volatile ("MRS R3, MSP");
	asm volatile("mov %0, R3" : "=r"(MSP_copy) :  : );   //move register into integer!
	A_taskStaskPointer[A_currentTask]=MSP_copy;
	
	//Preserve LR across function calls
	
//	LPC_GPIO0->FIOSET = (1<<28);
	
	asm volatile ("PUSH {LR}");
	//lcd_clear();
	//lcd_putuint(A_SYSCLK);
	A_scheduler();
	asm volatile ("POP {LR}");
	
	/*
	if(A_currentTask == 0xFFFFFFFF)
	{
		asm volatile ("MRS R3, MSP");
		asm volatile("mov %0, R3" : "=r"(MSP_copy) :  : );   //move register into integer!
	
		*( &idleStack[39]-1 ) = A_taskIdle; // copying function pointer
		*( &idleStack[39]   ) = *(MSP_copy+15);                       // copying xPSR
		
		asm volatile ("mov R4, %0" :  : "r"(&idleStack[39]-15) : );   //move integer into register!
		asm volatile ("MSR MSP, R4");
		
		//Load Multiple Registers Increment After
		asm volatile ("LDMIA SP!, {R4-R11}");	
	
		//Return from exception handler
		asm volatile ("bx lr");
	}
	*/
	
	if(A_taskFirstRun[A_currentTask]==1)
	{
		asm volatile ("MRS R3, MSP");
		asm volatile("mov %0, R3" : "=r"(MSP_copy) :  : );   //move register into integer!
	
		*( A_taskInitialTOS[A_currentTask]-1 ) = A_taskFunctionPointer[A_currentTask]; // copying function pointer
		*( A_taskInitialTOS[A_currentTask]   ) = *(MSP_copy+15);                       // copying xPSR

/*************************************************
 * if task stack is of 200 locations then:
 * 
 * taskstack[198]=function;
 * taskstack[199]=*(MSP_copy+15);   that is the xPSR
 * 
 * and then put address of 199-15 in MSP i.e.  
 * asm volatile ("mov R4, %0" :  : "r"(&taskstack[184]) : );   //move integer into register!
   asm volatile ("MSR MSP, R4");
 * *************************************************/

		SysTick->LOAD = A_systickReloadValue[A_currentTask];
		SysTick->VAL = 0;

		asm volatile ("mov R4, %0" :  : "r"(A_taskInitialTOS[A_currentTask]-15) : );   //move integer into register!
		asm volatile ("MSR MSP, R4");
		A_taskFirstRun[A_currentTask]=0;
	
		//Load Multiple Registers Increment After
		asm volatile ("LDMIA SP!, {R4-R11}");	
	
		//Return from exception handler
		asm volatile ("bx lr");
	}



	SysTick->LOAD = A_systickReloadValue[A_currentTask];
	SysTick->VAL = 0;
	//0222
	//LPC_GPIO0->FIOCLR = (1<<28);
	
	MSP_copy=A_taskStaskPointer[A_currentTask];
	asm volatile ("mov R3, %0" :  : "r"(MSP_copy) : );   //move integer into register!
	asm volatile ("MSR MSP, R3");
	
	
	//Load Multiple Registers Increment After
	asm volatile ("LDMIA SP!, {R4-R11}");
	
	//asm volatile ("mov lr, %0" :  : "r"(MAIN_RET) : );   //move integer into register!
	asm volatile ("bx lr");
}


