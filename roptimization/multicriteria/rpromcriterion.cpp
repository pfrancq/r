/*

	R Project Library

	RPromCriterion.cpp

	PROMETHEE Criterion - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rpromcriterion.h>
#include <rpromcritvalue.h>
#include <rpromkernel.h>
#include <rcursor.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// class RPromCriterion
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromCriterion::RPromCriterion(tCriteriaType type,double w,const RString& name,size_t nb)
	: RContainer<RPromCritValue,false,false>(nb,nb/2), Id(cNoRef), Name(name),
	  Type(type), Active(true)
{
	Weight=w;
}


//------------------------------------------------------------------------------
void RPromCriterion::Set(const RParam*)
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
			ptr()->Normalized=(ptr()->Value-min)/diff;
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
			ptr()->FiCritPlus+=ComputePref(ptr()->Normalized,ptr2()->Normalized);
			ptr()->FiCritMinus+=ComputePref(ptr2()->Normalized,ptr()->Normalized);
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
RPromLinearCriterion::RPromLinearCriterion(tCriteriaType type,double p,double q,double w,const RString& name,size_t nb)
	: RPromCriterion(type,w,name,nb)
{
	Set(p,q,w);
}


//------------------------------------------------------------------------------
RPromLinearCriterion::RPromLinearCriterion(tCriteriaType type,const RParam* params,const RString& name,size_t nb)
	: RPromCriterion(type,0.0,name,nb)
{
	Set(params);
}


//-----------------------------------------------------------------------------
void RPromLinearCriterion::Set(const RParam* param)
{
	double p,q,w;
	bool a;
	const RParamStruct* Param(dynamic_cast<const RParamStruct*>(param));
	if(!Param)
		mThrowRException("Parameter '"+param->GetName()+"' is not of type 'RParamStuct'");
	RParamValue* Val(Param->Get<RParamValue>("P"));
	if(!Val)
		mThrowRException("Parameter '"+param->GetName()+"' has no member 'P'");
	p=Val->GetDouble();
	Val=Param->Get<RParamValue>("Q");
	if(!Val)
		mThrowRException("Parameter '"+param->GetName()+"' has no member 'Q'");
	q=Val->GetDouble();
	Val=Param->Get<RParamValue>("Weight");
	if(!Val)
		mThrowRException("Parameter '"+param->GetName()+"' has no member 'Weight'");
	w=Val->GetDouble();
	Val=Param->Get<RParamValue>("Active");
	if(!Val)
		mThrowRException("Parameter '"+param->GetName()+"' has no member 'Active'");
	a=Val->GetBool();
	Set(p,q,w,a);
}


//-----------------------------------------------------------------------------
void RPromLinearCriterion::Set(double p,double q,double w,bool active)
{
	if((p<0.0)||(q<0.0)||(p>1.0)||(q>1.0)||(p<=q))
		throw RException("Wrong Promethee Parameters: 0<Q<P<1");
	P=p;
	Q=q;
	Weight=w;
	Active=active;
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

	// Between Q and P -> Compute the preference.
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


//-----------------------------------------------------------------------------
RParam* RPromLinearCriterion::CreateParam(const R::RString& name,const RString& desc)
{
	RParamStruct* param=new RParamStruct(name,desc);
	param->Insert(new RParamValue("P",0.2,"Preference threshold"));
	param->Insert(new RParamValue("Q",0.05,"Indifference threshold"));
	param->Insert(new RParamValue("Weight",1.0,"Weight"));
	param->Insert(new RParamValue("Active",true,"Active"));
	return(param);
}
