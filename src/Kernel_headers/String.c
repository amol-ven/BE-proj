void copystring(char* dest, char* src)  //copies string including the NULL character
{
	while(*src)
	{
		*(dest++) = *(src++);
		
	}
	*dest='\0';
}

int comparestring(volatile char* dest,volatile char* src)  //compare string including the NULL character
{		
	while( *(src++) == *(dest++) )
	{
		if(*(src)=='\0')
		{break;}
	}
	return (*src - *dest);
}


void copyNbytes(char* dest, char* src, int N)
{
	do
	{
		*(dest++) = *(src++);
		N--;
	}
	while(N);
}
/*

void copyNbytes(char* dest, char* src, int N)
{
	while( (N--)>0 )
	{*(dest++) = *(src++);}
}

*/ 
int stringlength( char* string ) //including null character
{
		int i=0;
		while(*string)
		{
			i++;
			string++;
		}
		return ++i;
}


/*********** Get Word from a Char Array*******/
int getNextWord(volatile char *str,volatile char *buffer, int refresh)
{
	static int index_for_str=0;
	if(refresh!=0)
	{
		index_for_str=0;
		return 0;
	}
	
	while(str[index_for_str]==' ' && str[index_for_str]!='\0')
	{
		index_for_str++;
	}
	
	while(str[index_for_str]!=' ' && str[index_for_str]!='\0')
	{
		*(buffer++) = (str[index_for_str++]);
	}
	
	
	*buffer = '\0';
	if(str[index_for_str]=='\0')
	{
	//	*buffer = '\0';
		return -1;
	}
	else
	{
	//	*buffer = '\0';
		return 0;
	}
}
