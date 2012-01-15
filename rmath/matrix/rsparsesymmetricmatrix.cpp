/*
	R Project Library

	RSparseSymmetricMatrix.cpp

	Sparse Matrix - Implementation.

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
#include <rsparsesymmetricmatrix.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// RSparseSymmetricMatrix
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RSparseSymmetricMatrix::RSparseSymmetricMatrix(size_t size,bool alllines,size_t init)
	: RSparseMatrix(size,size,alllines,init)
{
	if(AllLines)
	{
		for(size_t i=0;i<NbLines;i++)
			InsertPtrAt(new RSparseVector(NbCols,i),i,true);
	}
}


//-----------------------------------------------------------------------------
RSparseSymmetricMatrix::RSparseSymmetricMatrix(const RSparseSymmetricMatrix& src)
	: RSparseMatrix(src)
{
}


//-----------------------------------------------------------------------------
RSparseSymmetricMatrix& RSparseSymmetricMatrix::operator=(const RSparseSymmetricMatrix& matrix)
{
	RSparseMatrix::operator=(matrix);
	return(*this);
}


//------------------------------------------------------------------------------
double RSparseSymmetricMatrix::operator()(size_t i,size_t j) const
{
	if((i>NbLines)||(j>NbCols))
		throw std::range_error("RSparseSymmetricMatrix::operator() const : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

	if(j>i)
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}

	const RSparseVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<const RSparseVector*>(Tab[i]);
		else
			return(0);
	}
	else
		Line=GetPtr(i);
	if(!Line)
		return(0);
	RValue* Value(Line->GetPtr(j));
	if(!Value)
		return(0);
	return(Value->Value);
}


//------------------------------------------------------------------------------
double& RSparseSymmetricMatrix::operator()(size_t i,size_t j)
{
	if((i>NbLines)||(j>NbCols))
		throw std::range_error("RSparseSymmetricMatrix::operator() : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

	if(j>i)
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}

	RSparseVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<RSparseVector*>(Tab[i]);
		else
		{
			for(size_t add=GetNb();add<=i;add++)
				InsertPtrAt(Line=new RSparseVector(NbCols,add),add,true);
		}
	}
	else
	{
		Line=GetPtr(i);
		if(!Line)
			InsertPtr(Line=new RSparseVector(NbCols,i));
	}
	RValue* Value(Line->GetInsertPtr(j));
	return(Value->Value);
}


//-----------------------------------------------------------------------------
RSparseSymmetricMatrix::~RSparseSymmetricMatrix(void)
{
}
