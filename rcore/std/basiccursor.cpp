/*

	R Project Library

	BasicCursor.hh

	Basic Container Cursor - Inline Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rstd/base/basiccursor.h>
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
	ActPtr=LastPtr=NbPtr=0;
}


//-----------------------------------------------------------------------------
BasicCursor::BasicCursor(const BasicCursor& src)
{
	ActPtr=src.ActPtr;
	Current=src.Current;
	Tab=src.Tab;
	ActPtr=src.ActPtr;
	LastPtr=src.LastPtr;
	NbPtr=src.NbPtr;
}


//-----------------------------------------------------------------------------
BasicCursor::BasicCursor(const BasicContainer& c,size_t max)
{
	Set(c,max);
}


//-----------------------------------------------------------------------------
BasicCursor& BasicCursor::operator=(const BasicCursor& src)
{
	ActPtr=src.ActPtr;
	Current=src.Current;
	Tab=src.Tab;
	ActPtr=src.ActPtr;
	LastPtr=src.LastPtr;
	NbPtr=src.NbPtr;
	return(*this);
}


//-----------------------------------------------------------------------------
void BasicCursor::Set(const BasicContainer& c,size_t max)
{
	NbPtr=c.NbPtr;
	Tab=c.Tab;
	if((max)&&(max<LastPtr))
		LastPtr=max;
	else
		LastPtr=c.LastPtr;
	Current=0;
	ActPtr=0;
}



//-----------------------------------------------------------------------------
void BasicCursor::Clear(void)
{
	ActPtr=0;
	Current=Tab=0;
	ActPtr=LastPtr=NbPtr=0;
}


//-----------------------------------------------------------------------------
void BasicCursor::Start(void)
{
	ActPtr=0;
	if(!NbPtr)
	{
		Current=0;
		return;
	}
	Current=Tab;
	ActPtr=0;
	while(!(*Current))
		Next();
}


//-----------------------------------------------------------------------------
void BasicCursor::GoTo(size_t idx)
{
	if(idx>=LastPtr)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RContainer::operator[] const : index outside container");
	#endif
	Current=&Tab[idx];
	ActPtr=idx;
}


//-----------------------------------------------------------------------------
void BasicCursor::Next(void)
{
	if(!NbPtr) return;
	if(ActPtr==LastPtr)
		Start();
	else
	{
		ActPtr++;
		Current++;
		while((ActPtr<LastPtr)&&(!(*Current)))
		{
			ActPtr++;
			Current++;
		}
	}
}
