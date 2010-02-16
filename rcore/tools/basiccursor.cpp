/*

	R Project Library

	BasicCursor.hh

	Basic Container Cursor - Inline Implementation.

	Copyright 2005-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 by the Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <basiccursor.h>
#include <rstring.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class BasicCursor
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BasicCursor::BasicCursor(void)
{
	ActPtr=0;
	Current=Tab=0;
	ActPtr=FirstPtr=LastPtr=NbPtr=0;
}


//-----------------------------------------------------------------------------
BasicCursor::BasicCursor(const BasicCursor& src)
{
	ActPtr=src.ActPtr;
	Current=src.Current;
	Tab=src.Tab;
	FirstPtr=src.FirstPtr;
	LastPtr=src.LastPtr;
	NbPtr=src.NbPtr;
}


//-----------------------------------------------------------------------------
BasicCursor::BasicCursor(const BasicContainer& c,size_t min,size_t max)
{
	Set(c,min,max);
}


//-----------------------------------------------------------------------------
BasicCursor& BasicCursor::operator=(const BasicCursor& src)
{
	ActPtr=src.ActPtr;
	Current=src.Current;
	Tab=src.Tab;
	FirstPtr=src.FirstPtr;
	LastPtr=src.LastPtr;
	NbPtr=src.NbPtr;
	return(*this);
}


//-----------------------------------------------------------------------------
void BasicCursor::Set(const BasicContainer& c,size_t min,size_t max)
{
	RAssert(min<=max);
	NbPtr=c.NbPtr;
	Tab=c.Tab;
	if((max!=SIZE_MAX)&&(max<c.LastPtr))
		LastPtr=max+1;
	else
		LastPtr=c.LastPtr;
	if((min<=max)&&(min<c.LastPtr))
		FirstPtr=min;
	else
		FirstPtr=0;
	Current=0;
	ActPtr=0;
}



//-----------------------------------------------------------------------------
void BasicCursor::Clear(void)
{
	Current=Tab=0;
	ActPtr=FirstPtr=LastPtr=NbPtr=0;
}


//-----------------------------------------------------------------------------
void BasicCursor::Start(void)
{
	ActPtr=FirstPtr;
	if(!NbPtr)
	{
		Current=0;
		return;
	}
	Current=&Tab[FirstPtr];
	while((!(*Current))&&(ActPtr<LastPtr))
		Next();
}


//-----------------------------------------------------------------------------
void BasicCursor::GoTo(size_t idx)
{
	idx+=FirstPtr;
	if(idx>=LastPtr)
		throw std::range_error("void BasicCursor::GoTo(size_t) : column "+RString::Number(idx)+" outside range ["+RString::Number(FirstPtr)+","+RString::Number(LastPtr-1)+"]");
	Current=&Tab[idx];
	ActPtr=idx;
}


//-----------------------------------------------------------------------------
void BasicCursor::Next(size_t inc)
{
	if(!NbPtr) return;
	if(ActPtr==LastPtr)
		Start();
	else
	{
		ActPtr+=inc;
		Current+=inc;
		while((ActPtr<LastPtr)&&(!(*Current)))     // Go to next non-null pointer
		{
			ActPtr++;
			Current++;
		}
	}
}
