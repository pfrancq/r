/*

	R Project Library

	RBoolVector.cpp

	Class representing a list of boolean values - Implementation

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#include <rstring.h>
#include <rboolvector.h>
#include <rexception.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RBoolVector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RBoolVector::RBoolVector(size_t nb,bool val)
	: NbBool(nb), MaxBool(nb), List(0), Default(val)
{
	if(!MaxBool)
		MaxBool=100;
	List = new bool[MaxBool];
	memset(List,char(Default),MaxBool*sizeof(bool));
}


//------------------------------------------------------------------------------
RBoolVector::RBoolVector(const RBoolVector& vector)
	: MaxBool(vector.MaxBool)
{
	NbBool = vector.NbBool;
	MaxBool = vector.MaxBool;
	List = new bool[MaxBool];
	memcpy(List,vector.List,vector.MaxBool*sizeof(bool));
}


//------------------------------------------------------------------------------
void RBoolVector::SetDefault(bool val)
{
	Default=val;
}


//------------------------------------------------------------------------------
void RBoolVector::Verify(size_t max)
{
	if(max>MaxBool)
	{
		size_t OldSize(MaxBool);
		MaxBool=max;
		bool* ptr(new bool[MaxBool]);
		memcpy(ptr,List,OldSize*sizeof(bool));
		delete[] List;
		List=ptr;

	}
}


//------------------------------------------------------------------------------
void RBoolVector::ReSize(size_t size)
{
	if(size>MaxBool)
	{
		 size_t OldSize(MaxBool);
		 Verify(size);
		 memset(&List[OldSize],char(Default),(MaxBool-OldSize)*sizeof(bool));
	}
	NbBool=size;
}


//------------------------------------------------------------------------------
void RBoolVector::Init(size_t nb,bool val)
{
	Verify(nb);
	NbBool=nb;
	memset(List,char(val),nb*sizeof(bool));
}


//------------------------------------------------------------------------------
bool RBoolVector::IsSame(const RBoolVector& vector) const
{
	size_t i;
	bool *ptr1,*ptr2;

	if(NbBool!=vector.NbBool) return(false);
	for(i=NbBool+1,ptr1=List,ptr2=vector.List;--i;ptr1++,ptr2++)
		if(((*ptr1)!=-1)&&((*ptr1)!=(*ptr2))) return(false);
	return(true);
}


//------------------------------------------------------------------------------
void RBoolVector::Set(bool value,size_t pos)
{
	if(pos+1>MaxBool)
		ReSize(pos+1);
	List[pos]=value;
	NbBool++;
	if(NbBool<pos+1)
		NbBool=pos+1;
}


//------------------------------------------------------------------------------
void RBoolVector::Clear(void)
{
	NbBool=0;
	memset(List,char(Default),MaxBool*sizeof(char));
}


//------------------------------------------------------------------------------
RBoolVector& RBoolVector::operator=(const RBoolVector& src)
{
	Verify(src.NbBool);
	NbBool = src.NbBool;
	memcpy(List,src.List,sizeof(bool)*NbBool);
	return(*this);
}


//------------------------------------------------------------------------------
bool RBoolVector::operator[](size_t i) const
{
	if(i>=NbBool)
	{
		if(NbBool)
			throw std::range_error(RString("RBoolVector::operator[] const : idx "+RString::Number(i)+" outside range [0,"+RString::Number(NbBool-1)+"]").ToString());
		else
			throw std::range_error("RBoolVector::operator[] const : no elements");
	}
	char ptr(List[i]);
	if(ptr==-1)
		throw RException(RString("RBoolVector::operator[](size_t) const : Undefined value at position '"+RString::Number(i)+"'").ToString());
	return(ptr);
}


//------------------------------------------------------------------------------
bool& RBoolVector::operator[](size_t i)
{
	if(i>=NbBool)
	{
		if(NbBool)
			throw std::range_error(RString("RBoolVector::operator[] : idx "+RString::Number(i)+" outside range [0,"+RString::Number(NbBool-1)+"]").ToString());
		else
			throw std::range_error("RBoolVector::operator[] : no elements");
	}
	return(List[i]);
}


//------------------------------------------------------------------------------
RBoolVector::~RBoolVector(void)
{
	delete[] List;
}
