//---------------------------------------------------------------------------
#include "rfunc.h"
using namespace RStd;


//---------------------------------------------------------------------------
// Global Variables
char RStd::CommentIndicator=';';                        // Can be ';' (Default) or '#'


//---------------------------------------------------------------------------
//
// function ReadLine
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
char *RStd::ReadLine(char *s, int n, FILE *f)
/* Reads a line of max n chars from stream f into string s, ignoring lines
   begining with CommentIndicator (comments) and stripping eventual terminating CR,LF or
   CRLF. */
{
  char *result;
  int i;

  #pragma warn -pia
  for(;result=fgets(s,n,f);)
  #pragma warn +pia
  {
	 if(s[0]==CommentIndicator) continue;
	 if((i=strlen(s)-1)>=0 && s[i]==10) s[i--]=0;                   /* LF. */
	 if(i>=0 && s[i]==13) s[i]=0;                                   /* CR. */
	 break;
  }
  return(result);
}


//---------------------------------------------------------------------------
void strupr(char *s)
{
	char *ptr=s;
	
	while(*ptr)
	{
		if(((*ptr)>='a')&&((*ptr)<='z'))
			(*ptr)-='a'-'A';
		ptr++;
	}
}


//---------------------------------------------------------------------------
void strlwr(char *s)
{
	char *ptr=s;
	
	while(*ptr)
	{
		if(((*ptr)>='A')&&((*ptr)<='Z'))
			(*ptr)+='a'-'A';
		ptr++;
	}
}