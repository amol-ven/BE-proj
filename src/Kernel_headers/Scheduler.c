//300000000
/*************
 * wait sources
 * 		
 * 		sleep
 * 		mutex              //not yet done
 * 		command
 *  
 ************/ 
void Killtask(int x)
	{
		A_taskEnable[x]=0; //task kill
		A_taskFirstRun[x]=1;
	}
void Suspendtask(int x)
	{
		A_taskEnable[x] = 0; //task suspended
	}
void Resumetask(int x)
	{
		A_taskEnable[x]=1; //task resume
		A_taskSleeping[x] = 0;
	}
void A_scheduler(int x)
{	
	
	
	char keyWord[BUFSIZE];
	unsigned int commandID,taskCLIID=ALL_PROCESSES;		// ALL_PROCESSES not a valild task ID
	int task,shift;
	
	if(A_commandFlag==1)
	{	
		//getNextWord(A_receivedCommand, keyWord, 0);
		//lcd_putstring(0, keyWord);
		
		//getNextWord(A_receivedCommand, keyWord, 0);
		//lcd_putstring(1, keyWord);
		
		
		
		
		getNextWord( &A_receivedCommand[0],&keyWord[0],0);
		//lcd_putstring(0, &keyWord[0]);
		if(comparestring(keyWord, "kill") == 0)
			{commandID=1;/*lcd_putuint(commandID);*/shift=6;}
		else if(comparestring(keyWord, "suspend") == 0)
			{commandID=2;/*lcd_putuint(commandID);*/shift=9;}
		else if(comparestring(keyWord, "resume") == 0)
			{commandID=3;/*lcd_putuint(commandID);*/shift=8;}
		else
		{lcd_putstring(1,"invalid");commandID=4;}	
		getNextWord( A_receivedCommand,keyWord,0 );
		//lcd_putstring(0, keyWord);
		getNextWord(A_receivedCommand, keyWord,1);
		A_commandFlag=0;
		//lcd_putstring(1, &keyWord[0]);
		
		if(commandID>=1 && commandID<=3)
		{
	//	lcd_putstring(1, keyWord);
	//	lcd_putstring(0, A_taskNames[1]);
		for(task=0; task<=MAX_PROCESSES-1; task++)
			{
				//int x=comparestring(keyWord, A_taskNames[task]);
			//	lcd_putuint((unsigned int)x);
			//	lcd_putstring(task, A_taskNames[task]);
				if( (comparestring(keyWord, A_taskNames[task])) ==0 )
				{
					//lcd_putchar('1');
				//	lcd_putstring(task, A_taskNames[task]);
					taskCLIID=task;			//task number that cli needs
				//	lcd_putuint(taskCLIID);
					break;
				}
			}
			//lcd_putstring(1, keyWord);
			//lcd_putuint(taskCLIID);
			if (taskCLID!=ALL_PROCESSES && commandID==1)
				Killtask(taskCLIID);	// will kill the task with taskCLIID
			
			else if(taskCLID!=ALL_PROCESSES && commandID==2)
				Suspendtask(taskCLIID);	// will suspend the task with taskCLIID
			
			else if(taskCLID!=ALL_PROCESSES && commandID==3)
				Resumetask(taskCLIID);	// will resume the task with taskCLIID
			else
				{}				// invalid command
		}
		
	}
	
	
	int i=0, tasks_checked_for_scheduling=0;
	
	
	/*if(A_taskAlarm[A_currentTask]<A_SYSCLK)
	{
		A_taskSleeping[A_currentTask] = 0;	//Task Awake
	}
	*/
	if(A_currentTask == IDLE_TASK_ID)
	{
		A_currentTask = 0;
	}
	
	do
	{	
		A_currentTask++;
		//lcd_putchar('a');
		tasks_checked_for_scheduling++;
		if(A_currentTask >= IDLE_TASK_ID)
		{
			A_currentTask=0;
		}
		//lcd_putchar('b');
		if(A_SYSCLK < A_taskAlarm[A_currentTask])      //dont schedule
		{
			A_taskSleeping[A_currentTask] = 1;
		}
		else
		{
			A_taskSleeping[A_currentTask] = 0;
		}
		
		//tasks_checked_for_waiting++; 
	}
	while((A_taskSleeping[A_currentTask]==1 || A_taskEnable[A_currentTask]==0) && tasks_checked_for_scheduling<IDLE_TASK_ID);
	
	//now that control has come here, it means either an awake task is found or idle task has to be scheduled	
	if(A_taskSleeping[A_currentTask] == 1)
	{
		//schedule idle task
		A_currentTask = IDLE_TASK_ID;
	}
}
