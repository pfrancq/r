/*

	R Project Library

	RPromCriterion.cpp

	Promethee Criterion - Implementation.

	Copyright 2000-2003 by the Universit�Libre de Bruxelles.

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


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromcriterion.h>
#include <rpromcritvalue.h>
#include <rpromkernel.h>
#include <rcursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RPromCriterionParams
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromCriterionParams::RPromCriterionParams(void)
	: P(0.2), Q(0.05), Weight(1.0)
{
}


//------------------------------------------------------------------------------
RPromCriterionParams::RPromCriterionParams(double p,double q,double w)
	: P(p), Q(q), Weight(w)
{
}


//------------------------------------------------------------------------------
RPromCriterionParams::RPromCriterionParams(const RPromCriterionParams& p)
	: P(p.P), Q(p.Q), Weight(p.Weight)
{
}


//------------------------------------------------------------------------------
RPromCriterionParams::RPromCriterionParams(const RPromCriterionParams* p)
	: P(0.2), Q(0.05), Weight(1.0)
{
	if(!p) return;
	P=p->P;
	Q=p->Q;
	Weight=p->Weight;
}


//------------------------------------------------------------------------------
void RPromCriterionParams::Set(const char* values)
{
	sscanf(values,"%lf %lf %lf",&P,&Q,&Weight);
}


//------------------------------------------------------------------------------
RPromCriterionParams& RPromCriterionParams::operator=(const RPromCriterionParams &params)
{
	P=params.P;
	Q=params.Q;
	Weight=params.Weight;
	return(*this);
}


//-----------------------------------------------------------------------------
void RPromCriterionParams::Set(RParamStruct* param)
{
	P=param->Get<RParamValue>("P")->GetDouble();
	Q=param->Get<RParamValue>("Q")->GetDouble();
	Weight=param->Get<RParamValue>("Weight")->GetDouble();
}


//-----------------------------------------------------------------------------
RParam* RPromCriterionParams::CreateParam(const R::RString& name)
{
	RParamStruct* param=new RParamStruct(name);
	param->Insert(new RParamValue("P",0.2));
	param->Insert(new RParamValue("Q",0.05));
	param->Insert(new RParamValue("Weight",1.0));
	return(param);
}



//------------------------------------------------------------------------------
//
// class RPromCriterion
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromCriterion::RPromCriterion(CriteriaType type,double p,double q,double w,unsigned int id,unsigned int nb)
	: RContainer<RPromCritValue,false,false>(nb,nb/2), Id(id),
	  Type(type), P(p), Q(q), Weight(w)
{
	if(Type==Minimize)
	{
		P=-p;
		Q=-q;
	}
	else
	{
		P=p;
		Q=q;
	}
}


//------------------------------------------------------------------------------
RPromCriterion::RPromCriterion(CriteriaType type,const RPromCriterionParams& params,unsigned int id,unsigned int nb)
	: RContainer<RPromCritValue,false,false>(nb,nb/2), Id(id),
	  Type(type), P(params.P), Q(params.Q), Weight(params.Weight)
{
	if(Type==Minimize)
	{
		P=-params.P;
		Q=-params.Q;
	}
	else
	{
		P=params.P;
		Q=params.Q;
	}
}


//------------------------------------------------------------------------------
RPromCriterion::RPromCriterion(CriteriaType type,double p,double q,double w,unsigned int id,const char* name,unsigned int nb)
	: RContainer<RPromCritValue,false,false>(nb,nb/2), Id(id), Name(name),
	  Type(type), P(p), Q(q), Weight(w)
{
	if(Type==Minimize)
	{
		P=-p;
		Q=-q;
	}
	else
	{
		P=p;
		Q=q;
	}
}


//------------------------------------------------------------------------------
RPromCriterion::RPromCriterion(CriteriaType type,const RPromCriterionParams& params,unsigned int id,const char* name,unsigned int nb)
	: RContainer<RPromCritValue,false,false>(nb,nb/2), Id(id), Name(name),
	  Type(type), Weight(params.Weight)
{
	if(Type==Minimize)
	{
		P=-params.P;
		Q=-params.Q;
	}
	else
	{
		P=params.P;
		Q=params.Q;
	}
}


//------------------------------------------------------------------------------
void RPromCriterion::SetParams(const RPromCriterionParams& params)
{
	if(Type==Minimize)
	{
		P=-params.P;
		Q=-params.Q;
	}
	else
	{
		P=params.P;
		Q=params.Q;
	}
	Weight=params.Weight;
}


//------------------------------------------------------------------------------
RPromCriterionParams RPromCriterion::GetParams(void)
{
	RPromCriterionParams tmp;

	if(Type==Minimize)
	{
		tmp.P=-P;
		tmp.Q=-Q;
	}
	else
	{
		tmp.P=P;
		tmp.Q=Q;
	}
	tmp.Weight=Weight;
	return(tmp);
}


//------------------------------------------------------------------------------
void RPromCriterion::Normalize(void)
{
	unsigned int i;
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
double RPromCriterion::ComputePref(double u,double v) const
{
	double d;
	double y;

	if(u==0.0) return(0.0);
	if(u!=v)
		d=(u-v)/u;
	else
		d=0.0;

	switch(Type)
	{
		case Maximize:
			if(P<Q) return(0.0);
			if((P<0.0)||(Q<0.0)) return(0.0);
			if(P==Q)
			{
				if(d<=Q) return(0.0); else return(1.0);
			}
			if(d<=Q)
				y=0.0;
			else
			{
				if(d>P)
					y=1.0;
				else
					y=(d-Q)/(P-Q);
			}
			break;

		case Minimize:
			if(P>Q) return(0.0);
			if((P>0.0)||(Q>0.0)) return(0.0);
			if(P==Q)
			{
				if(d>=Q) return(0.0); else return(1.0);
			}
			if(d>=Q)
				y=0.0;
			else
			{
				if(d<P)
					y=1.0;
				else
					y=(d-Q)/(P-Q);
			}
			break;

		default:
			y=0.0;
			break;
	}
	return(y);
}


//------------------------------------------------------------------------------
void RPromCriterion::ComputeFiCrit(RPromKernel* kern)
{
	// Init all value to 0
	RCursor<RPromCritValue> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
		ptr()->FiCritPlus=ptr()->FiCritMinus=0.0;

	// Calculation of Fi Crit + & -
	RCursor<RPromCritValue> ptr2(*this);
	RCursor<RPromSol> sol(kern->Solutions);
	RCursor<RPromSol> sol2(kern->Solutions);
	for(ptr.Start(),sol.Start();!ptr.End();ptr.Next(),sol.Next())
	{
		for(ptr2.Start(),sol2.Start();!ptr2.End();ptr2.Next(),sol2.Next())
		{
			// Only if secondary solution is not the same than the primary one.
			if(sol()==sol2()) continue;
			ptr()->FiCritPlus+=ComputePref(ptr()->Value,ptr2()->Value)/((double)(GetNb()-1));
			ptr()->FiCritMinus+=ComputePref(ptr2()->Value,ptr2()->Value)/((double)(GetNb()-1));
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
