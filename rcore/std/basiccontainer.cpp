/*

	R Project Library

	BasicContainer.cpp

	Basic Container - Implementation.

	Copyright 2005-2008 by the Université Libre de Bruxelles.

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
#include <basiccontainer.h>
#include <rstring.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class BasicContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
BasicContainer::BasicContainer(void)
	: Tab(0), NbPtr(0), MaxPtr(0), LastPtr(0), IncPtr(0)
{
}


//-----------------------------------------------------------------------------
BasicContainer::BasicContainer(size_t m,size_t i)
	: Tab(0), NbPtr(0), MaxPtr(m), LastPtr(0), IncPtr(i)
{
	if(MaxPtr)
	{
		if(!IncPtr) IncPtr=MaxPtr/2;
		if(!IncPtr) IncPtr=10;
		Tab = new void*[MaxPtr];
		memset(Tab,0x0,MaxPtr*sizeof(void*));
	}
	else
	{
		MaxPtr=10;
		Tab = new void*[MaxPtr];
		memset(Tab,0x0,MaxPtr*sizeof(void*));
		if(!IncPtr) IncPtr=10;
	}
}


//-----------------------------------------------------------------------------
void BasicContainer::VerifyTab(size_t max)
{
	if((!max)&&(LastPtr==MaxPtr))
		max=MaxPtr+IncPtr;
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
		memset(&Tab[OldSize],0x0,(MaxPtr-OldSize)*sizeof(void*));
	}
}


//-----------------------------------------------------------------------------
void BasicContainer::Clear(bool bAlloc,size_t m,size_t i)
{
	if(bAlloc)
	{
		void **ptr;

		for(LastPtr++,ptr=Tab;--LastPtr;ptr++)
		{
			if(*ptr)
				Delete(*ptr);
		}
		memset(Tab,0x0,MaxPtr*sizeof(void*));
	}
	LastPtr=NbPtr=0;
	if(i)
		IncPtr=i;
	if(m)
		VerifyTab(m);
}


//-----------------------------------------------------------------------------
void BasicContainer::ReOrder(int sortOrder(const void*,const void*))
{
	if(NbPtr)
		qsort(static_cast<void*>(Tab),LastPtr,sizeof(void*),sortOrder);
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
size_t BasicContainer::GetIndex(bool bOrder,const void* tag,bool& find,size_t min, size_t max,int compare(const void*,const void*)) const
{
	size_t NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	void *ptr,**ptr2;

	if(!NbPtr)
	{
		find=false;
		return(0);
	}
	if(min<LastPtr-1)
		NbMin=min;
	else
		NbMin=0;
	if((!max)||(max>=LastPtr)||(max<min))
		NbMax=LastPtr-1;
	else
		NbMax=max;
	if(bOrder)
	{
		find=false;
		if(NbMax)
		{
			while(Cont)
			{
				i=(NbMax+NbMin)/2;
				ptr=Tab[i];
				Comp=compare(ptr,tag);
				if(!Comp)
				{
					find=true;
					return(i);
				}
				if(Comp>0)
				{
					NbMax = i;
					if(i) NbMax--;
				}
				else
					NbMin = i+1;
				Cont = NotLast;
				if(NbMin>=NbMax) NotLast=false;
			}
		}
		else
		{
			i = 0;
			Comp=compare(*Tab,tag);
			if(!Comp)
			{
				find=true;
				return(0);
			}
		}
		if(Comp<0) i++;
		return(i);
	}
	else
	{
		find=true;
		for(i=NbMin,ptr2=&Tab[NbMin];i<=NbMax;ptr2++,i++)
			if((*ptr2)&&(!compare(*ptr2,tag))) return(i);
		find=false;
		return(i);
	}
}


//-----------------------------------------------------------------------------
bool BasicContainer::IsIn(bool bOrder,const void* tag,bool sortkey,size_t min, size_t max,int compare(const void*,const void*)) const
{
	bool Find;
	GetIndex(bOrder&&sortkey,tag,Find,min,max,compare);
	return(Find);
}


//-----------------------------------------------------------------------------
const void* BasicContainer::operator[](size_t idx) const
{
	if(idx>=LastPtr)
	{
		if(LastPtr)
			throw std::range_error("BasicContainer::operator[] const : column "+RString::Number(idx)+" outside range [0,"+RString::Number(LastPtr-1)+"]");
		else
			throw std::range_error("BasicContainer::operator[] const : no elements");
	}
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
void* BasicContainer::operator[](size_t idx)
{
	if(idx>=LastPtr)
	{
		if(LastPtr)
			throw std::range_error("BasicContainer::operator[] : column "+RString::Number(idx)+" outside range [0,"+RString::Number(LastPtr-1)+"]");
		else
			throw std::range_error("BasicContainer::operator[] : no elements");
	}
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
void* BasicContainer::GetPtr(bool bOrder,const void* tag,bool sortkey,size_t min, size_t max,int compare(const void*,const void*)) const
{
	bool Find;
	size_t Index=GetIndex(bOrder&&sortkey,tag,Find,min,max,compare);
	if(Find)
		return(Tab[Index]);
	else
		return(0);
}


//-----------------------------------------------------------------------------
size_t BasicContainer::GetTab(const void** tab,size_t min, size_t max) const
{
	size_t NbMin,NbMax;
	if(min<LastPtr-1)
		NbMin=min;
	else
		NbMin=0;
	if((!max)||(max>=LastPtr)||(max<min))
		NbMax=LastPtr-1;
	else
		NbMax=max;
	memcpy(tab,&Tab[NbMin],(NbMax-NbMin+1)*sizeof(void*));
	return(NbMax-NbMin+1);
}


//-----------------------------------------------------------------------------
size_t BasicContainer::GetTab(void** tab,size_t min, size_t max)
{
	size_t NbMin,NbMax;
	if(min<LastPtr-1)
		NbMin=min;
	else
		NbMin=0;
	if((!max)||(max>=LastPtr)||(max<min))
		NbMax=LastPtr-1;
	else
		NbMax=max;
	memcpy(tab,&Tab[NbMin],(NbMax-NbMin+1)*sizeof(void*));
	return(NbMax-NbMin+1);
}


//-----------------------------------------------------------------------------
void BasicContainer::InsertPtr(bool bAlloc,bool bOrder,const void* ins,bool del,size_t min, size_t max,int compare(const void*,const void*))
{
	RReturnIfFail(ins);
	if(bOrder)
	{
		bool Find;
		size_t Index=GetIndex(bOrder,ins,Find,min,max,compare);
		InsertPtrAt(bAlloc,ins,Index,Find&&del);
	}
	else
	{
		VerifyTab(0);
		Tab[LastPtr++]=(void*)ins;
		NbPtr++;
	}
}


//-----------------------------------------------------------------------------
void BasicContainer::InsertPtrAt(bool bAlloc,const void* ins,size_t pos,bool del)
{
	void** ptr;

	RReturnIfFail(ins);
	if(pos+1>MaxPtr)
	{
		VerifyTab(pos+1+IncPtr);
	}
	else
	{
		VerifyTab(0);
	}
	ptr=&Tab[pos];
	if(pos<LastPtr)
	{
		if(del)
		{
			if(bAlloc&&(*ptr))
				Delete(*ptr);
		}
		else
		{
			memmove(ptr+1,ptr,(LastPtr-pos)*sizeof(void*));
			NbPtr++;
		}
	}
	else
		NbPtr++;
	(*ptr)=(void*)ins;
	if(pos+1>LastPtr)
	{
		LastPtr=pos+1;
	}
	else
	{
		if(!del)
			LastPtr++;
	}
}


//-----------------------------------------------------------------------------
void BasicContainer::DeletePtr(bool bOrder,bool bAlloc,const void* tag,bool sortkey,size_t min, size_t max,int compare(const void*,const void*))
{
	RReturnIfFail(tag);
	if(!tag) return;

	bool Find;
	size_t Index=GetIndex(bOrder&&sortkey,tag,Find,min,max,compare);
	if(!Find)
		return;
	DeletePtrAt(bAlloc,Index,true);
}


//-----------------------------------------------------------------------------
void BasicContainer::DeletePtrAt(bool bAlloc,size_t pos,bool del)
{
	void** ptr;

	RReturnIfFail(pos<LastPtr);
	ptr=&Tab[pos];
	if(bAlloc)
		Delete(*ptr);

	// If the position is the last one -> find the last position
	if(pos==LastPtr-1)
	{
		(*ptr)=0;
		for(;(!(*ptr))&&LastPtr;ptr--,LastPtr--);
	}
	else
	{
		// Not the last element -> Verify if the elements must be moved
		if(del)
		{
			memmove(ptr,ptr+1,((--LastPtr)-pos)*sizeof(void*));
			Tab[LastPtr]=0;
		}
		else
			(*ptr)=0;
	}

	// Decrease number of elements
	NbPtr--;
}


//-----------------------------------------------------------------------------
BasicContainer::~BasicContainer(void)
{
	delete[] Tab;
}
