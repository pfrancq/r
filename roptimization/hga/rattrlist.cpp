/*

	R Project Library

	RAttrList.cpp

	Class representing a list of attributes - Implementation

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>
#include <string.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rattrlist.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RAttrList
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RAttrList::RAttrList(const unsigned int MaxSize)
	: RVectorInt<unsigned int,true>(MaxSize)
{
}


//------------------------------------------------------------------------------
RAttrList::RAttrList(const RAttrList& lst)
	: RVectorInt<unsigned int,true>(lst)
{
}


//------------------------------------------------------------------------------
bool RAttrList::IsParent(const RAttrList& Child) const
{
	unsigned int i=Child.GetNb(),j=GetNb();
	const unsigned int *ptr1=GetList(),*ptr2=Child.GetList();

	// Current list cannot be larger than the child list
	if(GetNb()>i)
		return(false);

	// Go trough all the current attributes
	while(j)
	{
		// Skip all the "smaller" attributes of the child list
		while(i&&((*ptr2)<(*ptr1)))
		{
			i--;
			ptr2++;
		}

		// If no more attributes in the child list, it cannot be a child
		if(!i)
			return(false);

		// If the current attributes of both list is not identical, it cannot be a child
		if((*ptr2)!=(*ptr1))
			return(false);

		// OK for current attribute, analyze the next one
		j--;
		ptr1++;
	}
	return(true);
}


//------------------------------------------------------------------------------
bool RAttrList::IsInter(const RAttrList& PosAttr) const
{
	const unsigned int *FstA=GetList(),*SecA=PosAttr.GetList();
	unsigned int i=GetNb(),j=PosAttr.GetNb();

	while(i)
	{
		while(j&&((*SecA)<(*FstA)))
		{
			j--;
			SecA++;
		}
		if(j&&((*FstA)==(*SecA)))
			return(true);
		i--;
		FstA++;
	}
	return(false);
}


//------------------------------------------------------------------------------
unsigned int RAttrList::GetNbCommon(const RAttrList& PosAttr) const
{
	unsigned int count=0;
	const unsigned int *ptr1=GetList(),*ptr2=PosAttr.GetList();
	unsigned int i=GetNb(),j=PosAttr.GetNb();

	while(i)
	{
		while(j&&((*ptr2)<(*ptr1)))
		{
			j--;
			ptr2++;
		}
		if(j&&((*ptr1)==(*ptr2)))
			return(true);
		i--;
		ptr1++;
	}
	return(count);
}


//------------------------------------------------------------------------------
void RAttrList::Diff(const RAttrList& Top,const RAttrList& Child)
{
	const unsigned int *TopA=Top.GetList(),*ChildA=Child.GetList();
	unsigned int i=Top.GetNb(),j=Child.GetNb();

	Clear();
	while(j)
	{
		while(i&&((*TopA)<(*ChildA)))
		{
			i--;
			TopA++;
		}
		if((!i)||((*TopA)!=(*ChildA)))
			Insert(*ChildA);
		j--;
		ChildA++;
	}
}


//------------------------------------------------------------------------------
void RAttrList::Inter(const RAttrList& Fst,const RAttrList& Sec)
{
	const unsigned int *FstA=Fst.GetList(),*SecA=Sec.GetList();
	unsigned int i=Fst.GetNb(),j=Sec.GetNb();

	Clear();
	while(i)
	{
		while(j&&((*SecA)<(*FstA)))
		{
			j--;
			SecA++;
		}
		if(j&&((*FstA)==(*SecA)))
			Insert(*FstA);
		i--;
		FstA++;
	}
}


//------------------------------------------------------------------------------
unsigned int RAttrList::FindFirstDiff(const RAttrList& PosAttr) const
{
	const unsigned int *ptr1,*ptr2;
	unsigned int i;

	for(i=GetNb(),ptr1=GetList(),ptr2=PosAttr.GetList();i--;ptr1++,ptr2++)
		if(*ptr1!=*ptr2)
			return(*ptr1);
	return(NoAttr);
}


//------------------------------------------------------------------------------
RAttrList& RAttrList::operator=(const RAttrList& src)
{
	RVectorInt<unsigned int,true>::operator=(src);
	return(*this);
}


//------------------------------------------------------------------------------
RAttrList::~RAttrList(void)
{
}
