/*

	Rainbow Library Project

	RPromKernel.cpp

	Promethee Kernel - Implementation.

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
#include <rpromethee/rpromkernel.h>
#include <rpromethee/rpromcritvalue.h>
using namespace RPromethee;



//-----------------------------------------------------------------------------
//
// class RPromKernel
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPromKernel::RPromKernel(const RString& name,const unsigned int sol,const unsigned int crit,const bool norm)
	: Name(name), Solutions(sol,sol/2), Criteria(crit,crit/2), Normalize(norm)
{
}


//-----------------------------------------------------------------------------
void RPromKernel::ComputeEvalFunc(void)
{
}


//-----------------------------------------------------------------------------
void RPromKernel::ComputePrometheeII(void)
{
	RPromCriterion **crit;
	RPromSol **sol;
	RPromCritValue **value;	
	unsigned int i,j;	
	double SumWTot = 0.0;

	// Calculation of Fit Crit fot the criteria.
	for(i=Criteria.NbPtr+1,crit=Criteria.Tab;--i;crit++)
	{
		if(Normalize)
			(*crit)->Normalize();
		SumWTot+=(*crit)->Weight;
		(*crit)->ComputeFiCrit(this);
	}

	// Calculation of the flux.
	for(i=Solutions.NbPtr+1,sol=Solutions.Tab;--i;sol++)
	{
		(*sol)->Fi=0.0;
		for(j=(*sol)->NbPtr+1,value=(*sol)->Tab,crit=Criteria.Tab;--j;value++,crit++)
			(*sol)->Fi+=(((*crit)->Weight)*((*value)->FiCrit))/SumWTot;
	}
}


//-----------------------------------------------------------------------------
RPromCriterion* RPromKernel::NewCriterion(const CriteriaType t,const double p,const double q,const double w) throw(bad_alloc)
{
	RPromCriterion* crit=new RPromCriterion(t,p,q,w,Criteria.NbPtr,Solutions.MaxPtr);
	Criteria.InsertPtr(crit);
	return(crit);
}


//-----------------------------------------------------------------------------
RPromSol* RPromKernel::NewSol(void) throw(bad_alloc)
{
	RPromSol* sol=new RPromSol(Solutions.NbPtr,Criteria.MaxPtr);
	Solutions.InsertPtr(sol);
	return(sol);
}


//-----------------------------------------------------------------------------
void RPromKernel::Assign(RPromSol *sol,RPromCriterion *crit,const double v) throw(bad_alloc)
{
	RPromCritValue* val=new RPromCritValue(v);
	sol->InsertPtrAt(val,crit->Id);
	crit->InsertPtrAt(val,sol->Id);
}


//-----------------------------------------------------------------------------
RPromSol* RPromKernel::GetBestSol(void)
{
	RPromSol *best,**sol;
	unsigned int i;
	
	best=(*Solutions.Tab);
	for(i=Solutions.NbPtr,sol=&Solutions.Tab[1];--i;sol++)
		if(best->Fi<(*sol)->Fi)
			best=(*sol);
	return(best);
}


//-----------------------------------------------------------------------------
void RPromKernel::Clear(void)
{
	Criteria.Clear();
	Solutions.Clear();
}


//-----------------------------------------------------------------------------
RPromKernel::~RPromKernel(void)
{
}
