/*

	R Project Library

	RMatrix.cpp

	Matrix classes - Implementation.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
using namespace R;



//------------------------------------------------------------------------------
//
//    Class RMatrix
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrix::RMatrix(void)
{
	Lin=MaxLin=Col=MaxCol=0;
	M=0;
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(size_t size)
{
	Lin=MaxLin=Col=MaxCol=size;
 	M=0;
	Init();
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(size_t L,size_t C)
{
	Lin=MaxLin=L;
	Col=MaxCol=C;
	M=0;
	Init();
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(const RMatrix& Matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;

	Lin=MaxLin=Matrix.Lin;
	Col=MaxCol=Matrix.Col;
	M=0;
	Init();
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)=(*ptrCM);
}


//------------------------------------------------------------------------------
void RMatrix::Init(void)
{
	double **ptr;
	size_t i;

	M=new double*[MaxLin];
	for(i=MaxLin+1,ptr=M;--i;ptr++)
		(*ptr) = new double[MaxCol];
}


//------------------------------------------------------------------------------
void RMatrix::VerifySize(size_t NewLin,size_t NewCol)
{
	double **ptr;
	size_t i;

	if(MaxLin<NewLin)
	{
		MaxCol = NewCol;
		for(i=MaxLin+1,ptr=M;--i;ptr++) delete[](*ptr);
		delete[] M;
		MaxLin=NewLin;
		M = new double*[MaxLin];
		for(i=MaxLin+1,ptr=M;--i;ptr++)
			(*ptr)= new double[MaxCol];
	}
	else
		if(MaxCol<NewCol)
		{
			MaxCol=NewCol;
			for(i=MaxLin+1,ptr=M;--i;ptr++)
			{
				delete[] (*ptr);
				(*ptr)= new double[MaxCol];
			}
		}
	Lin=NewLin;
	Col=NewCol;
}


//------------------------------------------------------------------------------
void RMatrix::Symetrize(void)
{
	double **ptrL,**ptrLA,*ptrC,*ptrCA;
	size_t i,j;

	for(i=Lin+1,ptrL=M,ptrLA=M+(Lin-1);--i;ptrL++,ptrLA--)
		for(j=Col+1,ptrC=(*ptrL),ptrCA=(*ptrLA)+Col-1;--j;ptrC++,ptrCA--)
			if(!(*ptrC)) (*ptrC)=(*ptrCA);
}


//------------------------------------------------------------------------------
double& RMatrix::operator()(size_t m,size_t n) const
{
	if((m>Lin)||(n>Col))
		throw RException("Bad Index");
	return(M[m][n]);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator=(const RMatrix Matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;

	if(this==&Matrix) return *this;
	VerifySize(Matrix.Lin,Matrix.Col);
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator+=(const RMatrix Matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;

	if((Lin!=Matrix.Lin)||(Col!=Matrix.Col))
		throw RException("Not Compatible Sizes");
	VerifySize(Matrix.Lin,Matrix.Col);
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)+=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator-=(const RMatrix Matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM;
	size_t i,j;

	if((Lin!=Matrix.Lin)||(Col!=Matrix.Col))
		throw RException("Not Compatible Sizes");
	VerifySize(Matrix.Lin,Matrix.Col);
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)-=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const double arg)
{
	double **ptrL,*ptrC;
	size_t i,j;

	for(i=Lin+1,ptrL=M;--i;ptrL++)
		for(j=Col+1,ptrC=*ptrL;--j;ptrC++)
			(*ptrC)*=arg;
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const RMatrix Matrix)
{
	double **ptrL,*ptrC,**ptrLM,*ptrCM,**ptrLR,*ptrCR,Sum;
	size_t i,j,k;
	RMatrix res;

	if(Lin!=Matrix.Col)
		throw RException("Not Compatible Sizes");
	res.VerifySize(Lin,Matrix.Col);
	for(i=Lin+1,ptrL=M,ptrLR=res.M;--i;ptrL++,ptrLR++)
	{
		for(j=0,ptrCR=(*ptrLR),ptrCM=(*Matrix.M);j<Matrix.Col+1;j++,ptrCR++,ptrCM++)
		{
			Sum=0;
			for(k=Col+1,ptrC=(*ptrL),ptrLM=Matrix.M;--k;ptrC++,ptrLM++) Sum += (*ptrC)*((*ptrLM)[j]);
			(*ptrCR)=Sum;
		}
	}
	for(i=Lin+1,ptrL=M,ptrLM=res.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
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
		for(i=MaxLin,ptr=M;--i;ptr++) delete[] (*ptr);
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
