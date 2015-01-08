/*
	R Project Library

	RMaxVector.cpp

	Ascending Ordered Vector - Implementation.

	Copyright 2005-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by Valery Vandaele.
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



//---------------------------------------------------------------------------
// include files for R Project
#include <rmaxvector.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// RMaxVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RMaxVector::RMaxVector(size_t size,size_t id)
	: RContainer<RMaxValue,true,true>(size), Id(id)
{
}


//-----------------------------------------------------------------------------
RMaxVector::RMaxVector(const RMaxVector& src)
		: RContainer<RMaxValue,true,true>::RContainer(src), Id(src.Id)
{
}


//-----------------------------------------------------------------------------
RMaxVector& RMaxVector::operator=(const RMaxVector& vec)
{
	RContainer<RMaxValue,true,true>::operator=(vec);
	Id=vec.Id;
	return(*this);
}


//-----------------------------------------------------------------------------
int RMaxVector::Compare(const RMaxVector& vec) const
{
	return(CompareIds(Id,vec.Id));
}


//-----------------------------------------------------------------------------
int RMaxVector::Compare(size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
double RMaxVector::operator[](size_t i) const
{
	return(RContainer<RMaxValue,true,true>::operator[](i)->Value);
}


//------------------------------------------------------------------------------
size_t RMaxVector::Add(size_t id,double val,bool fixed)
{
	// Go through the vector to see where the identifier must be inserted (Where) and
	// if it is already in the vector (Act)
	size_t Act(cNoRef), Where(cNoRef);
	RMaxValue** Cur(Tab);
	for(size_t i=0;i<NbPtr;Cur++,i++)
	{
		if((Where==cNoRef)&&(val>(*Cur)->Value))
			Where=i;
		if((*Cur)->Id==id)
			Act=i;

		// If both positions are found -> stop the parsing of the vector
		if((Act!=cNoRef)&&(Where!=cNoRef))
			break;
	}

	if(Act==cNoRef)
	{
		// The identifier doesn't exist -> a new element must be inserted
		if(fixed)
		{
			// If the identifier must be inserted at the end -> nothing to do
			if(Where==cNoRef)
				return(cNoRef);

			// Remove the identifier at the end and put the new one at Where
			RMaxValue* ptr(Tab[NbPtr-1]);
			DeletePtrAt(NbPtr-1,true,false);
			ptr->Id=id;
			ptr->Value=val;
			InsertPtrAt(ptr,Where,false);
		}
		else
		{
			// Simply add it
			if(Where==cNoRef)
			{
				// Insert it at the end
				if(NbPtr)
					InsertPtrAt(new RMaxValue(id,val),NbPtr-1,false);
				else
					InsertPtrAt(new RMaxValue(id,val),0,false);
			}
			else
				InsertPtrAt(new RMaxValue(id,val),Where,false);
		}
	}
	else
	{
		// The identifier exist
		if(Where==cNoRef)
			Where=NbPtr-1; // The identifier must be put at the end of the vector

		// Change the value
		RMaxValue* ptr(Tab[Act]);
		ptr->Value=val;

		if(Act!=Where)
		{
			// Remove it from Act and put it at Where
			DeletePtrAt(Act,true,false);
			InsertPtrAt(ptr,Where,false);
		}
	}
	return(Where);
}


//-----------------------------------------------------------------------------
 RMaxVector::~RMaxVector(void)
{
}
