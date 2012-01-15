/*
	R Project Library

	RSparseVector.cpp

	Sparse Vector - Implementation.

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
#include <rsparsevector.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// RSparseVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RSparseVector::RSparseVector(size_t size,size_t id)
	: RContainer<RValue,true,true>(size), Id(id)
{
}


//-----------------------------------------------------------------------------
RSparseVector::RSparseVector(const RSparseVector& src)
		: RContainer<RValue,true,true>::RContainer(src), Id(src.Id)
{}


//-----------------------------------------------------------------------------
RSparseVector& RSparseVector::operator=(const RSparseVector& vec)
{
	RContainer<RValue,true,true>::operator=(vec);
	Id=vec.Id;
	return(*this);
}


//-----------------------------------------------------------------------------
int RSparseVector::Compare(const RSparseVector& vec) const
{
	return(CompareIds(Id,vec.Id));
}


//-----------------------------------------------------------------------------
int RSparseVector::Compare(const size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
void RSparseVector::SetExistingElements(double val)
{
	RCursor<RValue> Values(*this);
	for(Values.Start();!Values.End();Values.Next())
		(*Values())=val;
}



//------------------------------------------------------------------------------
double RSparseVector::operator[](size_t i) const
{
	RValue* ptr(GetPtr(i));
	if(!ptr)
		return(0);
	return(ptr->Value);
}


//------------------------------------------------------------------------------
double& RSparseVector::operator[](size_t i)
{
	RValue* ptr(GetInsertPtr(i));
	return(ptr->Value);
}


//-----------------------------------------------------------------------------
 RSparseVector::~RSparseVector(void)
{
}
