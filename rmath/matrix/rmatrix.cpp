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
#include <rstring.h>
using namespace R;



//------------------------------------------------------------------------------
//
//    Class RMatrix
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrix::RMatrix(void)
	: Lines(0), MaxLines(0), Cols(0), MaxCols(0), M(0)
{
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(size_t size)
	: Lines(size), MaxLines(size), Cols(size), MaxCols(size), M(0)
{
	Init();
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(size_t lines,size_t cols)
	: Lines(lines), MaxLines(lines), Cols(cols), MaxCols(cols), M(0)
{
	Init();
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(const RMatrix& matrix)
: Lines(matrix.Lines), MaxLines(matrix.Lines), Cols(matrix.Cols), MaxCols(matrix.Cols), M(0)
{
	Init();
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;
	for(i=Lines+1,ptrL=M,ptrLM=matrix.M;--i;ptrL++,ptrLM++)
		for(j=Cols+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)=(*ptrCM);
}


//------------------------------------------------------------------------------
void RMatrix::Init(void)
{
	double** ptr;
	size_t i;

	M=new double*[MaxLines];
	for(i=MaxLines+1,ptr=M;--i;ptr++)
		(*ptr) = new double[MaxCols];
}


//------------------------------------------------------------------------------
void RMatrix::VerifySize(size_t newlines,size_t newcols)
{
	double** ptr;
	size_t i;

	if(MaxLines<newlines)
	{
		MaxCols = newcols;
		for(i=MaxLines+1,ptr=M;--i;ptr++) delete[](*ptr);
		delete[] M;
		MaxLines=newlines;
		M = new double*[MaxLines];
		for(i=MaxLines+1,ptr=M;--i;ptr++)
			(*ptr)= new double[MaxCols];
	}
	else
		if(MaxCols<newcols)
		{
			MaxCols=newcols;
			for(i=MaxLines+1,ptr=M;--i;ptr++)
			{
				delete[] (*ptr);
				(*ptr)= new double[MaxCols];
			}
		}
	Lines=newlines;
	Cols=newcols;
}


//------------------------------------------------------------------------------
void RMatrix::Symetrize(void)
{
	double **ptrL,**ptrLA,*ptrC,*ptrCA;
	size_t i,j;

	for(i=Lines+1,ptrL=M,ptrLA=M+(Lines-1);--i;ptrL++,ptrLA--)
		for(j=Cols+1,ptrC=(*ptrL),ptrCA=(*ptrLA)+Cols-1;--j;ptrC++,ptrCA--)
			if(!(*ptrC)) (*ptrC)=(*ptrCA);
}


//------------------------------------------------------------------------------
double& RMatrix::operator()(size_t i,size_t j) const
{
	if((i>Lines)||(j>Cols))
		throw std::range_error("RMatrix::operator() const : index "+RString::Number(i)+","+RString::Number(j)+" outside range ("+RString::Number(Lines)+","+RString::Number(Cols)+")");
	return(M[i][j]);
}


//------------------------------------------------------------------------------
const double* RMatrix::operator[](size_t i) const
{
	if(i>Lines)
		throw std::range_error("RMatrix::operator[] const : index "+RString::Number(i)+" outside range (0,"+RString::Number(Lines)+")");
	return(M[i]);
}


//------------------------------------------------------------------------------
double* RMatrix::operator[](size_t i)
{
	if(i>Lines)
		throw std::range_error("RMatrix::operator[] const : index "+RString::Number(i)+" outside range (0,"+RString::Number(Lines)+")");
	return(M[i]);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator=(const RMatrix& matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;

	if(this==&matrix) return(*this);
	VerifySize(matrix.Lines,matrix.Cols);
	for(i=Lines+1,ptrL=M,ptrLM=matrix.M;--i;ptrL++,ptrLM++)
		for(j=Cols+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator+=(const RMatrix& matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;

	if((Lines!=matrix.Lines)||(Cols!=matrix.Cols))
		throw std::range_error("RMatrix::operator+= : Not Compatible Sizes");
	for(i=Lines+1,ptrL=M,ptrLM=matrix.M;--i;ptrL++,ptrLM++)
		for(j=Cols+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)+=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator-=(const RMatrix& matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;

	if((Lines!=matrix.Lines)||(Cols!=matrix.Cols))
		throw std::range_error("RMatrix::operator-= : Not Compatible Sizes");
	for(i=Lines+1,ptrL=M,ptrLM=matrix.M;--i;ptrL++,ptrLM++)
		for(j=Cols+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)-=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const double arg)
{
	double **ptrL,*ptrC;
	size_t i,j;

	for(i=Lines+1,ptrL=M;--i;ptrL++)
		for(j=Cols+1,ptrC=*ptrL;--j;ptrC++)
			(*ptrC)*=arg;
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const RMatrix& matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM,**ptrLR,*ptrCR,Sum;
	size_t i,j,k;
	RMatrix res;

	if(Lines!=matrix.Cols)
		throw std::range_error("RMatrix::operator*= : Not Compatible Sizes");
	res.VerifySize(Lines,matrix.Cols);
	for(i=Lines+1,ptrL=M,ptrLR=res.M;--i;ptrL++,ptrLR++)
	{
		for(j=0,ptrCR=(*ptrLR),ptrCM=(*matrix.M);j<matrix.Cols+1;j++,ptrCR++,ptrCM++)
		{
			Sum=0;
			for(k=Cols+1,ptrC=(*ptrL),ptrLM=matrix.M;--k;ptrC++,ptrLM++)
					Sum += (*ptrC)*((*ptrLM)[j]);
			(*ptrCR)=Sum;
		}
	}
	for(i=Lines+1,ptrL=M,ptrLM=res.M;--i;ptrL++,ptrLM++)
		for(j=Cols+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix::~RMatrix(void)
{
	double **ptr;
	size_t i;

	if(M)
	{
		for(i=MaxLines,ptr=M;--i;ptr++) delete[] (*ptr);
		delete[] M;
	}
}



//------------------------------------------------------------------------------
//
// General Functions and operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrix operator+(const RMatrix &arg1,const RMatrix &arg2)
{
	RMatrix res(arg1);

	res+=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator-(const RMatrix &arg1,const RMatrix &arg2)
{
	RMatrix res(arg1);

	res-=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator*(const RMatrix& arg1,const RMatrix& arg2)
{
	RMatrix res(arg1);

	res*=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator*(const double arg1,const RMatrix& arg2)
{
	RMatrix res(arg2);

	res*=arg1;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator*(const RMatrix& arg1,const double arg2)
{
	RMatrix res(arg1);

	res*=arg2;
	return(res);
}
