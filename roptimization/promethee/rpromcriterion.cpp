/*

	R Project Library

	RPromCriterion.cpp

	Promethee Criterion - Implementation.

	Copyright 2000-2008 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <math.h>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromcriterion.h>
#include <rpromcritvalue.h>
#include <rpromkernel.h>
#include <rcursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RPromCriterion
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromCriterion::RPromCriterion(tCriteriaType type,double w,const char* name,size_t nb)
	: RContainer<RPromCritValue,false,false>(nb,nb/2), Id(NullId), Name(name),
	  Type(type)
{
	Weight=w;
}


//------------------------------------------------------------------------------
void RPromCriterion::Set(R::RParam*)
{
}


//------------------------------------------------------------------------------
void RPromCriterion::Normalize(void)
{
	size_t i;
	double max,min,diff;

	// Compute maximal value
	RCursor<RPromCritValue> ptr(*this);
	ptr.Start();
	min=max=ptr()->Value;
	for(i=GetNb(),ptr.Next();--i;ptr.Next())
	{
		if(max<ptr()->Value)
			max=ptr()->Value;
		if(min>ptr()->Value)
			min=ptr()->Value;
	}
	diff=max-min;

	// Normalize
	if(diff)
	{
		for(ptr.Start();!ptr.End();ptr.Next())
			ptr()->Value=(ptr()->Value-min)/diff;
	}
}


//------------------------------------------------------------------------------
void RPromCriterion::ComputeFiCrit(RPromKernel* kern)
{
	// Calculation of Fi Crit + & -
	RCursor<RPromCritValue> ptr2(*this);
	RCursor<RPromCritValue> ptr(*this);
	RCursor<RPromSol> sol(kern->Solutions);
	RCursor<RPromSol> sol2(kern->Solutions);
	for(ptr.Start(),sol.Start();!ptr.End();ptr.Next(),sol.Next())
	{
		// Fi are zero
		ptr()->FiCritPlus=ptr()->FiCritMinus=0.0;
		for(ptr2.Start(),sol2.Start();!ptr2.End();ptr2.Next(),sol2.Next())
		{
			// Only if secondary solution is not the same than the primary one.
			if(sol()==sol2()) continue;
			ptr()->FiCritPlus+=ComputePref(ptr()->Value,ptr2()->Value);
			ptr()->FiCritMinus+=ComputePref(ptr2()->Value,ptr()->Value);
		}
	}

	// Compute Fi Crit
	for(ptr.Start();!ptr.End();ptr.Next())
		ptr()->FiCrit=ptr()->FiCritPlus-ptr()->FiCritMinus;
}


//------------------------------------------------------------------------------
RPromCriterion::~RPromCriterion(void)
{
}



//------------------------------------------------------------------------------
//
// class RPromLinearCriterion
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromLinearCriterion::RPromLinearCriterion(tCriteriaType type,double p,double q,double w,const char* name,size_t nb)
	: RPromCriterion(type,w,name,nb)
{
	Set(p,q,w);
}


//------------------------------------------------------------------------------
RPromLinearCriterion::RPromLinearCriterion(tCriteriaType type,RParam* params,const char* name,size_t nb)
	: RPromCriterion(type,0.0,name,nb)
{
	Set(params);
}


//-----------------------------------------------------------------------------
void RPromLinearCriterion::Set(RParam* param)
{
	double p,q,w;
	p=dynamic_cast<RParamStruct*>(param)->Get<RParamValue>("P")->GetDouble();
	q=dynamic_cast<RParamStruct*>(param)->Get<RParamValue>("Q")->GetDouble();
	w=dynamic_cast<RParamStruct*>(param)->Get<RParamValue>("Weight")->GetDouble();
	Set(p,q,w);
}


//-----------------------------------------------------------------------------
RParam* RPromLinearCriterion::CreateParam(const R::RString& name)
{
	RParamStruct* param=new RParamStruct(name);
	param->Insert(new RParamValue("P",0.2));
	param->Insert(new RParamValue("Q",0.05));
	param->Insert(new RParamValue("Weight",1.0));
	return(param);
}


//-----------------------------------------------------------------------------
void RPromLinearCriterion::Set(double p,double q,double w)
{
	if((p<0.0)||(q<0.0)||(p>1.0)||(q>1.0)||(p<=q))
		throw RException("Wrong Promethee Parameters: 0<Q<P<1");
	P=p;
	Q=q;
	Weight=w;
}


//------------------------------------------------------------------------------
double RPromLinearCriterion::ComputePref(double u,double v)
{
	double d=fabs(u-v);
	
	// No solution is better
	if(d<=Q)
		return(0.0);  
	
	// One solution is better than the other one but it depends of:
	// If u>v or v<u
	// If the criteria should be maximized or minimized
	if(d>=P)
	{		
		if(Type==Maximize)
		{
			if(u>v)
				return(1.0);
			else
				return(0.0);
		}
		else
		{
			if(u<v)
				return(1.0);
			else
				return(0.0);			
		}
	}
	
	// Betwwen Q and P -> Compute the prefrence.
	if(Type==Maximize)
	{
		if(u>v)
			return((d-Q)/(P-Q));
		else
			return(0);
	}
	else
	{
		if(u<v)
			return((d-Q)/(P-Q));
		else
			return(0);			
	}
}
