	/*

	R Project Library

	RLowerTriangularMatrix.cpp

	Matrix class - Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rlowertriangularmatrix.h>
#include <rnumcursor.h>
#include <rstring.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//    Class RLowerTriangularMatrix
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLowerTriangularMatrix::RLowerTriangularMatrix(size_t size)
	: RMatrix(size)
{
}


//------------------------------------------------------------------------------
RLowerTriangularMatrix::RLowerTriangularMatrix(const RLowerTriangularMatrix& matrix)
	: RMatrix(matrix)
{
}


//------------------------------------------------------------------------------
int RLowerTriangularMatrix::Compare(const RLowerTriangularMatrix&) const
{
	return(-1);
}


//------------------------------------------------------------------------------
void RLowerTriangularMatrix::Create(void)
{
	RVector* ptr;
	for(size_t i=0;i<NbLines;i++)
	{
		InsertPtrAt(ptr=new RVector(i+1),i,true);
		ptr->InsertAt(NAN,i,true);
	}
	MustCreate=false;
}


//------------------------------------------------------------------------------
void RLowerTriangularMatrix::VerifySize(size_t newlines,size_t newcols,bool fill,double val)
{
	if(newlines!=newcols)
		throw std::range_error("RLowerTriangularMatrix::VerifySize(size_t,size_t) : Matrix must be squared");

	TestThis();

	// Verify the lines
	if(newlines>NbLines)
	{
		RVector* ptr;

		// New lines must be added.
		VerifyTab(newlines);
		for(size_t i=NbLines;i<newlines;i++)
		{
			InsertPtrAt(ptr=new RVector(i+1),i,true);
			if(fill)
			{
				for(size_t j=0;j<=i;j++)
					ptr->InsertAt(val,j,true);
			}
			else
				ptr->InsertAt(NAN,i,true);
		}
	}
	else if(newlines<NbLines)
	{
		// A number of lines must be deleted
		for(size_t i=NbLines-newlines+1;--i;)
			DeletePtrAt(GetNb()-1);
	}

	NbLines=newlines;
	NbCols=newcols;
}


//------------------------------------------------------------------------------
double RLowerTriangularMatrix::operator()(size_t i,size_t j) const
{
	if((i>=NbLines)||(j>=NbCols))
		throw std::range_error("RLowerTriangularMatrix::operator() const : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");
	if(j>i)
		return(0.0);
	TestThis();
	return((*static_cast<const RVector*>(Tab[i]))[j]);
}


//------------------------------------------------------------------------------
double& RLowerTriangularMatrix::operator()(size_t i,size_t j)
{
	if((i>=NbLines)||(j>=NbCols))
		throw std::range_error("RLowerTriangularMatrix::operator() : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(NbLines)+","+RString::Number(NbCols)+")");
	if(j>i)
		throw std::range_error("RLowerTriangularMatrix::operator() : Invalid column "+RString::Number(j)+" for line "+RString::Number(i));
	TestThis();
	return((*static_cast<RVector*>(Tab[i]))[j]);
}


//------------------------------------------------------------------------------
RLowerTriangularMatrix& RLowerTriangularMatrix::operator=(const RLowerTriangularMatrix& matrix)
{
	RMatrix::operator=(matrix);
	return(*this);
}


//------------------------------------------------------------------------------
RLowerTriangularMatrix::~RLowerTriangularMatrix(void)
{
}
