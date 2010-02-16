/*

	R Project Library

	RVectorBool.cpp

	Class representing a list of boolean values - Implementation

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
#include <rvectorbool.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RVectorBool
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RVectorBool::RVectorBool(size_t max)
	: MaxBool(max)
{
	NbBool = 0;
	if(!max)
		MaxBool=100;
	List = new char[MaxBool];
	memset(List,-1,MaxBool*sizeof(char));
}


//------------------------------------------------------------------------------
RVectorBool::RVectorBool(const RVectorBool& vector)
	: MaxBool(vector.MaxBool)
{
	NbBool = vector.NbBool;
	List = new char[MaxBool];
	memcpy(List,vector.List,vector.MaxBool*sizeof(char));
}


//------------------------------------------------------------------------------
void RVectorBool::Verify(size_t max)
{
	if(max>MaxBool)
	{
		char* ptr;
		size_t OldSize;

		OldSize=MaxBool;
		MaxBool+=(MaxBool/2);
		if(max>MaxBool)
			MaxBool=max;
		ptr=new char[MaxBool];
		memcpy(ptr,List,OldSize*sizeof(char));
		delete[] List;
		List=ptr;
		memset(&List[OldSize],-1,(MaxBool-OldSize)*sizeof(char));
	}
}

//------------------------------------------------------------------------------
void RVectorBool::Init(size_t nb,bool val)
{
	Verify(nb);
	NbBool = nb;
	char* ptr(List);
	for(++nb;--nb;ptr++)
		(*ptr)=val;
}

//------------------------------------------------------------------------------
bool RVectorBool::IsSame(const RVectorBool& vi) const
{
	size_t i;
	char *ptr1,*ptr2;

	if(NbBool!=vi.NbBool) return(false);
	for(i=NbBool+1,ptr1=List,ptr2=vi.List;--i;ptr1++,ptr2++)
		if(((*ptr1)!=-1)&&((*ptr1)!=(*ptr2))) return(false);
	return(true);
}


//------------------------------------------------------------------------------
void RVectorBool::Set(bool value,size_t pos)
{
	if(pos+1>MaxBool)
		Verify(pos+1);
	List[pos]=value;
	NbBool++;
	if(NbBool<pos+1)
		NbBool=pos+1;
}


//------------------------------------------------------------------------------
void RVectorBool::Clear(void)
{
	NbBool=0;
	memset(List,-1,MaxBool*sizeof(char));
}


//------------------------------------------------------------------------------
RVectorBool& RVectorBool::operator=(const RVectorBool& src)
{
	Verify(src.NbBool);
	NbBool = src.NbBool;
	memcpy(List,src.List,sizeof(char)*NbBool);
	return(*this);
}


//------------------------------------------------------------------------------
bool RVectorBool::operator[](size_t i) const
{
	if(i>=NbBool)
	{
		if(NbBool)
			throw std::range_error("RVectorBool::operator[] const : idx "+RString::Number(i)+" outside range [0,"+RString::Number(NbBool-1)+"]");
		else
			throw std::range_error("RVectorBool::operator[] const : no elements");
	}
	char ptr(List[i]);
	if(ptr==-1)
		throw RException("RVectorBool::operator[](size_t) : Undefined value at position '"+RString::Number(i)+"'");
	return(ptr);
}


//------------------------------------------------------------------------------
char& RVectorBool::operator[](size_t i)
{
	if(i>=NbBool)
	{
		if(NbBool)
			throw std::range_error("RVectorBool::operator[] : idx "+RString::Number(i)+" outside range [0,"+RString::Number(NbBool-1)+"]");
		else
			throw std::range_error("RVectorBool::operator[] : no elements");
	}
	return(List[i]);
}


//------------------------------------------------------------------------------
void RVectorBool::Next(void)
{
	Pos++;
	Parse++;
	while(((*Parse)==-1)&&(Pos!=NbBool))
	{
		Pos++;
		Parse++;
	}
}


//------------------------------------------------------------------------------
RVectorBool::~RVectorBool(void)
{
	delete[] List;
}
