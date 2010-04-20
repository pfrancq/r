/*

	R Project Library

	RPromKernelSC.hh

	Similarity-based Clustering PROMETHEE Kernel - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
//
// class RPromKernelSC<cChromo>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cChromo>
	RPromKernelSC<cChromo>::RPromKernelSC(RParamsSC* p,size_t nbsols)
	: RPromKernel("RPromKernelSC",nbsols,3), Params(p),CritSimJ(0), CritAgreement(0),
	  CritDisagreement(0)
{
	// Init Criterion and Solutions of the PROMETHEE part
	AddCriterion(CritSimJ=new RPromLinearCriterion(RPromCriterion::Maximize,Params->ParamsSim,"J (Sim)"));
	AddCriterion(CritAgreement=new RPromLinearCriterion(RPromCriterion::Maximize,Params->ParamsAgreement,"Agreement"));
	AddCriterion(CritDisagreement=new RPromLinearCriterion(RPromCriterion::Minimize,Params->ParamsDisagreement,"Disagreement"));
}


//-----------------------------------------------------------------------------
template<class cChromo>
	void RPromKernelSC<cChromo>::AssignChromo(RPromSol* s,cChromo* c)
{
	Assign(s,CritSimJ,c->CritSimJ);
	Assign(s,CritAgreement,c->CritAgreement);
	Assign(s,CritDisagreement,c->CritDisagreement);
}


//-----------------------------------------------------------------------------
template<class cChromo>
	void RPromKernelSC<cChromo>::AssignSol(RPromSol* s,double sim,double agree,double disagree)
{
	Assign(s,CritSimJ,sim);
	Assign(s,CritAgreement,agree);
	Assign(s,CritDisagreement,disagree);
}
