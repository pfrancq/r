/*

  RFunc.cpp

  Some important functions - Implementation.

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


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

	#ifdef __BORLANDC__
	  #pragma warn -pia
	#endif
  for(;(result=fgets(s,n,f));)
	#ifdef __BORLANDC__
	  #pragma warn +pia
	#endif
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