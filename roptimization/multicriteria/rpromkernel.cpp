/*

	R Project Library

	RPromKernel.cpp

	PROMETHEE Kernel - Implementation.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
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
#include <stdlib.h>
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromcritvalue.h>
#include <rcursor.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// class RPromKernel
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPromKernel::RPromKernel(const char* name,size_t sol,size_t crit)
	: Name(name), Solutions(sol,sol/2), OrderedSolutions(sol,sol/2), Criteria(crit,crit/2), MustReOrder(false)
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

	// Calculation of Fit Crit for the criteria.
	RCursor<RPromCriterion> crit(Criteria);
	for(crit.Start();!crit.End();crit.Next())
	{
		crit()->Normalize();
		SumWTot+=crit()->Weight;
		crit()->ComputeFiCrit(this);
	}

	// Calculation of the flow.
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

	MustReOrder=true;
}


//------------------------------------------------------------------------------
void RPromKernel::AddCriterion(RPromCriterion* crit)
{
	crit->SetId(Criteria.GetNb());
	Criteria.InsertPtr(crit);
}


//------------------------------------------------------------------------------
void RPromKernel::AddSol(RPromSol* sol)
{
	Solutions.InsertPtr(sol);
}


//------------------------------------------------------------------------------
RPromSol* RPromKernel::NewSol(void)
{
	RPromSol* sol=new RPromSol(Solutions.GetNb(),Criteria.GetMaxNb());
	Solutions.InsertPtr(sol);
	return(sol);
}


//------------------------------------------------------------------------------
RPromSol* RPromKernel::NewSol(const RString& name)
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
		val=(*crit)[sol->Id];
		val->Value=v;
	}
}


//------------------------------------------------------------------------------
void RPromKernel::Assign(const RString& sol,RPromCriterion* crit,const double v)
{
	Assign(Solutions.GetPtr<RString>(sol),crit,v);
}


//------------------------------------------------------------------------------
void RPromKernel::Assign(RPromSol* sol,const RString& crit,const double v)
{
	Assign(sol,Criteria.GetPtr<RString>(crit),v);
}


//------------------------------------------------------------------------------
void RPromKernel::Assign(const RString& sol,const RString& crit,const double v)
{
	Assign(Solutions.GetPtr<RString>(sol),Criteria.GetPtr<RString>(crit),v);
}


//------------------------------------------------------------------------------
int RPromKernel::sort_function_solutions(const void *a,const void *b)
{
	double as((*((RPromSol**)(a)))->Fi);
	double bs((*((RPromSol**)(b)))->Fi);

	if(as>bs)
		return(-1);
	else if(as<bs)
		return(1);
	return(0);
}


//------------------------------------------------------------------------------
void RPromKernel::OrderSolutions(void)
{
	OrderedSolutions.VerifyTab(Solutions.GetNb());
	RCursor<RPromSol> Sol(Solutions);
	for(Sol.Start();!Sol.End();Sol.Next())
		OrderedSolutions.InsertPtr(Sol());
	OrderedSolutions.ReOrder(sort_function_solutions);
	MustReOrder=false;
}


//------------------------------------------------------------------------------
const RPromSol* RPromKernel::GetBestSol(void)
{
	if(!Solutions.GetNb())
		return(0);
	if(MustReOrder)
		OrderSolutions();
	return(OrderedSolutions[0]);
}


//------------------------------------------------------------------------------
RCursor<RPromSol> RPromKernel::GetSols(void)
{
	if(MustReOrder)
		OrderSolutions();
	return(RCursor<RPromSol>(OrderedSolutions));
}


//------------------------------------------------------------------------------
void RPromKernel::CopySols(RContainer<RPromSol,false,false>& sols)
{
	if(MustReOrder)
		OrderSolutions();
	sols=OrderedSolutions;
}


//------------------------------------------------------------------------------
double RPromKernel::GetMinFi(void)
{
	if(!Solutions.GetNb())
		return(0.0);
	if(MustReOrder)
		OrderSolutions();
	return(OrderedSolutions[OrderedSolutions.GetMaxPos()]->Fi);
}


//------------------------------------------------------------------------------
double RPromKernel::GetMaxFi(void)
{
	if(!Solutions.GetNb())
		return(0.0);
	if(MustReOrder)
		OrderSolutions();
	return(OrderedSolutions[0]->Fi);
}


//------------------------------------------------------------------------------
void RPromKernel::Clear(void)
{
	Criteria.Clear();
	Solutions.Clear();
	OrderedSolutions.Clear();
}


//------------------------------------------------------------------------------
void RPromKernel::ClearSols(void)
{
	RCursor<RPromCriterion> Cur(Criteria);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Solutions.Clear();
	OrderedSolutions.Clear();
}


//------------------------------------------------------------------------------
void RPromKernel::Print(bool normalized)
{
	if(!Solutions.GetNb())
		return;
	if(MustReOrder)
		OrderSolutions();

	// Print Header
	RString Str("Id");
	Str.SetLen(15," ");
	cout<<Str;
	Str="Fi";
	Str.SetLen(15," ");
	cout<<Str;
	Str="Fi+";
	Str.SetLen(15," ");
	cout<<Str;
	Str="Fi-";
	Str.SetLen(15," ");
	cout<<Str;
	RCursor<RPromCriterion> Crit(Criteria);
	for(Crit.Start();!Crit.End();Crit.Next())
	{
		Str=Crit()->Name;
		Str.SetLen(20," ");
		cout<<Str;
	}
	cout<<"Name"<<endl;

	// Print Solution
	RCursor<RPromSol> Sol(OrderedSolutions);
	for(Sol.Start();!Sol.End();Sol.Next())
	{
		Str=RString::Number(Sol()->Id);
		Str.SetLen(15," ");
		cout<<Str;
		Str=RString::Number(Sol()->Fi,"%E");
		Str.SetLen(15," ");
		cout<<Str;
		Str=RString::Number(Sol()->FiPlus,"%E");
		Str.SetLen(15," ");
		cout<<Str;
		Str=RString::Number(Sol()->FiMinus,"%E");
		Str.SetLen(15," ");
		cout<<Str;
		RCursor<RPromCritValue> Value(*Sol());
		for(Value.Start();!Value.End();Value.Next())
		{
			if(normalized)
				Str=RString::Number(Value()->Normalized,"%E");
			else
				Str=RString::Number(Value()->Value,"%E");
			Str.SetLen(20," ");
			cout<<Str;
		}
		cout<<Sol()->Name<<endl;
	}
}


//------------------------------------------------------------------------------
RPromKernel::~RPromKernel(void)
{
}
