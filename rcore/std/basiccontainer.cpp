/*

	R Project Library

	BasicContainer.cpp

	Basic Container - Implementation.

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
#include <rstd/base/basiccontainer.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class BasicContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BasicContainer::BasicContainer(size_t m,size_t i)
		: NbPtr(0), MaxPtr(m), LastPtr(0), IncPtr(i)
{
	if(MaxPtr)
	{
		Tab = new void*[MaxPtr];
		memset(Tab,0,MaxPtr*sizeof(void*));
		if(!IncPtr) IncPtr=MaxPtr/2;
		if(!IncPtr) IncPtr=10;
	}
	else
	{
		MaxPtr=10;
		Tab = new void*[MaxPtr];
		memset(Tab,0,MaxPtr*sizeof(void*));
		if(!IncPtr) IncPtr=10;
	}
}


//-----------------------------------------------------------------------------
void BasicContainer::VerifyTab(void)
{
	if(LastPtr==MaxPtr)
	{
		size_t OldSize;

		OldSize=MaxPtr;
		void** ptr;
		MaxPtr+=IncPtr;
		ptr=new void*[MaxPtr];
		memcpy(ptr,Tab,OldSize*sizeof(void*));
		delete[] Tab;
		Tab=ptr;
		memset(&Tab[OldSize],0,IncPtr*sizeof(void*));
	}
}


//-----------------------------------------------------------------------------
void BasicContainer::VerifyTab(size_t max)
{
	RReturnIfFail(max>0);
	if(max>MaxPtr)
	{
		void** ptr;
		size_t OldSize;

		OldSize=MaxPtr;
		MaxPtr=max;
		ptr=new void*[MaxPtr];
		memcpy(ptr,Tab,OldSize*sizeof(void*));
		delete[] Tab;
		Tab=ptr;
		memset(&Tab[OldSize],0,(MaxPtr-OldSize)*sizeof(void*));
	}
}


//-----------------------------------------------------------------------------
void BasicContainer::ReOrder(int sortOrder(const void*,const void*))
{
	if(NbPtr)
		qsort(static_cast<void*>(Tab),LastPtr,sizeof(void*),sortOrder);
}


//-----------------------------------------------------------------------------
void BasicContainer::InsertPtrOrderAt(const void* ins,size_t pos)
{
	void** ptr;

	RReturnIfFail(ins);
	RReturnIfFail(pos<=LastPtr);
	if(pos>MaxPtr)
	{
		VerifyTab(pos+IncPtr);
	}
	else
	{
		VerifyTab();
	}
	ptr=&Tab[pos];
	if(pos<LastPtr)
		memmove(ptr+1,ptr,(LastPtr-pos)*sizeof(void*));
	(*ptr)=(void*)ins;
	NbPtr++;
	LastPtr++;
}


//-----------------------------------------------------------------------------
void BasicContainer::Exchange(size_t pos1,size_t pos2)
{
	if((pos1>LastPtr)||(pos2>LastPtr))
		return;
	void* ptr=Tab[pos1];
	Tab[pos1]=Tab[pos2];
	Tab[pos2]=ptr;
}


//-----------------------------------------------------------------------------
const void* BasicContainer::operator[](size_t idx) const
{
	if(idx>=LastPtr)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("BasicContainer::operator[] const : index outside container");
	#endif
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
void* BasicContainer::operator[](size_t idx)
{
	if(idx>=LastPtr)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("BasicContainer::operator[] const : index outside container");
	#endif
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
size_t BasicContainer::GetTab(void** tab)
{
	memcpy(tab,Tab,LastPtr*sizeof(void*));
	return(LastPtr);
}


//-----------------------------------------------------------------------------
size_t BasicContainer::GetTab(const void** tab) const
{
	memcpy(tab,Tab,LastPtr*sizeof(void*));
	return(LastPtr);
}
