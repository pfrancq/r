/*

	R Project Library

	RVector.cpp

	Class representing a vector - Implementation

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
#include <rvector.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RVector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RVector::RVector(size_t max)
	: RNumContainer<double,false>(max)
{
	NbInt=max; // Suppose the vector has the correct size.
}


//------------------------------------------------------------------------------
RVector::RVector(const RVector& vec)
	: RNumContainer<double,false>(vec)
{
}


//------------------------------------------------------------------------------
RVector::~RVector(void)
{
}



//------------------------------------------------------------------------------
//
// class RMatrixLine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMatrixLine::RMatrixLine(size_t max)
	: RVector(max)
{
}


//------------------------------------------------------------------------------
RMatrixLine::RMatrixLine(const RMatrixLine& vec)
	: RVector(vec)
{
}


//------------------------------------------------------------------------------
RMatrixLine::~RMatrixLine(void)
{
}
