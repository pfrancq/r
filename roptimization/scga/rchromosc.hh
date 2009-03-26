/*

	R Project Library

	RChromoSC.cpp

	Similarity-based Clustering Chromosome - Implementation

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
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
	  thProm(0), thSols(0)
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
	double super(-2.0), max(-2.0),tmp;

	CritAgreement=CritDisagreement=CritSimJ=0.0;
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
			tmp=Instance->GetSim(Cur()->GetCentroid()->GetElementId(),Cur2()->GetCentroid()->GetElementId());
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
/*
void RChromoSC::ReAllocate(void)
{
	size_t nb;
	double sim,maxsim;
	GCAGroup* grp;
	GCAObj** Cur;

	// Put the prototypes in Protos
	Protos.Clear();
	R::RCursor<GCAGroup> Grp(Used);
	for(Grp.Start();!Grp.End();Grp.Next())
		Protos.InsertPtr(Grp()->GetCentroid());

	// Clear the chromosome
	Clear();

	// Insert the Prototypes in a group
	RCursor<GCAObj> CurP(Protos);
	for(CurP.Start();!CurP.End();CurP.Next())
	{
		grp=ReserveGroup();
		grp->Insert(CurP());
		grp->SetCentroid(CurP());
	}

	// Mix randomly thObjs1
//	Instance->RandOrder<GCAObj*>(thObjs1,Objs->GetNb());

	// Go through the randomly ordered objects and put them in the group of the
	// most similar prototype.
	for(Cur=thObjs1,nb=Objs.GetNb()+1;--nb;Cur++)
	{
		// If the object is a prototype -> already in a group
		if(GetGroup(*Cur)) continue;

		// Look first if one of the object with a ratio are already grouped
		RCursor<GCAMaxRatio> Best(*Instance->Ratios[(*Cur)->GetId()]);
		for(Best.Start(),grp=0;(!Best.End())&&(!grp);Best.Next())
			grp=GetGroup(Best()->ObjId);

		// If no group find, -> Go through each groups
		if(!grp)
		{
			R::RCursor<GCAGroup> Grp(Used);
			for(Grp.Start(),maxsim=-1.0;!Grp.End();Grp.Next())
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

		// If no group find -> Create a new group and make the current object the
		// prototype of it.
		if(!grp)
		{
			grp=ReserveGroup();
			grp->SetCentroid(*Cur);
		}

		// Insert the object in the current group.
		grp->Insert(*Cur);
	}
}


//-----------------------------------------------------------------------------
size_t RChromoSC::CalcNewProtosNb(void)
{
	size_t count;
	R::RCursor<GCAGroup> Grp;
	GCAObj* OldProto;

	// Computed the prototypes for each groups and count the number in Protos
	Grp.Set(Used);
	for(Grp.Start(),count=0;!Grp.End();Grp.Next())
	{
		OldProto=Grp()->Centroid;
//		Grp()->ComputeRelevant();
		if(OldProto!=Grp()->GetCentroid())
			count++;
	}
	return(count);
}


//-----------------------------------------------------------------------------
void RChromoSC::DoKMeans(void)
{
	size_t itermax;
	double error,minerror;
	GCAObj** obj;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("DoKMeans","GChomoIR");

	// Copy the objects into thObjs1
	R::RCursor<GCAGroup> Grp(Used);
	for(Grp.Start(),obj=thObjs1;!Grp.End();Grp.Next())
	{
		RCursor<GCAObj> ptr=GetObjs(Grp());
		for(ptr.Start();!ptr.End();ptr.Next(),obj++)
		{
			(*obj)=ptr();
		}
	}

	// Mix randomly thObjs1
	Instance->RandOrder<GCAObj*>(thObjs1,Objs.GetNb());

	// Max Iterations
	minerror=Instance->Params->Convergence/100.0;
	for(itermax=Instance->Params->MaxKMeans+1,error=1.0;(--itermax)&&(error>minerror);)
	{
		ReAllocate();
		error=static_cast<double>(CalcNewProtosNb())/static_cast<double>(Used.GetNb());
	}

	if(Instance->Debug)
	{
		Instance->Debug->PrintInfo("Number of k-Means runs: "+RString::Number(itermax));
		Instance->Debug->EndFunc("DoKMeans","GChomoIR");
	}
}


//-----------------------------------------------------------------------------
void RChromoSC::DivideWorstObjects(void)
{
	R::RCursor<GCAGroup> Grp;
	size_t i;
	GCAObj** ptr;
	GCAObj** ptr2;
	GCAObj** ptr3;
	double sim,minsim;
	size_t prof;
	size_t prof1;
	size_t prof2;
	GCAGroup* grp;
	GCAObj* worst1=0;
	GCAObj* worst2=0;

	// Find the group containing the two most dissimilar objects
	Grp.Set(Used);
	for(Grp.Start(),minsim=1.0,grp=0;!Grp.End();Grp.Next())
	{
		if(Grp()->GetNbObjs()<3) continue;
		RCursor<GCAObj> CurObj(GetObjs(*Grp()));
		RCursor<GCAObj> CurObj2(GetObjs(*Grp()));
		for(CurObj.Start(),i=0;i<Grp()->GetNbObjs()-1;CurObj.Next(),i++)
		{
			for(CurObj2.GoTo(i+1);!CurObj2.End();CurObj2.Next())
			{
				sim=Instance->GetSim(CurObj()->GetElementId(),CurObj2()->GetElementId());
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
	prof1=worst1->GetElementId();
	prof2=worst2->GetElementId();
	RCursor<GCAObj> CurObj(GetObjs(*grp));
	for(CurObj.Start();!CurObj.End();CurObj.Next())
	{
		if((CurObj()==worst1)||(CurObj()==worst2)) continue;
		prof=CurObj()->GetElementId();
		sim=Instance->GetSim(prof1,prof);
		minsim=Instance->GetSim(prof2,prof);
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
void RChromoSC::MergeBestGroups(void)
{
	size_t i;
	GCAGroup* bestgrp1((*this)[MostSimilarGroup1]);
	GCAGroup* bestgrp2((*this)[MostSimilarGroup2]);
	GCAObj** ptr;

	// Put the objects of bestgrp1 in bestgrp2 only if there are not already have the same user
	RCursor<GCAObj> CurObj(GetObjs(*bestgrp1));
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
*/

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::TreatSocialObjects(void)
{
	cObj* obj;
	R::RCursor<cGroup> Cur1(Used),Cur2(Used);
	cGroup* grp=0;
	double tmp,max;

//	cout<<"Do not treat social"<<endl;
	return;

	// Look for the groups to delete
	ToDel->Clear();
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		// Look if this group contains only 1 social profile
		if(Cur1()->GetNbObjs()!=1)
			continue;
		obj=Cur1()->GetObjPos(0);
		if(!obj->IsSocial())
			continue;

		// Find a new group
		for(Cur2.Start(),max=-2.0,grp=0;!Cur2.End();Cur2.Next())
		{
			if((Cur1()==Cur2())||(!Cur2()->GetNbObjs()))
				continue;
			if(Cur2()->HasSameUser(obj))
				continue;
			tmp=Instance->GetSim(obj->GetElementId(),Cur2()->GetCentroid()->GetElementId());
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
/*void RChromoSC::kMeansOptimisation(void)
{
	size_t i;
	size_t nb;
	RPromSol** s;
	RPromSol** tab;
	RChromoGC* LastDiv;
	RChromoGC* LastMerge;

	// Do not optimize
	if(!Instance->Params->NbDivChromo)
		return;

	// Copy the current chromosome in thTests
	Evaluate();
	(*thTests[0])=(*this);
	LastDiv=LastMerge=this;
	for(i=Instance->Params->NbDivChromo+1;--i;)
	{
		(*thTests[i*2-1])=(*LastDiv);
		LastDiv=thTests[i*2-1];
		LastDiv->DivideWorstObjects();
		(*thTests[i*2])=(*LastMerge);
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
	thProm->AssignChromo(*s,this);
	for(i=0,s++;i<nb;i++,s++)
	{
		thProm->AssignChromo(*s,thTests[i]);
	}
	thProm->ComputePrometheeII();
	if(thProm->GetBestSol()->GetId())
		(*this)=(*thTests[thProm->GetBestSol()->GetId()-1]);

	if(Instance->Debug)
	{
		for(tab=s=thProm->GetSols(),i=nb+2;--i;s++)
		{
			if((*s)->GetId())
				Instance->WriteChromoInfo(thTests[(*s)->GetId()-1]);
			else
				Instance->WriteChromoInfo(this);
		}
		delete[] tab;
	}
}
*/

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RChromoSC<cInst,cChromo,cThreadData,cGroup,cObj>::Optimisation(void)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("Optimisation","GChomoIR");
	std::cout<<"kMeansOptimisation();"<<std::endl;
	if(Instance->Debug)
		Instance->Debug->EndFunc("Optimisation","GChomoIR");
}


//------------------------------------------------------------------------------
/*void RChromoSC::Mutation(void)
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
