#define ST_CTRL_ENABLE (1<<0)
#define ST_CTRL_TICKINT (1<<1)
#define ST_CTRL_CLKSOURCE (1<<2)


#define A_TriggerPendSV() SCB->ICSR|=(1<<SCB_ICSR_PENDSVSET_Pos)	// triggers Pend SV


#define SYSTEM_PROCESSES 2
#define IDLE_TASK_ID ALL_PROCESSES-1
#define UART_TASK_ID ALL_PROCESSES-2



#define ALL_PROCESSES MAX_PROCESSES+SYSTEM_PROCESSES

//#################################################
//######## For Scheduler and Context Switcher #####
//#################################################
#define PROGRAM_COUNTER_OFFSET 14

volatile uint32_t A_taskFirstRun[ALL_PROCESSES];
volatile uint32_t A_taskStaskPointer[ALL_PROCESSES];
volatile uint32_t A_taskFunctionPointer[ALL_PROCESSES];
volatile uint32_t A_tasksCreatedSoFar=0;

volatile uint32_t* A_taskInitialTOS[ALL_PROCESSES];

volatile int16_t A_currentTask=ALL_PROCESSES;

volatile uint32_t* MSP_copy;

volatile uint32_t A_systickReloadValue[ALL_PROCESSES];

//#################################################




//#################################################
//######## For Mutex ##############################
//#################################################

typedef volatile uint8_t A_MUTEX;

//#################################################


//#################################################
//######## For Mailbox ############################
//#################################################

#ifndef MBOX_CONFIG_MANUAL
	#define MBOX_CONFIG_MANUAL 0
#endif

#if( MBOX_CONFIG_MANUAL!=0 && MBOX_CONFIG_MANUAL!=1 )
	#error "Illegal value for MBOX_CONFIG_MANUAL. Must be either 0 or 1."
#endif

#if(MBOX_CONFIG_MANUAL == 0)
typedef struct
{
	char Buffer[100];
	int TopOfBuffer;
	uint16_t owner;
	A_MUTEX MboxMut;
	char Desk[100];
}A_MBOX;
#endif

#if(MBOX_CONFIG_MANUAL == 1)
	#warning "Using manual configuration for Mailboxes."

typedef struct
{
	int TopOfBuffer;
	uint16_t owner;
	A_MUTEX MboxMut;
	A_DESKSPACE* deskptr;
	A_MAILSPACE* mailptr;
	int desksize, mailsize;
}A_MBOX;

typedef volatile char A_DESKSPACE;
typedef volatile char A_MAILSPACE;


#endif


//#################################################



//#################################################
//######## For System Clock #######################
//#################################################

volatile uint32_t A_SYSCLK;

volatile uint32_t A_taskSleeping[ALL_PROCESSES];
volatile uint32_t A_taskAlarm[ALL_PROCESSES];	

//#################################################


//#################################################
//######## For System ERROR CODE ##################
//#################################################
volatile uint32_t A_SYSTEM_ERROR_CODE;
//#################################################



//#################################################
//############## For CLI ##########################
//#################################################
#define BUFSIZE		0x40
volatile uint8_t A_commandFlag=0;
volatile char A_receivedCommand[BUFSIZE];
volatile char A_taskNames[ALL_PROCESSES][20];
volatile uint16_t taskCLID;
//#################################################

//#################################################
//########### For System Tasks ####################
//#################################################
uint32_t taskstackidle[100];
uint32_t A_taskUARTstack[100];


//#################################################
//########### For Tasks Enable ####################
//#################################################
uint8_t A_taskEnable[ALL_PROCESSES];


//#################################################
//########### For UART System Task ################
//#################################################
int A_uartSendIndex;

uint8_t A_customUartSend;
