/*

	RChromoG.hh

	Class representing a chromosome for a GGA - Inline implementation

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RChromoG(cInst *inst,unsigned id) throw(std::bad_alloc)
		: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id),
		  RGroups<cGroup,cObj,cGroupData,cChromo>(inst->Objs,inst->MaxGroups),
		  Heuristic(0)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(cThreadData *thData) throw(std::bad_alloc)
{
	// Initialisation of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// Init "thread-dependent" data
	Heuristic=thData->Heuristic;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Clear(void)
{
	RGroups<cGroup,cObj,cGroupData,cChromo>::ClearGroups();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RandomConstruct(void) throw(eGA)
{
	Heuristic->Run(static_cast<cChromo*>(this));
	this->ComputeOrd();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::CopyGroups(cChromo* parent1,cChromo* parent2,unsigned int pos1,unsigned int nb1,unsigned int pos2,unsigned int nb2)
{
	unsigned int i,j,k;
	cGroup* grp;
	cGroup* src;
	bool bInsertIt;
//    RContainer<cObj,false,false> Del(20,10);

	RCursor<cGroup> Cur1(parent1->Used);
	RCursor<cGroup> Cur2(parent2->Used);
	for(i=nb2+1,Cur2.GoTo(pos2);--i;Cur2.Next())
	{
		bInsertIt=true;
		for(j=nb1+1,Cur1.GoTo(pos1);(--j)&&bInsertIt;Cur1.Next())
			if((this->Instance->EmptyModifiedGroups&&(Cur1()->CommonObjs(Cur2())))||(!Cur1()->IsCompatible(Cur2())))
			//if(((*grps1)->CommonObjs(*grps2))||(!((*grps1)->IsCompatible(*grps2))))
				bInsertIt=false;
		if(bInsertIt)
		{
//			std::cout<<"Create grp"<<std::endl;
//			Verify();
			grp=this->ReserveGroup();
			if(this->Instance->EmptyModifiedGroups)
			{
				// Only groups with no common objects are inserted -> all objects can be inserted
				grp->Copy(Cur2());
				(*grp)=(*Cur2());
			}
			else
			{
				// Must only insert the objects that are not in parent1
				src=Cur2();
				RCursor<cObj> obj(RGroups<cGroup,cObj,cGroupData,cChromo>::ObjsAss);
				for(k=src->GetNbObjs()+1,obj.GoTo(src->GetObjsPos());--k;obj.Next())
				{
					bInsertIt=true;
					for(j=nb1+1,Cur1.GoTo(pos1);(--j)&&bInsertIt;Cur1.Next())
					{
						if(Cur1()->IsIn(obj()->GetId()))
						{
							bInsertIt=false;
						}
					}
					if(bInsertIt)
						grp->Insert(obj());
				}
				if(!grp->GetNbObjs())
				{
					this->ReleaseGroup(grp);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Crossover(cChromo* parent1,cChromo* parent2) throw(eGA)
{
	unsigned int pos1,len1,pos2,i;
	cGroup* grp;

	#ifdef RGADEBUG
		if(this->Instance->Debug) this->Instance->Debug->BeginFunc("Crossover","RChromoG");
	#endif

	// Clear the chromosome
	Clear();

	// Select two crossing sites
	pos1=this->Instance->RRand(parent1->Used.GetNb());
	len1=this->Instance->RRand(parent1->Used.GetNb()-pos1-1)+1;
	pos2=this->Instance->RRand(parent2->Used.GetNb());

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects insert from parent1. //EmptyModifiedGroups
	CopyGroups(parent1,parent2,pos1,len1,0,pos2);

	// Insert the selected group from parent1
	RCursor<cGroup> Cur(parent1->Used);
	for(i=len1+1,Cur.GoTo(pos1);--i;Cur.Next())
	{
			grp=this->ReserveGroup();
			grp->Copy(Cur());
			(*grp)=(*Cur());
	}

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects insert from parent1.
	CopyGroups(parent1,parent2,pos1,len1,pos2,parent2->Used.GetNb()-pos2);

	// Insert missing objects after a local optimisation
	LocalOptimisation();
	Heuristic->Run(static_cast<cChromo*>(this));
	Optimisation();
	this->ComputeOrd();

	#ifdef RGADEBUG
		if(this->Instance->Debug) this->Instance->Debug->EndFunc("Crossover","RChromoG");
	#endif
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Mutation(void) throw(eGA)
{
	unsigned int nb;

	#ifdef RGADEBUG
		if(this->Instance->Debug) this->Instance->Debug->BeginFunc("Mutation","RChromoG");
	#endif

	// Compute number of groups to eliminate
	if(this->Used.GetNb()>10)
		nb=this->Used.GetNb()/5;
	else
		nb=2;
	while(--nb)
		ReleaseGroup(this->Instance->RRand(this->Used.GetNb()));

	// Insert missing objects after a local optimisation
	Heuristic->Run(static_cast<cChromo*>(this));
	this->ComputeOrd();
	
	#ifdef RGADEBUG
		if(this->Instance->Debug) this->Instance->Debug->EndFunc("Mutation","RChromoG");
	#endif
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Inversion(void) throw(eGA)
{
	unsigned int g1,g2,hold;

	#ifdef RGADEBUG
		if(this->Instance->Debug) this->Instance->Debug->BeginFunc("Inversion","RChromoG");
	#endif

	if(this->Used.GetNb()<3) return;
	g1=this->Instance->RRand(this->Used.GetNb());
	hold=g2=g1+this->Instance->RRand(this->Used.GetNb()-2)+1;
	if(g2>this->Used.GetNb()-1)
		g2-=this->Used.GetNb()-1;
	RReturnIfFail(g2!=g1);

	// Exchange them in Used
	this->Used.Exchange(g1,g2);

	#ifdef RGADEBUG
		if(this->Instance->Debug) this->Instance->Debug->EndFunc("Inversion","RChromoG");
	#endif
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Modify(void) throw(eGA)
{
	Mutation();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Verify(void) throw(eGA)
{
	RGroups<cGroup,cObj,cGroupData,cChromo>::Verify();
	if(!this->Used.GetNb())
		throw eGAVerify("No Group used.");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>&
		RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::operator=(const RChromoG& chromo)
{
	RChromo<cInst,cChromo,cFit,cThreadData>::operator=(chromo);
	RGroups<cGroup,cObj,cGroupData,cChromo>::operator=(chromo);
	return(*this);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::~RChromoG(void)
{
}
