
		//A_taskAlarm[A_currentTask] = A_SYSCLK+1000000;void A_taskSleep(uint64_t sleeptime)

void A_taskSleep(uint32_t sleeptime )  //puts current task to sleep
{
	
	A_taskAlarm[A_currentTask] = A_SYSCLK+sleeptime;
	A_taskSleeping[A_currentTask] = 1;		// to check sleeping or awake 1=sleep
	//lcd_putchar('a');
	A_taskQuit();
}
