
void A_mutexInit(A_MUTEX* mut)
{
	*mut=1;
}


void A_mutexAcquire(A_MUTEX* mut)
{
	volatile uint8_t mutcopy=*mut;
	while(1)
	{
		
		while(mutcopy==0)
		{
			A_taskQuit();
		}
	
		*mut--;
		if(*mut==0xFF)
		{
			*mut++;
			A_taskQuit();
			continue;
		}
		break;
	}
}

void A_mutexRelease(A_MUTEX* mut)
{
	*mut=1;
	
}


