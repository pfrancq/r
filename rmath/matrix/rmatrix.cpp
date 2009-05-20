/*

	R Project Library

	RMatrix.cpp

	Matrix class - Implementation.

	Copyright 1999-2009 by Pascal Francq (pascal@francq.info).
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
#include <rmatrix.h>
#include <rnumcursor.h>
#include <rstring.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//    Class RMatrix
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrix::RMatrix(size_t size)
	: RContainer<RVector,true,false>(size), NbCols(size)
{
		Create(size,size);
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(size_t lines,size_t cols)
	: RContainer<RVector,true,false>(lines), NbCols(cols)
{
	Create(lines,cols);
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(const RMatrix& matrix)
	: RContainer<RVector,true,false>(matrix), NbCols(matrix.NbCols)
{
}


//------------------------------------------------------------------------------
void RMatrix::Create(size_t lines,size_t cols)
{
	RVector* ptr;
	for(size_t i=0;i<lines;i++)
	{
		InsertPtrAt(ptr=new RVector(cols),i,true);
		ptr->InsertAt(NAN,cols-1,true);
	}
}


//------------------------------------------------------------------------------
void RMatrix::VerifySize(size_t newlines,size_t newcols)
{
	size_t nblines(GetNb());

	// Verify the lines
	if(newlines>GetNb())
	{
		RVector* ptr;

		// New lines must be added.
		VerifyTab(newlines);
		for(size_t i=GetNb();i<newlines;i++)
		{
			InsertPtrAt(ptr=new RVector(newcols),i,true);
			ptr->InsertAt(NAN,newcols-1,true);
		}
	}
	else if(newlines<GetNb())
	{
		// A number of lines must be deleted
		for(size_t i=GetNb()-newlines+1;--i;)
			DeletePtrAt(GetNb()-1);
	}

	// Verify the columns
	if(newcols>NbCols)
	{
		// Each vector must be expanded
		RCursor<RVector> Cur(*this,0,nblines);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->InsertAt(NAN,newcols-1,true);
		NbCols=newcols;
	}
	else if(newcols<NbCols)
	{
		// Each vector must be resized
		RCursor<RVector> Cur(*this,0,nblines);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			for(size_t i=NbCols-newcols+1;--i;)
				Cur()->DeleteAt(GetNb()-1,false);
		}
		NbCols=newcols;
	}
}


//------------------------------------------------------------------------------
void RMatrix::Init(double val)
{
	RCursor<RVector> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RNumCursor<double> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()=val;
	}
}


//------------------------------------------------------------------------------
void RMatrix::Symetrize(void)
{
	if(NbCols!=GetNb())
		throw std::range_error("RMatrix::Symetrize() : Not a symmetric matrix");

	RCursor<RVector> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		const double* ptr(Cur()->GetList());
		for(size_t j=0;j<Cur.GetPos();j++)
			(*this)[j][Cur.GetPos()]=ptr[j];
	}
}


//------------------------------------------------------------------------------
double RMatrix::operator()(size_t i,size_t j) const
{
	if((i>GetNb())||(j>NbCols))
		throw std::range_error("RMatrix::operator() const : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(GetNb())+","+RString::Number(NbCols)+")");
	return((*static_cast<RVector*>(Tab[i]))[j]);
}


//------------------------------------------------------------------------------
double& RMatrix::operator()(size_t i,size_t j)
{
	if((i>GetNb())||(j>NbCols))
		throw std::range_error("RMatrix::operator() const : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(GetNb())+","+RString::Number(NbCols)+")");
	return((*static_cast<RVector*>(Tab[i]))[j]);
}


//------------------------------------------------------------------------------
const RVector* RMatrix::operator[](size_t i) const
{
	if(i>GetNb())
		throw std::range_error("RMatrix::operator[] const : index "+RString::Number(i)+" outside range (0,"+RString::Number(GetNb())+")");
	return(static_cast<RVector*>(Tab[i]));
}


//------------------------------------------------------------------------------
RVector* RMatrix::operator[](size_t i)
{
	if(i>GetNb())
		throw std::range_error("RMatrix::operator[] const : index "+RString::Number(i)+" outside range (0,"+RString::Number(GetNb())+")");
	return(static_cast<RVector*>(Tab[i]));
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator=(const RMatrix& matrix)
{
	NbCols=matrix.NbCols;
	RContainer<RVector,true,false>::operator=(matrix);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator+=(const RMatrix& matrix)
{
	if((GetNb()!=matrix.GetNb())||(NbCols!=matrix.NbCols))
		throw std::range_error("RMatrix::operator+= : Not Compatible Sizes");
	RCursor<RVector> Cur(*this);
	RCursor<RVector> Cur2(matrix);
	for(Cur.Start(),Cur2.Start();!Cur.End();Cur.Next(),Cur2.Next())
	{
		RNumCursor<double> Vec1(*Cur());
		RNumCursor<double> Vec2(*Cur2());
		for(Vec1.Start(),Vec2.Start();!Vec1.End();Vec1.Next(),Vec2.Next())
			Vec1()+=Vec2();
	}
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator-=(const RMatrix& matrix)
{
	if((GetNb()!=matrix.GetNb())||(NbCols!=matrix.NbCols))
		throw std::range_error("RMatrix::operator-= : Not Compatible Sizes");
	RCursor<RVector> Cur(*this);
	RCursor<RVector> Cur2(matrix);
	for(Cur.Start(),Cur2.Start();!Cur.End();Cur.Next(),Cur2.Next())
	{
		RNumCursor<double> Vec1(*Cur());
		RNumCursor<double> Vec2(*Cur2());
		for(Vec1.Start(),Vec2.Start();!Vec1.End();Vec1.Next(),Vec2.Next())
			Vec1()-=Vec2();
	}
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const double arg)
{
	RCursor<RVector> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RNumCursor<double> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()*=arg;
	}
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const RMatrix& matrix)
{
	if(NbCols!=matrix.GetNb())
		throw std::range_error("RMatrix::operator*= : Not Compatible Sizes");

	// Copy in res the results of the product
	RMatrix res(GetNb(),matrix.NbCols);
	RCursor<RVector> Cur(*this);
	RCursor<RVector> Cur2(matrix);
	RCursor<RVector> Cur3(res);
	for(Cur.Start(),Cur3.Start();!Cur3.End();Cur.Next(),Cur3.Next())
	{
		RNumCursor<double> LineRes(*Cur3());
		for(LineRes.Start();!LineRes.End();LineRes.Next())
		{
			double Sum(0);
			RNumCursor<double> Line(*Cur());
			for(Line.Start(),Cur2.Start();!Line.End();Line.Next(),Cur2.Next())
				Sum+=Line()*(*Cur2())[LineRes.GetPos()];
			LineRes()=Sum;
		}
	}

	// Copy res in this
	(*this)=res;
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix::~RMatrix(void)
{
}



//------------------------------------------------------------------------------
//
// General Functions and operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrix R::operator+(const RMatrix &arg1,const RMatrix &arg2)
{
	RMatrix res(arg1);

	res+=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix R::operator-(const RMatrix &arg1,const RMatrix &arg2)
{
	RMatrix res(arg1);

	res-=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix R::operator*(const RMatrix& arg1,const RMatrix& arg2)
{
	RMatrix res(arg1);

	res*=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix R::operator*(const double arg1,const RMatrix& arg2)
{
	RMatrix res(arg2);

	res*=arg1;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix R::operator*(const RMatrix& arg1,const double arg2)
{
	RMatrix res(arg1);

	res*=arg2;
	return(res);
}
