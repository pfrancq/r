/*

	R Project Library

	RChar.cpp

	Unicode character - Implementation.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++


//------------------------------------------------------------------------------
// include files for R Project
#include <rchar.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RChar
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
size_t RChar::StrLen(const RChar* str)
{
	size_t len;
	const RChar* ptr;

	if(!str) return(0);
	for(len=0,ptr=str;!ptr->IsNull();len++,ptr++) ;
	return(len);
}


//------------------------------------------------------------------------------
int RChar::StrCmp(const RChar* str1,const RChar* str2)
{
	UChar u1,u2;

	if(!str1)
	{
		if(!str2) return(0);
		return(-1);
	}
	else if(!str2)
		return(1);
	while((!str1->IsNull())&&(!str2->IsNull()))
	{
		u1=(*str1).Unicode();
		u2=(*str2).Unicode();
		if(u1!=u2)
			return(u1-u2);
		str1++;
		str2++;
	}
	return((*str1).Unicode()-(*str2).Unicode());
}


//------------------------------------------------------------------------------
int RChar::StrCmp(const RChar* str1,const char* str2)
{
	UChar u1,u2;

	if(!str1)
	{
		if(!str2) return(0);
		return(-1);
	}
	else if(!str2)
		return(1);
	while((!str1->IsNull())&&(*str2))
	{
		u1=(*str1).Unicode();
		u2=RChar(*str2).Unicode();
		if(u1!=u2)
			return(u1-u2);
		str1++;
		str2++;
	}
	return((*str1).Unicode()-RChar(*str2).Unicode());
}


//------------------------------------------------------------------------------
int RChar::StrCmp(const char* str1,const RChar* str2)
{
	UChar u1,u2;

	if(!str1)
	{
		if(!str2) return(0);
		return(-1);
	}
	else if(!str2)
		return(1);
	while((*str1)&&(!str2->IsNull()))
	{
		u1=RChar(*str1).Unicode();
		u2=(*str2).Unicode();
		if(u1!=u2)
			return(u1-u2);
		str1++;
		str2++;
	}
	return(RChar(*str1).Unicode()-(*str2).Unicode());
}


//------------------------------------------------------------------------------
int RChar::StrNCmp(const RChar* str1,const RChar* str2,size_t size)
{
	UChar u1,u2;

	if(!str1)
	{
		if(!str2) return(0);
		return(-1);
	}
	else if(!str2)
		return(1);
	size_t nb(0);
	while((nb<size)&&(!str1->IsNull())&&(!str2->IsNull()))
	{
		u1=(*str1).Unicode();
		u2=(*str2).Unicode();
		if(u1!=u2)
			return(u1-u2);
		str1++;
		str2++;
		nb++;
	}
	if(nb==size)
		return(0);
	return((*str1).Unicode()-(*str2).Unicode());
}


//------------------------------------------------------------------------------
int RChar::StrNCmp(const RChar* str1,const char* str2,size_t size)
{
	UChar u1,u2;

	if(!str1)
	{
		if(!str2) return(0);
		return(-1);
	}
	else if(!str2)
		return(1);
	size_t nb(0);
	while((nb<size)&&(!str1->IsNull())&&(*str2))
	{
		u1=(*str1).Unicode();
		u2=RChar(*str2).Unicode();
		if(u1!=u2)
			return(u1-u2);
		str1++;
		str2++;
		nb++;
	}
	if(nb==size)
		return(0);
	return((*str1).Unicode()-RChar(*str2).Unicode());
}


//------------------------------------------------------------------------------
int RChar::StrNCmp(const char* str1,const RChar* str2,size_t size)
{
	UChar u1,u2;

	if(!str1)
	{
		if(!str2) return(0);
		return(-1);
	}
	else if(!str2)
		return(1);
	size_t nb(0);
	while((nb<size)&&(*str1)&&(!str2->IsNull()))
	{
		u1=RChar(*str1).Unicode();
		u2=(*str2).Unicode();
		if(u1!=u2)
			return(u1-u2);
		str1++;
		str2++;
		nb++;
	}
	if(nb==size)
		return(0);
	return(RChar(*str1).Unicode()-(*str2).Unicode());
}


//------------------------------------------------------------------------------
const RChar* RChar::StrChr(const RChar* str1,const RChar c)
{
	UChar u1,u2;

	if(!str1)
		return(0);
	u2=c.Unicode();
	while(!str1->IsNull())
	{
		u1=(*str1).Unicode();
		if(u1==u2) return(str1);
		str1++;
	}
	return(0);
}
