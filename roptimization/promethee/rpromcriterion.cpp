/*

	R Project Library

	RPromCriterion.cpp

	Promethee Criterion - Implementation.

	(C) 2000-2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromcriterion.h>
#include <rpromethee/rpromcritvalue.h>
#include <rpromethee/rpromkernel.h>
using namespace RPromethee;



//-----------------------------------------------------------------------------
//
// class RPromCriterionParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPromethee::RPromCriterionParams::RPromCriterionParams(void)
	: P(0.2), Q(0.05), Weight(1.0)
{
}


//-----------------------------------------------------------------------------
RPromethee::RPromCriterionParams::RPromCriterionParams(double p,double q,double w)
	: P(p), Q(q), Weight(w)
{
}


//-----------------------------------------------------------------------------
RPromCriterionParams* RPromethee::RPromCriterionParams::GetParams(void)
{
	return(GetTemporaryObject<RPromCriterionParams,30>());
}


//-----------------------------------------------------------------------------
void RPromethee::RPromCriterionParams::Set(const char* values)
{
	sscanf(values,"%lf %lf %lf",&P,&Q,&Weight);
}


//-----------------------------------------------------------------------------
RPromCriterionParams& RPromethee::RPromCriterionParams::operator=(const RPromCriterionParams &params)
{
	P=params.P;
	Q=params.Q;
	Weight=params.Weight;
	return(*this);
}



//-----------------------------------------------------------------------------
//
// class RPromCriterion
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPromethee::RPromCriterion::RPromCriterion(CriteriaType type,double p,double q,double w,unsigned int id,unsigned int nb)
	: RContainer<RPromCritValue,unsigned int,false,false>(nb,nb/2), Id(id),
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


//-----------------------------------------------------------------------------
RPromethee::RPromCriterion::RPromCriterion(CriteriaType type,const RPromCriterionParams& params,unsigned int id,unsigned int nb)
	: RContainer<RPromCritValue,unsigned int,false,false>(nb,nb/2), Id(id),
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


//-----------------------------------------------------------------------------
RPromethee::RPromCriterion::RPromCriterion(CriteriaType type,double p,double q,double w,unsigned int id,const char* name,unsigned int nb)
	: RContainer<RPromCritValue,unsigned int,false,false>(nb,nb/2), Id(id), Name(name),
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


//-----------------------------------------------------------------------------
RPromethee::RPromCriterion::RPromCriterion(CriteriaType type,const RPromCriterionParams& params,unsigned int id,const char* name,unsigned int nb)
	: RContainer<RPromCritValue,unsigned int,false,false>(nb,nb/2), Id(id), Name(name),
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


//-----------------------------------------------------------------------------
void RPromethee::RPromCriterion::SetParams(const RPromCriterionParams& params)
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


//-----------------------------------------------------------------------------
RPromCriterionParams& RPromethee::RPromCriterion::GetParams(void)
{
	RPromCriterionParams* tmp=RPromCriterionParams::GetParams();

	if(Type==Minimize)
	{
		tmp->P=-P;
		tmp->Q=-Q;
	}
	else
	{
		tmp->P=P;
		tmp->Q=Q;
	}
	tmp->Weight=Weight;
	return(*tmp);
}


//-----------------------------------------------------------------------------
void RPromethee::RPromCriterion::Normalize(void)
{
	unsigned int i;
	RPromCritValue **ptr;
	double max,min,diff;

	// Compute maximal value
	ptr=Tab;
	min=max=(*(ptr++))->Value;
	for(i=NbPtr;--i;ptr++)
	{
		if(max<(*ptr)->Value)
			max=(*ptr)->Value;
		if(min>(*ptr)->Value)
			min=(*ptr)->Value;
	}
	diff=max-min;

	// Normalize
	if(diff)
	{
		for(i=NbPtr+1,ptr=Tab;--i;ptr++)
			(*ptr)->Value=((*ptr)->Value-min)/diff;
	}
}


//-----------------------------------------------------------------------------
double RPromethee::RPromCriterion::ComputePref(double u,double v) const
{
	double d;
	double y;

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


//-----------------------------------------------------------------------------
void RPromethee::RPromCriterion::ComputeFiCrit(RPromKernel *kern)
{
	RPromCritValue **ptr,**ptr1;
	RPromSol **sol,**sol1;
	unsigned int a,b;

	// Init all value to 0
	for(a=NbPtr+1,ptr=Tab;--a;ptr++)
		(*ptr)->FiCritPlus=(*ptr)->FiCritMinus=0.0;

	// Calculation of Fi Crit + & -
	for(a=NbPtr+1,ptr=Tab,sol=kern->Solutions.Tab;--a;ptr++,sol++)
	{
		for(b=NbPtr+1,ptr1=Tab,sol1=kern->Solutions.Tab;--b;ptr1++,sol1++)
		{
			// Only if secondary solution is not the same than the primary one.
			if((*sol)==(*sol1)) continue;
			(*ptr)->FiCritPlus+=ComputePref((*ptr)->Value,(*ptr1)->Value)/((double)(NbPtr-1));
			(*ptr)->FiCritMinus+=ComputePref((*ptr1)->Value,(*ptr)->Value)/((double)(NbPtr-1));
		}
	}

	// Compute Fi Crit
	for(a=NbPtr+1,ptr=Tab;--a;ptr++)
		(*ptr)->FiCrit=(*ptr)->FiCritPlus-(*ptr)->FiCritMinus;
}


//-----------------------------------------------------------------------------
RPromethee::RPromCriterion::~RPromCriterion(void)
{
}
