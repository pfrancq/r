/*

	R Project Library

	RStd.cpp

	R Standard Library - Implementation.

	Copyright 1999-2008 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RException::RException(void) throw()
{
	Msg[0]='\0';
}


//------------------------------------------------------------------------------
RException::RException(const char* str) throw()
{
	SetMsg(str);
	cerr<<Msg<<endl;
}


//------------------------------------------------------------------------------
RException::RException(const char* func,long where,const char* str) throw()
{
	SetMsg(func,where,str);
	cerr<<Msg<<endl;
}


//------------------------------------------------------------------------------
void RException::SetMsg(const char* str)
{
	size_t len(0);
	char* msg(Msg);
	if(str)
	{
		for(const char* ptr(str);(len<1024)&&(*ptr);ptr++,len++,msg++)
			(*msg)=(*ptr);
	}
	(*msg)='\0';
}


//------------------------------------------------------------------------------
void RException::SetMsg(const char* func,long where,const char* str)
{
	size_t len(0);
	const char* ptr;
	char* msg(Msg);
	if(func)
	{
		for(ptr=func;(len<1024)&&(*ptr);ptr++,len++,msg++)
			(*msg)=(*ptr);
		char add[30];
		sprintf(add," [%li]: ",where);
		for(ptr=add;(len<1024)&&(*ptr);ptr++,len++,msg++)
			(*msg)=(*ptr);
	}
	if(str)
	{
		for(ptr=str;(len<1024)&&(*ptr);ptr++,len++,msg++)
			(*msg)=(*ptr);
	}
	(*msg)='\0';
}


//------------------------------------------------------------------------------
RException::~RException(void) throw()
{
}
