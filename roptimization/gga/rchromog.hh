/*

	RChromoG.hh

	Class representing a chromosome for a GGA - Inline implementation

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
	ComputeOrd();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Crossover(cChromo* parent1,cChromo* parent2) throw(eGA)
{
	unsigned int pos1,len1,pos2,i,j;
	cGroup** grps2;
	cGroup** grps1;
	cGroup* grp;
	bool bInsertIt;

	#ifdef RGADEBUG
		if(Instance->Debug) Instance->Debug->BeginFunc("Crossover","RChromoG");
	#endif

	// Clear the chromosome
	Clear();

	// Select two crossing sites
	pos1=Instance->RRand(parent1->Used.NbPtr);
	len1=Instance->RRand(parent1->Used.NbPtr-pos1-1)+1;
	pos2=Instance->RRand(parent2->Used.NbPtr);

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects insert from parent1.
	for(i=pos2+1,grps2=parent2->Used.Tab;--i;grps2++)
	{
		bInsertIt=true;
		for(j=len1+1,grps1=&parent1->Used.Tab[pos1];(--j)&&bInsertIt;grps1++)
			if(((*grps1)->CommonObjs(*grps2))||(!((*grps1)->IsCompatible(*grps2))))
				bInsertIt=false;
		if(bInsertIt)
		{
			grp=ReserveGroup();
			grp->Copy(*grps2);
			(*grp)=(**grps2);
		}
	}

	// Insert the selected group from parent1
	for(i=len1+1,grps1=&parent1->Used.Tab[pos1];--i;grps1++)
	{
			grp=ReserveGroup();
			grp->Copy(*grps1);
			(*grp)=(**grps1);
	}

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects insert from parent1.
	for(i=parent2->Used.NbPtr-pos2+1,grps2=&parent2->Used.Tab[pos2];--i;grps2++)
	{
		bInsertIt=true;
		for(j=len1+1,grps1=&parent1->Used.Tab[pos1];(--j)&&bInsertIt;grps1++)
			if(((*grps1)->CommonObjs(*grps2))||(!((*grps1)->IsCompatible(*grps2))))
				bInsertIt=false;
		if(bInsertIt)
		{
			grp=ReserveGroup();
			grp->Copy(*grps2);
			(*grp)=(**grps2);
		}
	}

	// Insert missing objects after a local optimisation
	LocalOptimisation();
	Heuristic->Run(static_cast<cChromo*>(this));
	ComputeOrd();

	#ifdef RGADEBUG
		if(Instance->Debug) Instance->Debug->EndFunc("Crossover","RChromoG");
	#endif
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Mutation(void) throw(eGA)
{
	unsigned int nb;

	// Compute number of groups to eliminate
	if(Used.NbPtr>10)
		nb=Used.NbPtr/5;
	else
		nb=2;
	while(--nb)
		ReleaseGroup(Instance->RRand(Used.NbPtr));

	// Insert missing objects after a local optimisation
	LocalOptimisation();
	Heuristic->Run(static_cast<cChromo*>(this));
	ComputeOrd();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Inversion(void) throw(eGA)
{
	unsigned int g1,g2,hold;
	cGroup* ptr;

	if(Used.NbPtr<3) return;
	g1=Instance->RRand(Used.NbPtr);
	hold=g2=g1+Instance->RRand(Used.NbPtr-2)+1;
	if(g2>Used.NbPtr-1)
		g2-=Used.NbPtr-1;
	RReturnIfFail(g2!=g1);

	// Exchange them in Used
	ptr=Used.Tab[g1];
	Used.Tab[g1]=Used.Tab[g2];
	Used.Tab[g2]=ptr;
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
	if(!Used.NbPtr)
		throw eGAVerify("No Group used.");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>& RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::operator=(const RChromoG& chromo)
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
