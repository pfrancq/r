/*

  RString.cpp

  String - Implementation.

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
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#if unix
	#include <unistd.h>
#endif
#include <fcntl.h>
#include "rtextfile.h"
using namespace RStd;


//---------------------------------------------------------------------------
//
// RTextFile
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RTextFile::RTextFile(const RString &name,bool all) throw(bad_alloc): Name(name),All(all)
{
	struct stat statbuf;
	
	handle=open(Name(),O_RDONLY);
	Buffer=NULL;
	if(All)	
	{
		fstat(handle, &statbuf);
  	Buffer=new char[statbuf.st_size+1];
		read(handle,Buffer,statbuf.st_size);
		Buffer[statbuf.st_size]=0;
	}
	else
	{
  	Buffer=new char[1001];
		read(handle,Buffer,1000);
		Buffer[1000]=0;
	}
	ptr=Buffer;
	SkipSpaces();
}


//---------------------------------------------------------------------------
void RTextFile::SkipSpaces(void)
{
	while((*ptr)&&((*ptr)==' '||(*ptr)=='\t'||(*ptr)=='\n'||(*ptr)=='\r')) ptr++;
	if((*ptr)=='%')
		while((*ptr)&&(*ptr)!='\n'&&(*ptr)!='\r') ptr++;
	while((*ptr)&&((*ptr)==' '||(*ptr)=='\t'||(*ptr)=='\n'||(*ptr)=='\r')) ptr++;		
}


//---------------------------------------------------------------------------
long int RTextFile::GetInt(void) throw(RString)
{
	char *ptr2=ptr;
	
	while((*ptr)&&(*ptr)!=' '&&(*ptr)!='\t'&&(*ptr)!='\n'&&(*ptr)!='\r')
		if(!isdigit(*(ptr++))) throw("No Int");
	(*(ptr++))=0;
	SkipSpaces();
	return(atol(ptr2));
}


//---------------------------------------------------------------------------
float RTextFile::GetFloat(void) throw(RString)
{
	char *ptr2=ptr;

	while((*ptr)&&(*ptr)!=' '&&(*ptr)!='\t'&&(*ptr)!='\n'&&(*ptr)!='\r')
	{
		if((!isdigit(*ptr))&&(*ptr)!='.'&&(*ptr)!='e'&&(*ptr)!='E') throw("No float");
		ptr++;
	}
	(*(ptr++))=0;
	SkipSpaces();
	return(atof(ptr2));
}


//---------------------------------------------------------------------------
char* RTextFile::GetWord(void)
{
	char *ptr2=ptr;

	while((*ptr)&&(*ptr)!=' '&&(*ptr)!='\t'&&(*ptr)!='\n'&&(*ptr)!='\r') ptr++;
	(*(ptr++))=0;
	SkipSpaces();
	return(ptr2);
}


//---------------------------------------------------------------------------
char* RTextFile::GetLine(void)
{
	char *ptr2=ptr;

	while((*ptr)&&(*ptr)!='\n'&&(*ptr)!='\r') ptr++;
	(*(ptr++))=0;
	SkipSpaces();
	return(ptr2);
}


//---------------------------------------------------------------------------
RTextFile::~RTextFile(void)
{
	if(Buffer) delete[] Buffer;
	close(handle);
}
