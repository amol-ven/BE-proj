


//expected to be called before kernel start
void A_MBoxInit(A_MBOX* box, uint16_t _owner)
{
	box->TopOfBuffer=0;
	box->owner=_owner;
	
	A_mutexInit( &(box->MboxMut) );
}


int A_sendMail( char* string, A_MBOX* box  )
{	
	A_mutexAcquire( &(box->MboxMut) );
	int length=stringlength( string );
	
	
	if(99-box->TopOfBuffer < length+3)       //mail has messg + 2 bytes of senderID + 1 byte of length
	{
		return -1;
	}
	
	
	copystring( &(box->Buffer[box->TopOfBuffer]) , string);
	box->TopOfBuffer+=length;
	
	box->Buffer[box->TopOfBuffer] = A_currentTask>>8 ;    //write High Byte of SenderID
	(box->TopOfBuffer)++;
	box->Buffer[box->TopOfBuffer] = A_currentTask ;       //write LOW Byte of SenderID
	(box->TopOfBuffer)++;
	box->Buffer[box->TopOfBuffer] = length+3 ;              //write length byte
	(box->TopOfBuffer)++;
	
	
	A_mutexRelease( &(box->MboxMut) );
	
	return 0;
}



/************************************
 * checks if there is mail.
 * if not owner of mailbox it returns error
 * if no mail it returns error
 * if mail it returns char pointer of message in the deskspace
 * 		it puts message in desk array with first byte containing length of message...next two bytes contain sender task id. (length of message, HI of senderID, LO of senderID, message)
 ************************************/
 
char* A_checkMail( A_MBOX* box )
{
	A_mutexAcquire( &(box->MboxMut) );
	
	if(box->owner!=A_currentTask)
	{
		A_mutexRelease( &(box->MboxMut) );
		A_SYSTEM_ERROR_CODE = 3;
		return NULL;
	}
	else
	{
		if(box->TopOfBuffer == 0)
		{
			//mailbox empty
			A_mutexRelease( &(box->MboxMut) );
			A_SYSTEM_ERROR_CODE = 3;
			return NULL;
		}
		else
		{	
			int lengthofmail = box->Buffer[ (box->TopOfBuffer)-1 ] ;   //lengthof mail includes mssg + 2 bytes SenderID + 1 byte length of mail
			*(box->Desk) = lengthofmail-3;                             //write length of message first
			*( (box->Desk)+1 ) = box->Buffer[ (box->TopOfBuffer)-3 ];  //HI byte of sender ID
			*( (box->Desk)+2 ) = box->Buffer[ (box->TopOfBuffer)-2 ];  //LO byte of sender ID
			copyNbytes( (box->Desk)+3, &(box->Buffer[ (box->TopOfBuffer)-lengthofmail ]), lengthofmail-3); //Write message onto Desk
			
			box->TopOfBuffer -= lengthofmail; //decrement top of buffer so nw mail can arrive;
			A_mutexRelease( &(box->MboxMut) );
			//lcd_putstring(0, (box->Desk)+3);
			return (box->Desk)+3;
		}
	}
}

