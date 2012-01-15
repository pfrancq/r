/*
	R Project Library

	RMaxVector.cpp

	Ascending Ordered Vector - Implementation.

	Copyright 2005-2012 by Pascal Francq (pascal@francq.info).
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
void RMaxVector::Add(size_t id,double val)
{
	InsertPtr(new RMaxValue(id,val));
}


//-----------------------------------------------------------------------------
size_t RMaxVector::Replace(double& val,size_t& id)
{
	// Test where the value is to be placed in the vector
	bool Find;
	RMaxValue Search(id,val);
	size_t Pos(GetIndex(Search,Find));

	// If it must be placed at the end -> worser than all the other ones
	if(Pos==GetNb())
		return(cNoRef);

	// if Find and the same identifier -> nothing to do
	const RMaxValue* Old(GetValue(Pos));
	if(Find&&(Old->Id==id))
		return(cNoRef);

		// Verify one step after and one step beyond
/*		if((GetNb()>2)&&(Pos<GetNb()-2)&&(GetValue(Pos+1)->Id==id))
			return(cNoRef);

		// Verify one step beyond
		if((Pos>0)&&(GetValue(Pos-1)->Id==id))
			return(cNoRef);
	}*/

	// Replace the worser one by the new one
	RMaxValue* New(new RMaxValue(id,val));
	if(GetNb())
	{
		// There is at least one element
		id=Old->Id;
		val=Old->Value;
		DeletePtrAt(GetNb()-1);
	}
	InsertPtrAt(New,Pos,false);
	return(Pos);
}


//-----------------------------------------------------------------------------
 RMaxVector::~RMaxVector(void)
{
}
