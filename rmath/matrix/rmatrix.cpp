/*

	R Project Library

	RMatrix.cpp

	Matrix classes - Implementation.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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
#include <rmath/rmatrix.h>
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
RMatrix::RMatrix(tSize Size) throw(std::bad_alloc)
{
	Lin=MaxLin=Col=MaxCol=Size;
 	M=0;
	Init();
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(tSize L,tSize C) throw(std::bad_alloc)
{
	Lin=MaxLin=L;
	Col=MaxCol=C;
	M=0;
	Init();
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(const RMatrix &Matrix) throw(std::bad_alloc)
{
	tNumber **ptrL,*ptrC,**ptrLM,*ptrCM;
	tSize i,j;

	Lin=MaxLin=Matrix.Lin;
	Col=MaxCol=Matrix.Col;
	M=0;
	Init();
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)=(*ptrCM);
}


//------------------------------------------------------------------------------
RMatrix::RMatrix(const RMatrix *Matrix) throw(std::bad_alloc)
{
	tNumber **ptrL,*ptrC,**ptrLM,*ptrCM;
	tSize i,j;

	if(Matrix)
	{
		Lin=MaxLin=Matrix->Lin;
		Col=MaxCol=Matrix->Col;
		M=0;
		Init();
		for(i=Lin+1,ptrL=M,ptrLM=Matrix->M;--i;ptrL++,ptrLM++)
			for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
				(*ptrC)=(*ptrCM);
	}
	else
	{
		RMatrix();
		RReturnIfFail(Matrix);
	}
}


//------------------------------------------------------------------------------
void RMatrix::Init(void) throw(std::bad_alloc)
{
	tNumber **ptr;
	tSize i;

	M=new tNumber*[MaxLin];
	for(i=MaxLin+1,ptr=M;--i;ptr++)
		(*ptr) = new tNumber[MaxCol];
}


//------------------------------------------------------------------------------
void RMatrix::VerifySize(tSize NewLin,tSize NewCol) throw(std::bad_alloc)
{
	tNumber **ptr;
	tSize i;

	if(MaxLin<NewLin)
	{
		MaxCol = NewCol;
		for(i=MaxLin+1,ptr=M;--i;ptr++) delete[](*ptr);
		delete[] M;
		MaxLin=NewLin;
		M = new tNumber*[MaxLin];
		for(i=MaxLin+1,ptr=M;--i;ptr++)
			(*ptr)= new tNumber[MaxCol];
	}
	else
		if(MaxCol<NewCol)
		{
			MaxCol=NewCol;
			for(i=MaxLin+1,ptr=M;--i;ptr++)
			{
				delete[] (*ptr);
				(*ptr)= new tNumber[MaxCol];
			}
		}
	Lin=NewLin;
	Col=NewCol;
}


//------------------------------------------------------------------------------
void RMatrix::Symetrize(void)
{
	tNumber **ptrL,**ptrLA,*ptrC,*ptrCA;
	tSize i,j;

	for(i=Lin+1,ptrL=M,ptrLA=M+(Lin-1);--i;ptrL++,ptrLA--)
		for(j=Col+1,ptrC=(*ptrL),ptrCA=(*ptrLA)+Col-1;--j;ptrC++,ptrCA--)
			if(!(*ptrC)) (*ptrC)=(*ptrCA);
}


//------------------------------------------------------------------------------
char RMatrix::TransitiveClosure(RMatrix *Matrix) throw(RException,std::bad_alloc)
{
	tNumber **ptrL,*ptrC,**ptrLM,*ptrCM,**ptrLT,*ptrCT;
	tSize i,j,k;
	char Ok=1;

	if(Col!=Lin)
		throw RException("Not Quadratic Matrix");
	if(Matrix)
	{
		Matrix->VerifySize(Lin,Col);
		for(i=Lin+1,ptrL=M;--i;ptrL++)
			for(j=Col+1,ptrC=*ptrL,ptrLT=M,ptrLM=Matrix->M;--j;ptrC++,ptrLT++,ptrLM++)
				if(*ptrC)
					for(k=Col+1,ptrCT=*ptrLT,ptrCM=*ptrLM;--k;ptrCT++,ptrCM++)
						if(*ptrCT)
         		     (*ptrCM) = 1;
            		else
              			Ok=0;
	}
	else
		for(i=Lin+1,ptrL=M;--i;ptrL++)
			for(j=Col+1,ptrC=*ptrL,ptrLT=M;--j;ptrC++,ptrLT++)
				if(*ptrC)
					for(k=Col+1,ptrCT=*ptrLT;--k;ptrCT++)
						if(!(*ptrCT)) return(0);
	return(Ok);
}


//------------------------------------------------------------------------------
tNumber& RMatrix::operator()(int m, int n) const throw(RException,std::bad_alloc)
{
	if(m<0||m>Lin||n<0||n>Col)
		throw RException("Bad Index");
	return(M[m][n]);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator=(const RMatrix Matrix) throw(RException,std::bad_alloc)
{
	tNumber **ptrL,*ptrC,**ptrLM,*ptrCM;
	tSize i,j;

	if(this==&Matrix) return *this;
	VerifySize(Matrix.Lin,Matrix.Col);
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator+=(const RMatrix Matrix) throw(RException,std::bad_alloc)
{
	tNumber **ptrL,*ptrC,**ptrLM,*ptrCM;
	tSize i,j;

	if((Lin!=Matrix.Lin)||(Col!=Matrix.Col))
		throw RException("Not Compatible Sizes");
	VerifySize(Matrix.Lin,Matrix.Col);
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)+=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator-=(const RMatrix Matrix) throw(RException,std::bad_alloc)
{
	tNumber **ptrL,*ptrC,**ptrLM,*ptrCM;
	tSize i,j;

	if((Lin!=Matrix.Lin)||(Col!=Matrix.Col))
		throw RException("Not Compatible Sizes");
	VerifySize(Matrix.Lin,Matrix.Col);
	for(i=Lin+1,ptrL=M,ptrLM=Matrix.M;--i;ptrL++,ptrLM++)
		for(j=Col+1,ptrC=*ptrL,ptrCM=*ptrLM;--j;ptrC++,ptrCM++)
			(*ptrC)-=(*ptrCM);
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const tNumber arg) throw(RException,std::bad_alloc)
{
	tNumber **ptrL,*ptrC;
	tSize i,j;

	for(i=Lin+1,ptrL=M;--i;ptrL++)
		for(j=Col+1,ptrC=*ptrL;--j;ptrC++)
			(*ptrC)*=arg;
	return(*this);
}


//------------------------------------------------------------------------------
RMatrix& RMatrix::operator*=(const RMatrix Matrix) throw(RException,std::bad_alloc)
{
	tNumber **ptrL,*ptrC,**ptrLM,*ptrCM,**ptrLR,*ptrCR,Sum;
	tSize i,j,k;
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
	tNumber **ptr;
	tSize i;

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
RMatrix operator+(const RMatrix &arg1,const RMatrix &arg2) throw(RException,std::bad_alloc)
{
	RMatrix res(arg1);

	res+=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator-(const RMatrix &arg1,const RMatrix &arg2) throw(RException,std::bad_alloc)
{
	RMatrix res(arg1);

	res-=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator*(const RMatrix& arg1,const RMatrix& arg2) throw(RException,std::bad_alloc)
{
	RMatrix res(arg1);

	res*=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator*(const tNumber arg1,const RMatrix& arg2) throw(std::bad_alloc)
{
	RMatrix res(arg2);

	res*=arg1;
	return(res);
}


//------------------------------------------------------------------------------
RMatrix operator*(const RMatrix& arg1,const tNumber arg2) throw(std::bad_alloc)
{
	RMatrix res(arg1);

	res*=arg2;
	return(res);
}
