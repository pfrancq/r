/*

	R Project Library

	RPromKernel.cpp

	Promethee Kernel - Implementation.

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
#include <stdlib.h>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromcritvalue.h>
#include <rcursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RPromKernel
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromKernel::RPromKernel(const char* name,size_t sol,size_t crit)
	: Name(name), Solutions(sol,sol/2), Criteria(crit,crit/2)
{
}


//------------------------------------------------------------------------------
void RPromKernel::ComputeEvalFunc(void)
{
}


//------------------------------------------------------------------------------
void RPromKernel::ComputePrometheeII(void)
{
	double SumWTot = 0.0;

	// Calculation of Fit Crit fot the criteria.
	RCursor<RPromCriterion> crit(Criteria);
	for(crit.Start();!crit.End();crit.Next())
	{
		crit()->Normalize();
		SumWTot+=crit()->Weight;
		crit()->ComputeFiCrit(this);
	}

	// Calculation of the flux.
	RCursor<RPromSol> sol(Solutions);
	for(sol.Start();!sol.End();sol.Next())
	{
		sol()->FiPlus=sol()->FiMinus=0.0;		
		RCursor<RPromCritValue> value(*sol());
		for(crit.Start(),value.Start();!value.End();crit.Next(),value.Next())
		{
			sol()->FiPlus+=crit()->Weight*value()->FiCritPlus;
			sol()->FiMinus+=crit()->Weight*value()->FiCritMinus;
		}
		sol()->FiPlus/=SumWTot*((double)sol.GetNb()-1);
		sol()->FiMinus/=SumWTot*((double)sol.GetNb()-1);
		sol()->Fi=sol()->FiPlus-sol()->FiMinus;
	}
}


//------------------------------------------------------------------------------
void RPromKernel::AddCriterion(RPromCriterion* crit)
{
	crit->SetId(Criteria.GetNb());
	Criteria.InsertPtr(crit);
}


//------------------------------------------------------------------------------
RPromSol* RPromKernel::NewSol(void)
{
	RPromSol* sol=new RPromSol(Solutions.GetNb(),Criteria.GetMaxNb());
	Solutions.InsertPtr(sol);
	return(sol);
}


//------------------------------------------------------------------------------
RPromSol* RPromKernel::NewSol(const char* name)
{
	RPromSol* sol=new RPromSol(Solutions.GetNb(),name,Criteria.GetMaxNb());
	Solutions.InsertPtr(sol);
	return(sol);
}


//------------------------------------------------------------------------------
void RPromKernel::Assign(RPromSol* sol,RPromCriterion* crit,const double v)
{
	RPromCritValue* val;

	if((!sol)||(!crit)) return;
	if((!sol->GetNb())||(sol->GetMaxPos()<crit->Id)||(!(val=(*sol)[crit->Id])))
	{
		val=new RPromCritValue(v);
		sol->InsertPtrAt(val,crit->Id);
		crit->InsertPtrAt(val,sol->Id);
	}
	else
	{
		val->Value=v;
		val=(*crit)[sol->Id];
		val->Value=v;
	}
}


//------------------------------------------------------------------------------
void RPromKernel::Assign(const char* sol,RPromCriterion* crit,const double v)
{
	Assign(Solutions.GetPtr<RString>(sol),crit,v);
}


//------------------------------------------------------------------------------
void RPromKernel::Assign(RPromSol* sol,const char* crit,const double v)
{
	Assign(sol,Criteria.GetPtr<RString>(crit),v);
}


//------------------------------------------------------------------------------
void RPromKernel::Assign(const char* sol,const char* crit,const double v)
{
	Assign(Solutions.GetPtr<RString>(sol),Criteria.GetPtr<RString>(crit),v);
}


//------------------------------------------------------------------------------
RPromSol* RPromKernel::GetBestSol(void)
{
	RPromSol *best;
	RCursor<RPromSol> sol(Solutions);
	sol.Start();
	best=sol();
	for(sol.Next();!sol.End();sol.Next())
		if(best->Fi<sol()->Fi)
			best=sol();
	return(best);
}


//------------------------------------------------------------------------------
int RPromKernel::sort_function_solutions(const void *a,const void *b)
{
	const RPromSol* as=(*((RPromSol**)(a)));
	const RPromSol* bs=(*((RPromSol**)(b)));
	double d;

	d=bs->Fi-as->Fi;
	if(d==0.0) return(0);
	if(d<0.0)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
RPromSol** RPromKernel::GetSols(void)
{
	RPromSol** Sols=new RPromSol*[Solutions.GetMaxPos()+1];
	Solutions.GetTab(Sols);
	qsort(static_cast<void*>(Sols),Solutions.GetNb(),sizeof(RPromSol*),sort_function_solutions);
	return(Sols);
}


//------------------------------------------------------------------------------
void RPromKernel::GetSols(RPromSol** sols)
{
	Solutions.GetTab(sols);
	qsort(static_cast<void*>(sols),Solutions.GetNb(),sizeof(RPromSol*),sort_function_solutions);
}


//------------------------------------------------------------------------------
void RPromKernel::Clear(void)
{
	Criteria.Clear();
	Solutions.Clear();
}


//------------------------------------------------------------------------------
void RPromKernel::ClearSols(void)
{
	RCursor<RPromCriterion> Cur(Criteria);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Solutions.Clear();
}


//------------------------------------------------------------------------------
RPromKernel::~RPromKernel(void)
{
}
