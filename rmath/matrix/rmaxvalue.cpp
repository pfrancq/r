/*

	R Project Library

	RMaxValue.cpp

	Ascending Ordered value associate to an identifier - Implementation.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files ANSI C/C++
#include <math.h>
#include <limits>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rmaxvalue.h>
using namespace R;


//------------------------------------------------------------------------------
// Define static variables
const RMaxValue RMaxValue::Null(cNoRef,0.0);



//------------------------------------------------------------------------------
//
// class RMaxValue
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RMaxValue::RMaxValue(void)
	: Id(cNoRef), Value(-numeric_limits<double>::min())
{
}

//------------------------------------------------------------------------------
RMaxValue::RMaxValue(const RMaxValue& val)
	: Id(val.Id), Value(val.Value)
{
}


//------------------------------------------------------------------------------
RMaxValue::RMaxValue(size_t id)
	: Id(id), Value(-numeric_limits<double>::min())
{
}


//------------------------------------------------------------------------------
RMaxValue::RMaxValue(size_t id,double val)
	: Id(id), Value(val)
{
}



//------------------------------------------------------------------------------
int RMaxValue::Compare(const RMaxValue& obj) const
{
	double Comp(Value-obj.Value);
	if(Comp>0.0)
		return(-1);
	else if(Comp<0.0)
		return(1);
	return(CompareIds(Id,obj.Id));
}


//------------------------------------------------------------------------------
int RMaxValue::Compare(double val) const
{
	double Comp(Value-val);
	if(Comp>0.0)
		return(-1);
	else if(Comp<0.0)
		return(1);
	return(0);
}


//-----------------------------------------------------------------------------
int RMaxValue::Compare(size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
RMaxValue& RMaxValue::operator=(const RMaxValue& val)
{
	Id=val.Id;
	Value=val.Value;
	return(*this);
}


//------------------------------------------------------------------------------
RMaxValue& RMaxValue::operator=(const double val)
{
	Value=val;
	return(*this);
}


//------------------------------------------------------------------------------
RMaxValue::~RMaxValue(void)
{
}
