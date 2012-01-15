/*
	R Project Library

	RMaxMatrix.cpp

	Ascending Ordered Sparse Matrix - Implementation.

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
#include <rmaxmatrix.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// RMaxMatrix
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RMaxMatrix::RMaxMatrix(size_t nblines,size_t nbcols,bool alllines,size_t init)
	: RGenericMatrix(nblines,nbcols), RContainer<RMaxVector,true,true>(nblines),
	  AllLines(alllines), InitNbCols(init)
{
	if(AllLines)
	{
		for(size_t i=0;i<NbLines;i++)
			InsertPtrAt(new RMaxVector(InitNbCols,i),i,true);
	}
}


//-----------------------------------------------------------------------------
RMaxMatrix::RMaxMatrix(const RMaxMatrix& src)
	: RGenericMatrix(src), RContainer<RMaxVector,true,true>::RContainer(src), AllLines(src.AllLines)
{
}


//-----------------------------------------------------------------------------
void RMaxMatrix::Clear(double,bool clean)
{
	if(clean)
		RContainer<RMaxVector,true,true>::Clear();
	else
	{
		RCursor<RMaxVector> Cur(*this);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Clear();
	}
}


//------------------------------------------------------------------------------
void RMaxMatrix::Init(double)
{
	ThrowRException("Not valid operator for RMaxMatrix.");
}


//------------------------------------------------------------------------------
void RMaxMatrix::VerifySize(size_t newlines,size_t newcols,bool,double)
{
	// Verify the lines
	if(newlines>NbLines)
	{
		// New lines must be added.
		VerifyTab(newlines);
		if(AllLines)
		{
			for(size_t i=NbLines;i<newlines;i++)
				InsertPtrAt(new RMaxVector(InitNbCols,i),i,true);
		}
	}
	else if(newlines<NbLines)
	{
		// A number of lines must be deleted
		for(size_t i=NbLines+1;--i;)
		{
			// If the line correspond to a identifier smaller than the number of lines
			// -> All necessary lines are removed.
			if(RContainer<RMaxVector,true,true>::operator[](GetNb()-1)->Id<newlines)
				break;

			DeletePtrAt(GetNb()-1);
		}
	}

	// Verify the columns
	if(newcols<NbCols)
	{
		// In each vector, the non-null elements between newcols and NbCols must be removed.
		RCursor<RMaxVector> Cur(*this,0,NbLines-1);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Treat the elements in reverse order since the the columns to remove
			// are stored at the end of the vector
			for(size_t i=NbCols+1;--i;)
			{
				// If the last element is inside the new number of columns -> vector is treated.
				if(Cur()->RContainer<RMaxValue,true,true>::operator[](GetNb()-1)->Id<newcols)
					break;

				Cur()->DeletePtrAt(GetNb()-1,false);
			}
		}
	}

	NbLines=newlines;
	NbCols=newcols;
}


//-----------------------------------------------------------------------------
RMaxMatrix& RMaxMatrix::operator=(const RMaxMatrix& matrix)
{
	RGenericMatrix::operator=(matrix);
	RContainer<RMaxVector,true,true>::operator=(matrix);
	AllLines=matrix.AllLines;
	return(*this);
}


//------------------------------------------------------------------------------
double RMaxMatrix::operator()(size_t i,size_t j) const
{
	if((i>NbLines)||(j>NbCols))
		throw std::range_error("RMaxMatrix::operator() const : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");

	const RMaxVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<const RMaxVector*>(Tab[i]);
		else
			return(0);
	}
	else
		Line=(*this)[i];
	return((*Line)[j]);
}


//------------------------------------------------------------------------------
double& RMaxMatrix::operator()(size_t,size_t)
{
	ThrowRException("Not valid operator for RMaxMatrix : use Add");
}


//------------------------------------------------------------------------------
void RMaxMatrix::Add(size_t i,size_t j,double val)
{
	if(i>NbLines)
		throw std::range_error("RMaxMatrix::Add(size_t,size_t,double) : index "+RString::Number(i)+" outside range ("+RString::Number(NbLines)+")");

	RMaxVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<RMaxVector*>(Tab[i]);
		else
		{
			for(size_t add=GetNb();add<=i;add++)
				InsertPtrAt(Line=new RMaxVector(InitNbCols,add),add,true);
		}
	}
	else
	{
		Line=GetPtr(i);
		if(!Line)
			InsertPtr(Line=new RMaxVector(InitNbCols,i));
	}
	if(Line->GetNb()+1>=NbLines)
		throw std::range_error("RMaxMatrix::Add(size_t,size_t,double) : Maximal number of elements for index "+RString::Number(i));
	Line->Add(j,val);
}


//------------------------------------------------------------------------------
const RMaxVector* RMaxMatrix::operator[](size_t i) const
{
	if(i>NbLines)
		throw std::range_error("RMaxMatrix::operator[] const : index "+RString::Number(i)+" outside range (0,"+RString::Number(NbLines)+")");

	if(AllLines)
	{
		if(i<GetNb())
			return(RContainer<RMaxVector,true,true>::operator[](i));
		else
			return(0);
	}
	return(GetPtr(i));
}


//------------------------------------------------------------------------------
RMaxVector* RMaxMatrix::operator[](size_t i)
{
	if(i>NbLines)
		throw std::range_error("RMaxMatrix::operator[] : index "+RString::Number(i)+" outside range (0,"+RString::Number(NbLines)+")");

	RMaxVector* Line;

	if(AllLines)
	{
		if(i<GetNb())
			Line=static_cast<RMaxVector*>(Tab[i]);
		else
		{
			for(size_t add=GetNb();add<=i;add++)
				InsertPtrAt(Line=new RMaxVector(InitNbCols,add),add,true);
		}
	}
	else
	{
		Line=GetPtr(i);
		if(!Line)
			InsertPtr(Line=new RMaxVector(InitNbCols,i));
	}
	return(Line);
}


//-----------------------------------------------------------------------------
RMaxMatrix::~RMaxMatrix(void)
{
}
