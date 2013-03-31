/*

	R Project Library

	RVector.cpp

	Class representing a vector - Implementation

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
// include files for ANSI C/C++
#include <math.h>


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
	NbInt=vec.NbInt; // Suppose the vector has the correct size.
}


//------------------------------------------------------------------------------
int RVector::Compare(const RVector&) const
{
	return(-1);
}


//------------------------------------------------------------------------------
void RVector::Init(double val)
{
	RNumCursor<double> Vec1(*this);
	for(Vec1.Start();!Vec1.End();Vec1.Next())
		Vec1()=val;
}


//------------------------------------------------------------------------------
RVector& RVector::operator+=(const RVector& vector)
{
	if(GetNb()!=vector.GetNb())
		throw std::range_error("R::operator* : Not Compatible Sizes");
	RNumCursor<double> Vec1(*this);
	RNumCursor<double> Vec2(vector);
	for(Vec1.Start(),Vec2.Start();!Vec1.End();Vec1.Next(),Vec2.Next())
		Vec1()+=Vec2();
	return(*this);
}


//------------------------------------------------------------------------------
RVector& RVector::operator-=(const RVector& vector)
{
	if(GetNb()!=vector.GetNb())
		throw std::range_error("R::operator* : Not Compatible Sizes");
	RNumCursor<double> Vec1(*this);
	RNumCursor<double> Vec2(vector);
	for(Vec1.Start(),Vec2.Start();!Vec1.End();Vec1.Next(),Vec2.Next())
		Vec1()-=Vec2();
	return(*this);
}


//------------------------------------------------------------------------------
RVector& RVector::operator*=(const double arg)
{
	RNumCursor<double> Vec(*this);
	for(Vec.Start();!Vec.End();Vec.Next())
		Vec()*=arg;
	return(*this);
}


//------------------------------------------------------------------------------
RVector& RVector::operator/=(const double arg)
{
	RNumCursor<double> Vec(*this);
	for(Vec.Start();!Vec.End();Vec.Next())
		Vec()/=arg;
	return(*this);
}


//------------------------------------------------------------------------------
RVector::~RVector(void)
{
}



//------------------------------------------------------------------------------
//
// General Functions and operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RVector R::operator+(const RVector &arg1,const RVector &arg2)
{
	RVector res(arg1);

	res+=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RVector R::operator-(const RVector &arg1,const RVector &arg2)
{
	RVector res(arg1);

	res-=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RVector R::operator*(const double arg1,const RVector& arg2)
{
	RVector res(arg2);

	res*=arg1;
	return(res);
}


//------------------------------------------------------------------------------
RVector R::operator*(const RVector& arg1,const double arg2)
{
	RVector res(arg1);

	res*=arg2;
	return(res);
}


//------------------------------------------------------------------------------
RVector R::operator/(const double arg1,const RVector& arg2)
{
	RVector res(arg2);

	res/=arg1;
	return(res);
}


//------------------------------------------------------------------------------
RVector R::operator/(const RVector& arg1,const double arg2)
{
	RVector res(arg1);

	res/=arg2;
	return(res);
}


//------------------------------------------------------------------------------
double R::operator*(const RVector& arg1,const RVector& arg2)
{
	if(arg1.GetNb()!=arg2.GetNb())
		throw std::range_error("R::operator* : Not Compatible Sizes");

	double Sum(0.0);
	double Norm1(0.0);
	double Norm2(0.0);
	for(size_t i=0;i<arg1.GetNb();i++)
	{
		Sum+=arg1[i]*arg2[i];
		Norm1+=arg1[i]*arg1[i];
		Norm2+=arg2[i]*arg2[i];
	}
	if(Sum==0.0)
		return(0.0);
	return(Sum/(sqrt(Norm1)*sqrt(Norm2)));
}
