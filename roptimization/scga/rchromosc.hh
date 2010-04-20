/*

	R Project Library

	RChromoSC.cpp

	Similarity-based Clustering Chromosome - Implementation

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
// class RChromoSC
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::RChromoSC(cInst* inst,size_t id)
		: RChromoG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>(inst,id),
	  ToDel(0), CritSimJ(0.0), CritAgreement(0.0), CritDisagreement(1.0), Protos(Used.GetMaxNb()),
	  OldProtos(Used.GetMaxNb()),
	  thProm(0), thSols(0), MostSimilarGroup1(cNoRef), MostSimilarGroup2(cNoRef), VerifyCentroids(true)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::Init(cThreadData* thData)
{
	// Parent Initialization
	RChromoG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Init(thData);

	// Current
	thObjs1=thData->tmpObjs1;
	thObjs2=thData->tmpObjs2;
	thTests=thData->Tests;
	thProm=&thData->Prom;
	thSols=thData->Sols;
	ToDel=&thData->ToDel;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::Evaluate(void)
{
//	double super(-2.0);

	/*AvgInterSim=*/CritAgreement=CritDisagreement=CritSimJ=0.0;
	MostSimilarGroup1=MostSimilarGroup2=cNoRef;
	if(!Used.GetNb())
		return;

	double AvgIntraSim(0.0), AvgInterSim(0.0), AvgIntraAgree(0.0), AvgInterAgree(0.0), AvgIntraDisagree(0.0), AvgInterDisagree(0.0);
	size_t NbIntraSim(0), NbInterSim(0), NbIntraAgree(0), NbInterAgree(0), NbIntraDisagree(0), NbInterDisagree(0);

	// Go trough each object
	for(Objs.Start();!Objs.End();Objs.Next())
	{
		size_t GroupId(ObjectsAss[Objs()->GetId()]);

		// Treat the similarities
		RCursor<RMaxValue> Sim(*Instance->GetSims(Objs()));
		for(Sim.Start();!Sim.End();Sim.Next())
		{
			if(ObjectsAss[Sim()->Id]==GroupId)
			{
				AvgIntraSim+=Sim()->Value;
				NbIntraSim++;
			}
			else
			{
				AvgInterSim+=Sim()->Value;
				NbInterSim++;
			}
		}

		// Treat the agreement ratios
		RCursor<RMaxValue> Agree(*Instance->GetAgreementRatios(Objs()));
		for(Agree.Start();!Agree.End();Agree.Next())
		{
			if(ObjectsAss[Agree()->Id]==GroupId)
			{
				AvgIntraAgree+=Agree()->Value;
				NbIntraAgree++;
			}
			else
			{
				AvgInterAgree+=Agree()->Value;
				NbInterAgree++;
			}
		}

		// Treat the disagreement ratios
		RCursor<RMaxValue> Disagree(*Instance->GetDisagreementRatios(Objs()));
		for(Disagree.Start();!Disagree.End();Disagree.Next())
		{
			if(ObjectsAss[Disagree()->Id]==GroupId)
			{
				AvgIntraDisagree+=Disagree()->Value;
				NbIntraDisagree++;
			}
			else
			{
				AvgInterDisagree+=Disagree()->Value;
				NbInterDisagree++;
			}
		}
	}

	// Compute the similarity criterion
	if(NbIntraSim)
	{
		// At least two objects with a non-null similarity are grouped together
		AvgIntraSim/=NbIntraSim;
		if(NbInterSim)
		{
			// At least two objects with a non-null similarity are not grouped together
			AvgInterSim/=NbInterSim;
			CritSimJ=AvgIntraSim/AvgInterSim;
		}
		else
			CritSimJ=AvgIntraSim;  // The objects have only a non-null similarity with the objects of the same group
	}
	else
	{
		// No object has a non-null similarity with another object of the same group
		CritSimJ=0.0;
	}

	// Compute the agreement criterion
	if(NbIntraAgree)
	{
		// At least two objects with a non-null agreement ratio are grouped together
		AvgIntraAgree/=NbIntraAgree;
		if(NbInterAgree)
		{
			// At least two objects with a non-null agreement ratio are not grouped together
			AvgInterAgree/=NbInterAgree;
			CritAgreement=AvgIntraAgree/AvgInterAgree;
		}
		else
			CritAgreement=AvgIntraAgree;  // The objects have only a non-null agreement ratio with the objects of the same group
	}
	else
	{
		// No object has a non-null agreement ratio with another object of the same group
		CritAgreement=0.0;
	}

	// Compute the disagreement criterion
	if(NbIntraDisagree)
	{
		// At least two objects with a non-null disagreement ratio are grouped together
		AvgIntraDisagree/=NbIntraDisagree;
		if(NbInterDisagree)
		{
			// At least two objects with a non-null disagreement ratio are not grouped together
			AvgInterDisagree/=NbInterDisagree;
			CritDisagreement=AvgIntraDisagree/AvgInterDisagree;
		}
		else
			CritDisagreement=AvgIntraDisagree;  // The objects have only a non-null disagreement ratio with the objects of the same group
	}
	else
	{
		// No object has a non-null disagreement ratio with another object of the same group
		CritDisagreement=0.0;
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	bool RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::SameGroup(size_t obj1,size_t obj2) const
{
	return(ObjectsAss[obj1]==ObjectsAss[obj2]);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::Copy(const cChromo& chromo)
{
	RChromoG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Copy(chromo);
	CritSimJ=chromo.CritSimJ;
	CritAgreement=chromo.CritAgreement;
	CritDisagreement=chromo.CritDisagreement;
	Fi=chromo.Fi;
	FiPlus=chromo.FiPlus;
	FiMinus=chromo.FiMinus;
	MostSimilarGroup1=chromo.MostSimilarGroup1;
	MostSimilarGroup2=chromo.MostSimilarGroup2;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::~RChromoSC(void)
{
}
