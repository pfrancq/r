/*

	RChromoG.hh

	Class representing a chromosome for a GGA - Inline implementation

	(C) 2001 by P. Francq.

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



//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RChromoG(cInst *inst,unsigned id) throw(bad_alloc)
		: RGA::RChromo<cInst,cChromo,cFit,cThreadData>(inst,id),
		  RGroups<cGroup,cObj,cGroupData>(inst->NbObjs,inst->MaxGroups),
		  Heuristic(0)
{
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(cThreadData *thData) throw(bad_alloc)
{
	// Initialisation of the parent
	RGA::RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// Init "thread-dependent" data
	Heuristic=thData->Heuristic;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Clear(void)
{
	RGroups<cGroup,cObj,cGroupData>::ClearGroups();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	bool RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RandomConstruct(void)
{
	Heuristic->Run(Objs,this,NbObjs);
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	bool RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Crossover(cChromo* parent1,cChromo* parent2)
{
	unsigned int pos1,len1,pos2,i,j;
	cGroup** grps2;
	cGroup** grps1;
	cGroup* grp;
	bool bInsertIt;

	// Clear the chromosome
	Clear();
	
	// Select two crossing sites
	pos1=Instance->RRand(parent1->Used.NbPtr);
	len1=Instance->RRand(parent1->Used.NbPtr-pos1-1)+1;
	pos2=Instance->RRand(parent2->Used.NbPtr);

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects inseert from parent1.
	for(i=pos2+1,grps2=parent2->Used.Tab;--i;grps2++)
	{
		bInsertIt=true;
		for(j=len1+1,grps1=&parent1->Used.Tab[pos1];(--j)&&bInsertIt;grps1++)
			if((*grps1)->CommonObjs(*grps2))
				bInsertIt=false;
		if(bInsertIt)
		{
			grp=ReserveGroup();
			grp->Insert(Objs,*grps2);
		}
	}

	// Insert the selected group from parent1
	for(i=len1+1,grps1=&parent1->Used.Tab[pos1];--i;grps1++)
	{
			grp=ReserveGroup();
			grp->Insert(Objs,*grps1);
	}

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects inseert from parent1.
	for(i=parent2->Used.NbPtr-pos2+1,grps2=&parent2->Used.Tab[pos2];--i;grps2++)
	{
		bInsertIt=true;
		for(j=len1+1,grps1=&parent1->Used.Tab[pos1];(--j)&&bInsertIt;grps1++)
			if((*grps1)->CommonObjs(*grps2))
				bInsertIt=false;
		if(bInsertIt)
		{
			grp=ReserveGroup();
			grp->Insert(Objs,*grps2);
		}
	}

	// Insert missing objects after a local optimisation
	LocalOptimisation();
	Heuristic->Run(Objs,this,NbObjs);

	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	bool RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Mutation(void)
{
	unsigned int nb;

	// Compute number of groups to eliminate
	if(Used.NbPtr>10)
		nb=Instance->RRand(Used.NbPtr/10-3)+4;
	else
		nb=2;
	while(--nb)
		ReleaseGroup(Instance->RRand(Used.NbPtr));

	// Insert missing objects after a local optimisation
	LocalOptimisation();
	Heuristic->Run(Objs,this,NbObjs);

	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	bool RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Inversion(void)
{
	unsigned int grp1,grp2,hold;
	cGroup* ptr;

	if(Used.NbPtr<3) return(true);
	grp1=Instance->RRand(Used.NbPtr);
	hold=grp2=grp1+Instance->RRand(Used.NbPtr-2)+1;
	if(grp2>Used.NbPtr-1)
		grp2-=Used.NbPtr-1;
	RReturnValIfFail(grp2!=grp1,false);

	// Exchange them in Used
	ptr=Used.Tab[grp1];
	Used.Tab[grp1]=Used.Tab[grp2];
	Used.Tab[grp2]=ptr;

	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	bool RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Verify(void)
{
	if(!RGroups<cGroup,cObj,cGroupData>::Verify())
		return(false);
	if(!Used.NbPtr)
	{
		cout<<"No Group used."<<endl;
		return(false);
	}
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>& RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::operator=(const RChromoG& chromo)
{
	RGA::RChromo<cInst,cChromo,cFit,cThreadData>::operator=(chromo);
	RGroups<cGroup,cObj,cGroupData>::operator=(chromo);
	return(*this);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::~RChromoG(void)
{
}
