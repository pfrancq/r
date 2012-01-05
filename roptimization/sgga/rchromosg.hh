/*

	R Project Library

	RChromoSG.cpp

	Similarity-based Grouping Genetic Algorithm Chromosome - Implementation

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
// class RChromoSG
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::RChromoSG(cInst* inst,size_t id)
		: RChromoG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>(inst,id),
	  ToDel(0), CritSimJ(0.0), CritAgreement(0.0), CritDisagreement(1.0), Protos(Used.GetMaxNb()),
	  OldProtos(Used.GetMaxNb()),
	  thProm(0), thSols(0), MostSimilarGroup1(cNoRef), MostSimilarGroup2(cNoRef), VerifyCentroids(true)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::Init(cThreadData* thData)
{
	// Parent Initialization
	RChromoG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Init(thData);

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
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::Evaluate(void)
{
	double super(-2.0), max(-2.0),tmp;

	CritAgreement=CritDisagreement=CritSimJ=0.0;
	MostSimilarGroup1=MostSimilarGroup2=cNoRef;
	if(!Used.GetNb())
		return;

	// Go through the groups to compute each contribution to the average
	// measures and to find the similarity between the centroids and the meta-centroid.
	R::RCursor<cGroup> Cur(Used);
	R::RCursor<cGroup> Cur2(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		double avg(0.0);
		Cur()->Evaluate(CritSimJ,CritAgreement,CritDisagreement);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			if(Cur()==Cur2()) continue;
			tmp=Instance->GetSim(Cur()->GetCentroid(),Cur2()->GetCentroid());
			avg+=tmp;
			if(tmp>super)
			{
				super=tmp;
				MostSimilarGroup1=Cur()->GetId();
				MostSimilarGroup2=Cur2()->GetId();
			}
		}
		avg/=static_cast<double>(Used.GetNb()-1);
		if(avg>max)
			max=avg;
	}

	CritAgreement/=static_cast<double>(Used.GetNb());
	CritDisagreement/=static_cast<double>(Used.GetNb());
	CritSimJ/=(static_cast<double>(Used.GetNb())*max);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	double RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::ReAllocate(void)
{
	size_t nb;
	double sim,maxsim;
	cGroup* grp;
	cObj** Cur;

	// Put the prototypes in Protos
	Protos.Clear();
	R::RCursor<cGroup> Grp(Used);
	for(Grp.Start();!Grp.End();Grp.Next())
		Protos.InsertPtr(Grp()->GetCentroid());

	// Clear the chromosome
	Clear();

	// Insert the Prototypes in a group
	RCursor<cObj> CurP(Protos);
	for(CurP.Start();!CurP.End();CurP.Next())
	{
		grp=ReserveGroup();
		grp->Insert(CurP());
		grp->SetCentroid(CurP());
	}

	// Suspend the auto-computation of the centroids
	VerifyCentroids=false;

	// Go through the randomly ordered objects and put them in the group of the
	// most similar prototype.
	for(Cur=thObjs1,nb=Objs.GetNb()+1;--nb;Cur++)
	{
		// If the object is a prototype -> already in a group
		if(GetGroup(*Cur)) continue;

		// Look first if one of the object with a ratio are already grouped
		Instance->Params->FindBestGroup((*Cur)->GetId(),static_cast<cChromo*>(this),grp);

		// If no group find, -> Go through each group
		if(!grp)
		{
			Grp.Set(Used);
			for(Grp.Start(),maxsim=-2.0;!Grp.End();Grp.Next())
			{
				// If all the hard constraints are not respected -> skip the group.
				if(!Grp()->CanInsert(*Cur))
					continue;

				// Compute similarity with the relevant profile of the group.
				sim=Grp()->ComputeRelSim(*Cur);
				if(sim>maxsim)
				{
					maxsim=sim;
					grp=Grp();
				}
			}
		}

		// Insert the object in the current group.
		if(grp)
			grp->Insert(*Cur);
		else
		{
			// If no group find -> Create a new group and make the current object the
			// prototype of it.
			grp=ReserveGroup();
			grp->Insert(*Cur);
			grp->SetCentroid(*Cur);
		}
	}

	// Compute the average similarity of the groups
	VerifyCentroids=true;
	Grp.Set(Used);
	for(Grp.Start(),maxsim=0.0;!Grp.End();Grp.Next())
	{
		Grp()->SetCentroid(0); // Force to recompute the new centroids
		maxsim+=Grp()->GetAvgIntraSim();
	}
	return(maxsim/static_cast<double>(Used.GetNb()));
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	size_t RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::CalcNewProtosNb(void)
{
	size_t count;

	// Computed the prototypes for each groups and count the number in Protos
	R::RCursor<cGroup> Grp(Used);
	R::RCursor<cObj> OldProto(OldProtos);
	for(Grp.Start(),OldProto.Start(),count=0;!Grp.End();Grp.Next(),OldProto.Next())
	{
//		Grp()->ComputeRelevant();
		if(OldProto()!=Grp()->GetCentroid())
			count++;
	}
	return(count);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::DoKMeans(void)
{
	size_t itermax;
	double error,minerror;
	cObj** obj;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("DoKMeans","RChromoSG");

	// Copy and mix the objects into thObjs1
	for(Objs.Start(),obj=thObjs1;!Objs.End();Objs.Next(),obj++)
		(*obj)=Objs();
	Instance->RandOrder(thObjs1,Objs.GetNb());

	// Max Iterations
	minerror=Instance->Params->Convergence/100.0;
	for(itermax=0,error=1.0;(itermax<Instance->Params->MaxKMeans)&&(error>minerror);itermax++)
	{
		// Get the old prototypes
		OldProtos.Clear();
		R::RCursor<cGroup> Grp(Used);
		for(Grp.Start();!Grp.End();Grp.Next())
			OldProtos.InsertPtr(Grp()->GetCentroid());
		ReAllocate();
		error=static_cast<double>(CalcNewProtosNb())/static_cast<double>(Used.GetNb());
	}

	if(Instance->Debug)
	{
		Instance->Debug->PrintInfo("Number of k-Means runs: "+RString::Number(itermax));
		Instance->Debug->EndFunc("DoKMeans","RChromoSG");
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::DivideWorstObjects(void)
{
	R::RCursor<cGroup> Grp;
	size_t i;
	cObj** ptr;
	cObj** ptr2;
	cObj** ptr3;
	double sim,minsim;
	cGroup* grp;
	cObj* worst1(0);
	cObj* worst2(0);

	// Find the group containing the two most dissimilar objects
	Grp.Set(Used);
	for(Grp.Start(),minsim=1.0,grp=0;!Grp.End();Grp.Next())
	{
		if(Grp()->GetNbObjs()<3) continue;
		RCursor<cObj> CurObj(GetObjs(*Grp()));
		RCursor<cObj> CurObj2(GetObjs(*Grp()));
		for(CurObj.Start(),i=0;i<Grp()->GetNbObjs()-1;CurObj.Next(),i++)
		{
			for(CurObj2.GoTo(i+1);!CurObj2.End();CurObj2.Next())
			{
				sim=Instance->GetSim(CurObj(),CurObj2());
				if(sim<minsim)
				{
					minsim=sim;
					grp=Grp();
					worst1=CurObj();
					worst2=CurObj2();
				}
			}
		}
	}

	// Put in worst1 in thObj1 and worst2 in thObjs2
	if(!grp) return;
	NbObjs1=NbObjs2=1;
	ptr2=thObjs1;
	ptr3=thObjs2;
	(*(ptr2++))=worst1;
	(*(ptr3++))=worst2;

	// Copy in thObjs1 the objects most similar to worst1 and in thObjs2
	// the objects most similar to worst2
	RCursor<cObj> CurObj(GetObjs(*grp));
	for(CurObj.Start();!CurObj.End();CurObj.Next())
	{
		if((CurObj()==worst1)||(CurObj()==worst2)) continue;
		sim=Instance->GetSim(worst1,CurObj());
		minsim=Instance->GetSim(worst2,CurObj());
		if(sim>minsim)
		{
			NbObjs1++;
			(*(ptr2++))=CurObj();
		}
		else
		{
			NbObjs2++;
			(*(ptr3++))=CurObj();
		}
	}

	// Release grp
	ReleaseGroup(grp);

	// Reserve a new group and insert the objects of thObjs1
	grp=ReserveGroup();
	for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
		grp->Insert(*ptr);

	// Reserve a new group and insert the objects of thObjs2
	grp=ReserveGroup();
	for(ptr=thObjs2,i=NbObjs2+1;--i;ptr++)
		grp->Insert(*ptr);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::MergeBestGroups(void)
{
	// There must be at least 2 groups
	if(Used.GetNb()<2)
		return;

	// Verify that two groups can be merge
	if((MostSimilarGroup1==cNoRef)||(MostSimilarGroup2==cNoRef))
	{
		std::cerr<<"'MostSimilarGroup1' and/or 'MostSimilarGroup2' not correctly assigned"<<std::endl;
		return;
	}

	cGroup* bestgrp1((*this)[MostSimilarGroup1]);
	cGroup* bestgrp2((*this)[MostSimilarGroup2]);
	cObj** ptr;

	// Put the objects of bestgrp1 in bestgrp2 only if there are not already have the same user
	size_t i;
	RCursor<cObj> CurObj(GetObjs(*bestgrp1));
	for(CurObj.Start(),ptr=thObjs1,NbObjs1=0;!CurObj.End();CurObj.Next(),ptr++,NbObjs1++)
		(*ptr)=CurObj();
	for(ptr=thObjs1,i=NbObjs1+1;--i;ptr++)
	{
		if(bestgrp2->HasSameUser(*thObjs1))
			continue;
		bestgrp1->Delete(*ptr);
		bestgrp2->Insert(*ptr);
	}

	// Release the groups
	if(!bestgrp1->GetNbObjs())
		ReleaseGroup(bestgrp1);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::TreatSocialObjects(void)
{
	cObj* obj;
	R::RCursor<cGroup> Cur1(Used),Cur2(Used);
	cGroup* grp=0;
	double tmp,max;

	// Look for the groups to delete
	ToDel->Clear();
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		// Look if this group contains only 1 social profile
		if(Cur1()->GetNbObjs()!=1)
			continue;
		obj=Cur1()->GetObjPos(0);
		if(!Instance->Params->IsSocial(obj->GetId()))
			continue;

		// Find a new group
		for(Cur2.Start(),max=-2.0,grp=0;!Cur2.End();Cur2.Next())
		{
			if((Cur1()==Cur2())||(!Cur2()->GetNbObjs()))
				continue;
			if(Cur2()->HasSameUser(obj))
				continue;
			tmp=Instance->GetSim(obj,Cur2()->GetCentroid());
			if(tmp>max)
			{
				max=tmp;
				grp=Cur2();
			}
		}
		if(grp)
		{
			Cur1()->Delete(obj);
			grp->Insert(obj);
			ToDel->InsertPtr(Cur1());
		}
	}

	// Delete
	Cur1.Set(*ToDel);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
		ReleaseGroup(Cur1());
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::kMeansOptimisation(void)
{
	size_t i;
	size_t nb;
	RPromSol** s;
	RPromSol** tab;
	cChromo* LastDiv;
	cChromo* LastMerge;

	// Do not optimize
	if(!Instance->Params->NbDivChromo)
		return;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("kMeansOptimisation","RChromoSG");

	// Copy the current chromosome in thTests
	Evaluate();
	thTests[0]->Copy(*static_cast<cChromo*>(this));
	thTests[0]->Evaluate();
	LastDiv=LastMerge=static_cast<cChromo*>(this);
	for(i=Instance->Params->NbDivChromo+1;--i;)
	{
		thTests[i*2-1]->Copy(*LastDiv);
		LastDiv=thTests[i*2-1];
		LastDiv->DivideWorstObjects();
		thTests[i*2]->Copy(*LastMerge);
		LastMerge=thTests[i*2];
		LastMerge->MergeBestGroups();
	}

	// Do K-Means on the temporaries chromosomes.
	nb=(Instance->Params->NbDivChromo*2)+1;
	for(i=0;i<nb;i++)
	{
		thTests[i]->DoKMeans();
		thTests[i]->TreatSocialObjects();
		thTests[i]->Evaluate();
	}

	// Use PROMETHEE to determine the best solution.
	s=thSols;
	thProm->AssignChromo(*s,static_cast<cChromo*>(this));
	for(i=0,s++;i<nb;i++,s++)
	{
		thProm->AssignChromo(*s,thTests[i]);
	}
	thProm->ComputePrometheeII();
	if(thProm->GetBestSol()->GetId())
		Copy(*thTests[thProm->GetBestSol()->GetId()-1]);

	if(Instance->Debug)
	{
		for(tab=s=thProm->GetSols(),i=nb+2;--i;s++)
		{
			if((*s)->GetId())
				Instance->WriteChromoInfo(thTests[(*s)->GetId()-1]);
			else
				Instance->WriteChromoInfo(static_cast<cChromo*>(this));
		}
		delete[] tab;
		Instance->Debug->EndFunc("kMeansOptimisation","RChromoSG");
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::Optimisation(void)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("Optimisation","RChromoSG");
	kMeansOptimisation();
	if(Instance->Debug)
		Instance->Debug->EndFunc("Optimisation","RChromoSG");
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::LocalOptimisation(void)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("LocalOptimisation","RChromoSG");

	// If no non-assigned objects -> return
	if(!ObjsNoAss.GetNb())
	{
		if(Instance->Debug)
			Instance->Debug->EndFunc("LocalOptimisation","RChromoSG");
		return;
	}

	// Determine all non assigned objects
	size_t nbobjs(ObjsNoAss.GetTab(thObjs1));
	Instance->RandOrder(thObjs1,nbobjs);

	//std::cout<<"Test "<<Id<<std::endl;
	bool bOpti;
	size_t max(5);
	for(bOpti=true;(--max)&&bOpti&&nbobjs;)
	{
		bOpti=false;

		// Go trough existing groups
		RCursor<cGroup> Cur(Used);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()->DoOptimisation(thObjs1,nbobjs))
			{
				bOpti=true;
				break;
			}
		}
	}

	if(Instance->Debug)
		Instance->Debug->EndFunc("LocalOptimisation","RChromoSG");
}


//------------------------------------------------------------------------------
/*void RChromoSG::Mutation(void)
{
	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("Mutation","GChomoIR");

	MergeBestGroups();
	ComputeOrd();

	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("Mutation","GChomoIR");
}
*/

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	bool RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::SameGroup(size_t obj1,size_t obj2) const
{
	return(ObjectsAss[obj1]==ObjectsAss[obj2]);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::Copy(const cChromo& chromo)
{
	RChromoG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Copy(chromo);
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
	RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>::~RChromoSG(void)
{
}
