/*

	R Project Library

	RBoolCursor.cpp

	Cursor over a container of numbers - Inline implementation

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
#include <rboolcursor.h>
#include <rstring.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RBoolCursor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RBoolCursor::RBoolCursor(void)
	: NbBool(0), List(0), Parse(0), Pos(0), First(0), Last(0)
{
}


//------------------------------------------------------------------------------
RBoolCursor::RBoolCursor(const RBoolCursor& cur)
	: NbBool(cur.NbBool), List(cur.List), Parse(cur.Parse), Pos(cur.Pos), First(cur.First), Last(cur.Last)
{
}


//------------------------------------------------------------------------------
RBoolCursor::RBoolCursor(const RBoolVector& vector,size_t min,size_t max)
	: NbBool(0), List(0), Parse(0), Pos(0), First(0), Last(0)
{
	Set(vector,min,max);
}


//------------------------------------------------------------------------------
void RBoolCursor::Set(const RBoolVector& vector,size_t min,size_t max)
{
	NbBool=vector.NbBool;
	List=vector.List;
	Parse=vector.List;
	Pos=0;
	if((max!=SIZE_MAX)&&(max<vector.GetNb()))
		Last=max+1;
	else
		Last=vector.GetNb();
	if((min<=max)&&(min<vector.GetNb()))
		First=min;
	else
		First=0;
}


//-----------------------------------------------------------------------------
void RBoolCursor::Start(void)
{
	Pos=First;
	if(!NbBool)
	{
		Parse=0;
		return;
	}
	Parse=&List[First];
}


//-----------------------------------------------------------------------------
void RBoolCursor::StartFromEnd(void)
{
	Pos=Last-1;
	if(!NbBool)
	{
		Parse=0;
		return;
	}
	Parse=&List[Last-1];
}


//-----------------------------------------------------------------------------
void RBoolCursor::GoTo(size_t idx)
{
	idx+=First;
	if(idx>=Last)
		throw std::range_error("void RBoolCursor::GoTo(size_t) : column "+RString::Number(idx)+" outside range ["+RString::Number(First)+","+RString::Number(Last-1)+"]");
	Parse=&List[idx];
	Pos=idx;
}


//-----------------------------------------------------------------------------
void RBoolCursor::Next(size_t inc)
{
	if(!NbBool) return;
	if(Pos==Last)
		Start();
	else
	{
		Pos+=inc;
		Parse+=inc;
	}
}


//-----------------------------------------------------------------------------
void RBoolCursor::Prev(size_t inc)
{
	if(!NbBool) return;
	if(Pos==cNoRef)
	{
		StartFromEnd();
		if(inc>1)
			Prev(inc);
	}
	else
	{
		if(inc>Pos)
			Pos=cNoRef;
		else
		{
			Pos-=inc;
			Parse-=inc;
		}
	}
}
