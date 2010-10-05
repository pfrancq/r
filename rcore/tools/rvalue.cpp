/*

	R Project Library

	RValue.cpp

	Value associate to an identifier - Implementation.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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


//------------------------------------------------------------------------------
// include files for R Project
#include <rvalue.h>
using namespace R;


//------------------------------------------------------------------------------
// Define static variables
const RValue RValue::Null(cNoRef,NAN);



//------------------------------------------------------------------------------
//
// class RValue
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RValue::RValue(void)
	: Id(cNoRef), Value(NAN)
{
}


//------------------------------------------------------------------------------
RValue::RValue(size_t id)
	: Id(id), Value(NAN)
{
}


//------------------------------------------------------------------------------
RValue::RValue(size_t id,double val)
	: Id(id), Value(val)
{
}


//------------------------------------------------------------------------------
int RValue::Compare(size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
int RValue::Compare(const RValue& obj) const
{
  return(CompareIds(Id,obj.Id));
}


//------------------------------------------------------------------------------
RValue& RValue::operator=(const RValue& val)
{
	Id=val.Id;
	Value=val.Value;
	return(*this);
}


//------------------------------------------------------------------------------
RValue& RValue::operator=(const double val)
{
	Value=val;
	return(*this);
}


//------------------------------------------------------------------------------
RValue::~RValue(void)
{
}
