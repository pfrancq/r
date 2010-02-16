/*

	R Project Library

	RGenericMatrix.cpp

	Generic Matrix class - Implementation.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
#include <rgenericmatrix.h>
#include <rmatrix.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//    Class RGenericMatrix
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGenericMatrix::RGenericMatrix(size_t size)
	: NbLines(size), NbCols(size)
{
}


//------------------------------------------------------------------------------
RGenericMatrix::RGenericMatrix(size_t lines,size_t cols)
	: NbLines(lines), NbCols(cols)
{
}


//------------------------------------------------------------------------------
RGenericMatrix::RGenericMatrix(const RGenericMatrix& matrix)
	: NbLines(matrix.NbLines), NbCols(matrix.NbCols)
{
}


//------------------------------------------------------------------------------
void RGenericMatrix::Clear(double val,bool)
{
	Init(val);
}


//------------------------------------------------------------------------------
void RGenericMatrix::Init(double val)
{
	for(size_t i=0;i<NbLines;i++)
		for(size_t j=0;j<NbCols;j++)
			(*this)(i,j)=val;
}


//------------------------------------------------------------------------------
RGenericMatrix& RGenericMatrix::operator=(const RGenericMatrix& matrix)
{
	NbCols=matrix.NbLines;
	NbLines=matrix.NbCols;
	return(*this);
}


//------------------------------------------------------------------------------
RGenericMatrix::~RGenericMatrix(void)
{
}



//------------------------------------------------------------------------------
//
// General Functions and operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrix R::operator+(const RGenericMatrix& arg1,const RGenericMatrix& arg2)
{
	if((arg1.GetNbLines()!=arg2.GetNbLines())||(arg1.GetNbCols()!=arg2.GetNbCols()))
		throw std::range_error("operator+(const RGenericMatrix&,const RGenericMatrix&) : Not Compatible Sizes");

	RMatrix res(arg1.GetNbLines(),arg1.GetNbCols());
	for(size_t i=0;i<arg1.GetNbLines();i++)
		for(size_t j=0;j<arg1.GetNbCols();j++)
			res(i,j)=arg1(i,j)+arg2(i,j);
	return(res);
}


//------------------------------------------------------------------------------
RMatrix R::operator-(const RGenericMatrix& arg1,const RGenericMatrix& arg2)
{
	if((arg1.GetNbLines()!=arg2.GetNbLines())||(arg1.GetNbCols()!=arg2.GetNbCols()))
		throw std::range_error("operator-(const RGenericMatrix&,const RGenericMatrix&) : Not Compatible Sizes");

	RMatrix res(arg1.GetNbLines(),arg1.GetNbCols());
	for(size_t i=0;i<arg1.GetNbLines();i++)
		for(size_t j=0;j<arg1.GetNbCols();j++)
			res(i,j)=arg1(i,j)-arg2(i,j);
	return(res);
}

//------------------------------------------------------------------------------
RMatrix R::operator*(const double arg1,const RGenericMatrix& arg2)
{
	RMatrix res(arg2.GetNbLines(),arg2.GetNbCols());
	for(size_t i=0;i<arg2.GetNbLines();i++)
		for(size_t j=0;j<arg2.GetNbCols();j++)
			res(i,j)=arg1*arg2(i,j);
	return(res);
}


//------------------------------------------------------------------------------
RMatrix R::operator*(const RGenericMatrix& arg1,const double arg2)
{
	RMatrix res(arg1.GetNbLines(),arg1.GetNbCols());
	for(size_t i=0;i<arg1.GetNbLines();i++)
		for(size_t j=0;j<arg1.GetNbCols();j++)
			res(i,j)=arg1(i,j)*arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix R::operator*(const RGenericMatrix& arg1,const RGenericMatrix& arg2)
{
	if(arg1.GetNbCols()!=arg2.GetNbLines())
		throw std::range_error("operator*(const RGenericMatrix&,const RGenericMatrix&) : Not Compatible Sizes");


	RMatrix res(arg1.GetNbLines(),arg2.GetNbCols());
	for(size_t i=0;i<arg1.GetNbLines();i++)
		for(size_t j=0;j<arg2.GetNbCols();j++)
		{
			double Sum(0);
			for(size_t r=0;r<arg1.GetNbCols();r++)
				Sum+=arg1(i,r)*arg2(r,j);
			res(i,j)=Sum;
		}
	return(res);
}
