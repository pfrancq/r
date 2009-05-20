/*
	R Project Library

	RSparseMatrix.cpp

	Sparse Matrix - Implementation.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
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
#include <rsparsematrix.h>
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
//
// RSparseMatrix
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RSparseMatrix::RSparseMatrix(size_t nblines,size_t nbcols,bool alllines)
	: RContainer<RSparseVector,true,true>(nblines), AllLines(alllines), NbCols(nbcols)
{
	if(AllLines)
	{
		for(size_t i=0;i<nblines;i++)
			InsertPtrAt(new RSparseVector(NbCols,i),i,true);
	}
}


//-----------------------------------------------------------------------------
RSparseMatrix::RSparseMatrix(const RSparseMatrix& src)
	: RContainer<RSparseVector,true,true>::RContainer(src), AllLines(src.AllLines)
{
}


//-----------------------------------------------------------------------------
void RSparseMatrix::Clear(bool dellines)
{
	if(dellines)
		RContainer<RSparseVector,true,true>::Clear();
	else
	{
		RCursor<RSparseVector> Cur(*this);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Clear();
	}
}


//-----------------------------------------------------------------------------
RSparseMatrix& RSparseMatrix::operator=(const RSparseMatrix& src)
{
	AllLines=src.AllLines;
	RContainer<RSparseVector,true,true>::operator=(src);
	return(*this);
}


//------------------------------------------------------------------------------
double RSparseMatrix::operator()(size_t i,size_t j) const
{
	RSparseVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<RSparseVector*>(Tab[i]);
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
double& RSparseMatrix::operator()(size_t i,size_t j)
{
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


//------------------------------------------------------------------------------
const RSparseVector* RSparseMatrix::operator[](size_t i) const
{
	if(AllLines)
	{
		if(i<GetNb())
			return(RContainer<RSparseVector,true,true>::operator[](i));
		else
			return(0);
	}
	return(GetPtr(i));
}


//------------------------------------------------------------------------------
RSparseVector* RSparseMatrix::operator[](size_t i)
{
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
	return(Line);
}


//-----------------------------------------------------------------------------
RSparseMatrix::~RSparseMatrix(void)
{
}
