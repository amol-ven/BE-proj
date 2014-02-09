void A_taskIdle();
void A_taskUART();


void A_init()
{
	int i;
	A_uartSendIndex=0;
	UARTInit( 1, 9600 );
	UARTInit( 0, 9600 );
	UARTSend(1, "hi", 2);
	for(i=0;i<ALL_PROCESSES;i++)
	{
		A_taskFirstRun[i]=1;
		A_taskSleeping[i]=0;
		A_taskAlarm[i] = 0;
		A_taskNames[i][0]='\0';
		A_taskEnable[i]=1;		// All task are enabled initially
	}
	//A_taskSleeping[IDLE_TASK_INDEX]=1;
	
}

void A_taskCreate( void (*fp)(), uint32_t* SPaddress, uint32_t reload)
{
	A_taskFunctionPointer[A_tasksCreatedSoFar]=fp;
	A_taskInitialTOS[A_tasksCreatedSoFar]=SPaddress;
	A_systickReloadValue[A_tasksCreatedSoFar]=reload;
	
	A_tasksCreatedSoFar++;
}

void A_start()
{	
	A_taskCreate(A_taskUART, &A_taskUARTstack[99],0xFFFF);
	A_taskCreate(A_taskIdle, &taskstackidle[99], 0xFFF);
	A_SYSCLK=0;
	SysTick->CTRL |= ST_CTRL_ENABLE; 		//Enable system tick timmer
	SysTick->CTRL |= ST_CTRL_TICKINT;  			// ENABLE TIMMER INTERRUPT
    SysTick->CTRL |= ST_CTRL_CLKSOURCE; 		//TO SELECT INTERNal CLOCK AS SOURCE
	SysTick->LOAD = 0x00FFFFFF;		//RELOAD VALUE OF 100 IN HEX 64
	
}


void A_taskQuit()
{	
	A_SYSCLK = A_SYSCLK+(A_systickReloadValue[A_currentTask]-SysTick->VAL);
	SysTick->VAL = 0;     //writes data in current so that current is cleared.
	A_TriggerPendSV();
}

void A_setTaskName(char* name)			// to set task name
{
	copystring(A_taskNames[A_currentTask], name);
	
}

void A_taskIdle()
{
	while(1)
	{
		
			//UARTSend(0, "idle\n\r", 6);
	}
}

void A_taskUART()
{
	while(1)
	{
		while(A_uartSendIndex!=UART0Count)
		//while(A_uartSendIndex<UART0Count)
		{
			if(UART0Buffer[A_uartSendIndex]==0x08)
			{
				UARTSend(0,&UART0Buffer[A_uartSendIndex],1);
				UARTSend(0," ",1);
				UARTSend(0,&UART0Buffer[A_uartSendIndex],1);
			}
			else if(UART0Buffer[A_uartSendIndex]==0x0D)
			{
				UARTSend(0,"\n\r",2);
				//A_commandFlag=1;	//A command has been received to be executed
			}
			else
			{
				UARTSend(0,&UART0Buffer[A_uartSendIndex],1);
			}
			A_uartSendIndex++;
			if(A_uartSendIndex==BUFSIZE)
			{
				A_uartSendIndex=0;
			}
		}
		
		//while(A_customUartSend)
		A_taskEnable[A_currentTask]=0;
		A_taskQuit();
	}
}
