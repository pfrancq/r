/*

	Rainbow Library Project

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
#include <rpromethee/rpromcriterion.h>
#include <rpromethee/rpromcritvalue.h>
#include <rpromethee/rpromkernel.h>
using namespace RPromethee;



//-----------------------------------------------------------------------------
//
// class RPromCriterion
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPromCriterion::RPromCriterion(const CriteriaType type,const double p,const double q,const double w,const unsigned int id,const unsigned int nb)
	: RContainer<RPromCritValue,unsigned int,false,false>(nb,nb/2), Id(id),
	  Type(type), P(p), Q(q), Weight(w)
{
}


//-----------------------------------------------------------------------------
void RPromCriterion::Normalize(void)
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
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
		(*ptr)->Value=((*ptr)->Value-min)/diff;
}


//-----------------------------------------------------------------------------
double RPromCriterion::ComputePref(const double u,const double v)
{
	double d=u-v;
	double y;

	switch(Type)
	{
		case Maximize:
			if(P<Q) return(0.0);
			if((P<0.0)||(Q<0.)) return(0.0);
			if(P==Q)
			{
				if(d<=P) return(0.0); else return(1.0);
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
			if(Q<P) return(0.0);
			if((P>0.0)||(Q>0.0)) return(0.0);
			if(P==Q)
			{
				if(d>=P) return(0.0); else return(1.0);
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
void RPromCriterion::ComputeFiCrit(RPromKernel *kern)
{
	RPromCritValue **ptr,**ptr1;
	RPromSol **sol,**sol1;
	unsigned int a,b;

	// Init all value to 0
	for(a=NbPtr+1,ptr=Tab;--a;ptr++)
		(*ptr)->FiCrit=0.0;
		
	// Calculation of Fi Crit		
	for(a=NbPtr+1,ptr=Tab,sol=kern->Solutions.Tab;--a;ptr++,sol++)
	{
		for(b=NbPtr+1,ptr1=Tab,sol1=kern->Solutions.Tab;--b;ptr1++,sol1++)
		{
			// Only if secondary solution is not the same than the primary one.
			if((*sol)!=(*sol1))
				(*ptr)->FiCrit+=(ComputePref((*ptr)->Value,(*ptr1)->Value)-ComputePref((*ptr1)->Value,(*ptr)->Value))/(NbPtr-1);
		}
	}
}


//-----------------------------------------------------------------------------
RPromCriterion::~RPromCriterion(void)
{
}
