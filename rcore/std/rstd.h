/*

  RFunc.h

  Some important functions

  By (c) P. Francq, 1999.

  Version 1.0.0.1

  Last Modify: 1999/07/13

    
*/

//---------------------------------------------------------------------------
#ifndef RFuncH
#define RFuncH


//---------------------------------------------------------------------------
#ifdef unix
	#ifndef NULL
		#define NULL 0
	#endif
#else
	#include <mem.h>
#endif
#include <new.h>
#include <string.h>
#include <stdio.h>


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// General Variables
extern char CommentIndicator;                        // Default ';'


//---------------------------------------------------------------------------
char *ReadLine(char *s, int n, FILE *f);
void strupr(char *s);
void strlwr(char *s);


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif