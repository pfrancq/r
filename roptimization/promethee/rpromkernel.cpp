/*

	R Project Library

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromkernel.h>
#include <rpromethee/rpromcritvalue.h>
using namespace RPromethee;



//-----------------------------------------------------------------------------
//
// class RPromKernel
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPromethee::RPromKernel::RPromKernel(const RString& name,const unsigned int sol,const unsigned int crit,const bool norm)
	: Name(name), Solutions(sol,sol/2), Criteria(crit,crit/2), Normalize(norm)
{
}


//-----------------------------------------------------------------------------
void RPromethee::RPromKernel::ComputeEvalFunc(void)
{
}


//-----------------------------------------------------------------------------
void RPromethee::RPromKernel::ComputePrometheeII(void)
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
RPromCriterion* RPromethee::RPromKernel::NewCriterion(const CriteriaType t,const double p,const double q,const double w) throw(bad_alloc)
{
	RPromCriterion* crit=new RPromCriterion(t,p,q,w,Criteria.NbPtr,Solutions.MaxPtr);
	Criteria.InsertPtr(crit);
	return(crit);
}


//-----------------------------------------------------------------------------
RPromCriterion* RPromethee::RPromKernel::NewCriterion(const CriteriaType t,const RPromCriterionParams& params) throw(bad_alloc)
{
	RPromCriterion* crit=new RPromCriterion(t,params,Criteria.NbPtr,Solutions.MaxPtr);
	Criteria.InsertPtr(crit);
	return(crit);
}


//-----------------------------------------------------------------------------
RPromCriterion* RPromethee::RPromKernel::NewCriterion(const CriteriaType t,const RString& name,
		const double p,const double q,const double w) throw(bad_alloc)
{
	RPromCriterion* crit=new RPromCriterion(t,p,q,w,Criteria.NbPtr,name,Solutions.MaxPtr);
	Criteria.InsertPtr(crit);
	return(crit);
}


//-----------------------------------------------------------------------------
RPromCriterion* RPromethee::RPromKernel::NewCriterion(const CriteriaType t,const RString& name,
		const RPromCriterionParams& params) throw(bad_alloc)
{
	RPromCriterion* crit=new RPromCriterion(t,params,Criteria.NbPtr,name,Solutions.MaxPtr);
	Criteria.InsertPtr(crit);
	return(crit);
}


//-----------------------------------------------------------------------------
RPromSol* RPromethee::RPromKernel::NewSol(void) throw(bad_alloc)
{
	RPromSol* sol=new RPromSol(Solutions.NbPtr,Criteria.MaxPtr);
	Solutions.InsertPtr(sol);
	return(sol);
}


//-----------------------------------------------------------------------------
RPromSol* RPromethee::RPromKernel::NewSol(const RString& name) throw(bad_alloc)
{
	RPromSol* sol=new RPromSol(Solutions.NbPtr,name,Criteria.MaxPtr);
	Solutions.InsertPtr(sol);
	return(sol);
}


//-----------------------------------------------------------------------------
void RPromethee::RPromKernel::Assign(RPromSol *sol,RPromCriterion *crit,const double v) throw(bad_alloc)
{
	RPromCritValue* val=new RPromCritValue(v);
	sol->InsertPtrAt(val,crit->Id);
	crit->InsertPtrAt(val,sol->Id);
}


//-----------------------------------------------------------------------------
void RPromethee::RPromKernel::Assign(const RString& sol,RPromCriterion *crit,const double v) throw(bad_alloc)
{
	Assign(Solutions.GetPtr<RString>(sol),crit,v);
}


//-----------------------------------------------------------------------------
void RPromethee::RPromKernel::Assign(RPromSol *sol,const RString& crit,const double v) throw(bad_alloc)
{
	Assign(sol,Criteria.GetPtr<RString>(crit),v);
}


//-----------------------------------------------------------------------------
void RPromethee::RPromKernel::Assign(const RString& sol,const RString& crit,const double v) throw(bad_alloc)
{
	Assign(Solutions.GetPtr<RString>(sol),Criteria.GetPtr<RString>(crit),v);
}



//-----------------------------------------------------------------------------
RPromSol* RPromethee::RPromKernel::GetBestSol(void)
{
	RPromSol *best,**sol;
	unsigned int i;
	
	best=(*Solutions.Tab);
	for(i=Solutions.NbPtr,sol=&Solutions.Tab[1];--i;sol++)
		if(best->Fi>(*sol)->Fi)
			best=(*sol);
	return(best);
}


//-----------------------------------------------------------------------------
int RPromethee::RPromKernel::sort_function_solutions( const void *a, const void *b)
{
	RPromSol* as=(*(static_cast<RPromSol**>(a)));
	RPromSol* bs=(*(static_cast<RPromSol**>(b)));
	double d;
	
	d=bs->Fi-as->Fi;
	if(d<=0.000001) return(0);
	if(d<0)
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
RPromSol** RPromethee::RPromKernel::GetSols(void)
{
	RPromSol **Sols=new RPromSol*[Solutions.NbPtr];
	RPromSol **sol1,**sol2;
	unsigned int i;	

	for(i=Solutions.NbPtr+1,sol1=Sols,sol2=Solutions.Tab;--i;sol1++,sol2++)
		(*sol1)=(*sol2);
	qsort(static_cast<void*>(Sols),Solutions.NbPtr,sizeof(RPromSol*),sort_function_solutions);	
	return(Sols);
}


//-----------------------------------------------------------------------------
void RPromethee::RPromKernel::Clear(void)
{
	Criteria.Clear();
	Solutions.Clear();
}


//-----------------------------------------------------------------------------
RPromethee::RPromKernel::~RPromKernel(void)
{
}
