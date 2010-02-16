/*
	R Project Library

	RSparseMatrix.cpp

	Sparse Matrix - Implementation.

	Copyright 2005-2010 by Pascal Francq (pascal@francq.info).
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
RSparseMatrix::RSparseMatrix(size_t nblines,size_t nbcols,bool alllines,size_t init)
	: RGenericMatrix(nblines,nbcols), RContainer<RSparseVector,true,true>(nblines),
	  AllLines(alllines), InitNbCols(init)
{
	if(AllLines)
	{
		for(size_t i=0;i<NbLines;i++)
			InsertPtrAt(new RSparseVector(InitNbCols,i),i,true);
	}
}


//-----------------------------------------------------------------------------
RSparseMatrix::RSparseMatrix(const RSparseMatrix& src)
	: RGenericMatrix(src), RContainer<RSparseVector,true,true>::RContainer(src), AllLines(src.AllLines)
{
}


//-----------------------------------------------------------------------------
void RSparseMatrix::Clear(double,bool clean)
{
	if(clean)
		RContainer<RSparseVector,true,true>::Clear();
	else
	{
		RCursor<RSparseVector> Cur(*this);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Clear();
	}
}


//------------------------------------------------------------------------------
void RSparseMatrix::Init(double val)
{
	std::cerr<<"All elements of a RSparseMatrix are allocated. Use RMatrix instead"<<std::endl;
	RGenericMatrix::Init(val);
}


//------------------------------------------------------------------------------
void RSparseMatrix::VerifySize(size_t newlines,size_t newcols,bool,double)
{
	// Verify the lines
	if(newlines>NbLines)
	{
		// New lines must be added.
		VerifyTab(newlines);
		if(AllLines)
		{
			for(size_t i=NbLines;i<newlines;i++)
				InsertPtrAt(new RSparseVector(InitNbCols,i),i,true);
		}
	}
	else if(newlines<NbLines)
	{
		// A number of lines must be deleted
		for(size_t i=NbLines+1;--i;)
		{
			// If the line correspond to a identifier smaller than the number of lines
			// -> All necessary lines are removed.
			if(RContainer<RSparseVector,true,true>::operator[](GetNb()-1)->Id<newlines)
				break;

			DeletePtrAt(GetNb()-1);
		}
	}

	// Verify the columns
	if(newcols<NbCols)
	{
		// In each vector, the non-null elements between newcols and NbCols must be removed.
		RCursor<RSparseVector> Cur(*this,0,NbLines-1);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Treat the elements in reverse order since the the columns to remove
			// are stored at the end of the vector
			for(size_t i=NbCols+1;--i;)
			{
				// If the last element is inside the new number of columns -> vector is treated.
				if(Cur()->RContainer<RValue,true,true>::operator[](GetNb()-1)->Id<newcols)
					break;

				Cur()->DeletePtrAt(GetNb()-1,false);
			}
		}
	}

	NbLines=newlines;
	NbCols=newcols;
}


//-----------------------------------------------------------------------------
RSparseMatrix& RSparseMatrix::operator=(const RSparseMatrix& matrix)
{
	RGenericMatrix::operator=(matrix);
	RContainer<RSparseVector,true,true>::operator=(matrix);
	AllLines=matrix.AllLines;
	return(*this);
}


//------------------------------------------------------------------------------
double RSparseMatrix::operator()(size_t i,size_t j) const
{
	if((i>NbLines)||(j>NbCols))
		throw std::range_error("RSparseMatrix::operator() const : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

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
	if((i>NbLines)||(j>NbCols))
		throw std::range_error("RSparseMatrix::operator() : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

	RSparseVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<RSparseVector*>(Tab[i]);
		else
		{
			for(size_t add=GetNb();add<=i;add++)
				InsertPtrAt(Line=new RSparseVector(InitNbCols,add),add,true);
		}
	}
	else
	{
		Line=GetPtr(i);
		if(!Line)
			InsertPtr(Line=new RSparseVector(InitNbCols,i));
	}
	RValue* Value(Line->GetInsertPtr(j));
	return(Value->Value);
}


//------------------------------------------------------------------------------
const RSparseVector* RSparseMatrix::operator[](size_t i) const
{
	if(i>NbLines)
		throw std::range_error("RSparseMatrix::operator[] const : index "+RString::Number(i)+" outside range (0,"+RString::Number(NbLines)+")");

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
	if(i>NbLines)
		throw std::range_error("RSparseMatrix::operator[] : index "+RString::Number(i)+" outside range (0,"+RString::Number(NbLines)+")");

	RSparseVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<RSparseVector*>(Tab[i]);
		else
		{
			for(size_t add=GetNb();add<=i;add++)
				InsertPtrAt(Line=new RSparseVector(InitNbCols,add),add,true);
		}
	}
	else
	{
		Line=GetPtr(i);
		if(!Line)
			InsertPtr(Line=new RSparseVector(InitNbCols,i));
	}
	return(Line);
}


//-----------------------------------------------------------------------------
RSparseMatrix::~RSparseMatrix(void)
{
}
