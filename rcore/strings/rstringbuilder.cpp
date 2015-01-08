/*

	R Project Library

	RTextFile.h

	Text File - Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).

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
#include <rstringbuilder.h>
using namespace R;


//------------------------------------------------------------------------------
//
// class RStringBuilder
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RStringBuilder::RStringBuilder(void)
	: Len(0), CurChar(Buffer)
{
}


//------------------------------------------------------------------------------
RStringBuilder& RStringBuilder::operator+=(const RChar src)
{
	// Read a block of maximum 1025 characters in Buffer
	if(Len==1024)
		AppendBuffer();
	(*(CurChar++))=src;
	Len++;
	return(*this);
}


//------------------------------------------------------------------------------
RStringBuilder& RStringBuilder::operator+=(const RString& src)
{
	if(Len)
		AppendBuffer();
	String+=src;
	return(*this);
}


//------------------------------------------------------------------------------
RStringBuilder& RStringBuilder::operator+=(const RChar* src)
{
	if(Len)
		AppendBuffer();
	String+=src;
	return(*this);
}


//------------------------------------------------------------------------------
RStringBuilder& RStringBuilder::operator+=(const char* src)
{
	if(Len)
		AppendBuffer();
	String+=src;
	return(*this);
}


//------------------------------------------------------------------------------
const RString& RStringBuilder::operator()(void)
{
	if(Len)
		AppendBuffer();
	return(String);
}


//------------------------------------------------------------------------------
void RStringBuilder::Clear(void)
{
	Len=0;
	CurChar=Buffer;
	String.Clear();
}



//------------------------------------------------------------------------------
void RStringBuilder::AppendBuffer(void)
{
	Buffer[Len]=0;
	String+=Buffer;
	Len=0;
	CurChar=Buffer;
}
