	/*

	R Project Library

	RSymmetricMatrix.cpp

	Matrix class - Implementation.

	Copyright 1999-2010 by Pascal Francq (pascal@francq.info).
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
#include <rsymmetricmatrix.h>
#include <rnumcursor.h>
#include <rstring.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//    Class RSymmetricMatrix
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RSymmetricMatrix::RSymmetricMatrix(size_t size)
	: RLowerTriangularMatrix(size)
{
}


//------------------------------------------------------------------------------
RSymmetricMatrix::RSymmetricMatrix(const RSymmetricMatrix& matrix)
	: RLowerTriangularMatrix(matrix)
{
}


//------------------------------------------------------------------------------
double RSymmetricMatrix::operator()(size_t i,size_t j) const
{
	if(j>i)
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}
	return(RLowerTriangularMatrix::operator()(i,j));
}


//------------------------------------------------------------------------------
double& RSymmetricMatrix::operator()(size_t i,size_t j)
{
	if(j>i)
	{
		size_t tmp(i);
		i=j;
		j=tmp;
	}
	return(RLowerTriangularMatrix::operator()(i,j));
}


//------------------------------------------------------------------------------
RSymmetricMatrix& RSymmetricMatrix::operator=(const RSymmetricMatrix& matrix)
{
	RLowerTriangularMatrix::operator=(matrix);
	return(*this);
}


//------------------------------------------------------------------------------
RSymmetricMatrix::~RSymmetricMatrix(void)
{
}
