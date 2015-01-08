/*

	R Project Library

	RStd.cpp

	R Standard Library - Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
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
#include <rexception.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void RException::Null(void)
{
}


//------------------------------------------------------------------------------
RException::RException(void)
	: Msg(RString::Null)
{
}


//------------------------------------------------------------------------------
RException::RException(const RString& str)
{
	SetMsg(str);
}


//------------------------------------------------------------------------------
RException::RException(const RString& func,long where,const RString& str)
{
	SetMsg(func,where,str);
}


//------------------------------------------------------------------------------
void RException::SetMsg(const RString& str)
{
	Msg=str;
}


//------------------------------------------------------------------------------
void RException::SetMsg(const RString& func,long where,const RString& str)
{
	Msg=func+" ["+RString::Number(where)+"]: "+str;
}


//------------------------------------------------------------------------------
RException::~RException(void)
{
}
