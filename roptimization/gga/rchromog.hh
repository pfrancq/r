/*

	R Project Library

	RChromoG.hh

	Class representing a chromosome for a GGA - Inline implementation

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RChromoG(cInst *inst,size_t id)
		: R::RChromo<cInst,cChromo,cFit,cThreadData>(inst,id),
		  R::RGroups<cGroup,cObj,cChromo>(inst->Objs,inst->MaxGroups),
		  Heuristic(0)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Init(cThreadData *thData)
{
	// Initialization of the parent
	R::RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// Init "thread-dependent" data
	Heuristic=thData->Heuristic;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Clear(void)
{
	R::RGroups<cGroup,cObj,cChromo>::ClearGroups();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RandomConstruct(void)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("RamdomConstruct","RChromoG");
	Heuristic->Run(static_cast<cChromo*>(this));
	ComputeOrd();
	if(Instance->Debug)
		Instance->Debug->EndFunc("RamdomConstruct","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::CopyGroups(cChromo* parent1,cChromo* parent2,size_t pos1,size_t begin,size_t end,size_t pos2,size_t nb2)
{
	size_t i,j;
	cGroup* grp;
	cGroup* src;
	bool bInsertIt;

	RCursor<cGroup> Cur1(parent1->Used);
	RCursor<cGroup> Cur2(parent2->Used);
	for(i=nb2+1,Cur2.GoTo(pos2);--i;Cur2.Next())
	{
		// By default, group can be insert
		bInsertIt=true;

		// Verify if the 'begin' first groups of parent1
		for(j=begin+1,Cur1.Start();(--j)&&bInsertIt;Cur1.Next())
			if((Instance->EmptyModifiedGroups&&(Cur1()->CommonObjs(Cur2())))||(!Cur1()->IsCompatible(Cur2())))
				bInsertIt=false;

		// Verify if the 'end' groups of parent1 after pos1
		for(j=end+1,Cur1.GoTo(pos1);(--j)&&bInsertIt;Cur1.Next())
			if((Instance->EmptyModifiedGroups&&(Cur1()->CommonObjs(Cur2())))||(!Cur1()->IsCompatible(Cur2())))
				bInsertIt=false;

		// If group can be inserted -> do it
		if(bInsertIt)
		{
			grp=ReserveGroup();
			if(Instance->EmptyModifiedGroups)
			{
				// Only groups with no common objects are inserted -> all objects can be inserted
				grp->CopyObjs(Cur2());
				grp->CopyInfos(Cur2());
			}
			else
			{
				// Must only insert the objects that are not in parent1
				src=Cur2();
				RCursor<cObj> obj=GetObjs(*src);
				for(obj.Start();!obj.End();obj.Next())
				{
					// By default, insert the object.
					bInsertIt=true;

					// Verify if the 'begin' first groups of parent1
					for(j=begin+1,Cur1.Start();(--j)&&bInsertIt;Cur1.Next())
					{
						if(Cur1()->IsIn(obj()->GetId()))
						{
							bInsertIt=false;
						}
					}

					// Verify if the 'end' groups of parent1 after pos1
					for(j=end+1,Cur1.GoTo(pos1);(--j)&&bInsertIt;Cur1.Next())
					{
						if(Cur1()->IsIn(obj()->GetId()))
						{
							bInsertIt=false;
						}
					}

					// If object can be inserted -> do it
					if(bInsertIt)
						grp->Insert(obj());
				}
				if(!grp->GetNbObjs())
				{
					ReleaseGroup(grp);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Crossover(cChromo* parent1,cChromo* parent2)
{
	size_t pos1;              // Position in parent1
	size_t end;               // Number of groups after pos1 to copy from parent1
	size_t begin;             // Number of first groups to copy from parent1
	size_t pos2;              // Position where the groups must be copied in parent2
	size_t i;
	cGroup* grp;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("Crossover","RChromoG");

	// Clear the chromosome
	Clear();

	// Select two crossing sites
	pos1=Instance->RRand(parent1->Used.GetNb());
	//len1=Instance->RRand(parent1->Used.GetNb()-pos1-1)+1;
	end=Instance->RRand(parent1->Used.GetNb()-1)+1;
	if(end>parent1->Used.GetNb()-pos1)
	{
		i=parent1->Used.GetNb()-pos1;
		begin=end-i;
		end=i;
	}
	else
		begin=0;
	pos2=Instance->RRand(parent2->Used.GetNb());

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects insert from parent1.
	CopyGroups(parent1,parent2,pos1,begin,end,0,pos2);

	// Insert the selected group from parent1
	RCursor<cGroup> Cur(parent1->Used);
	for(i=begin+1,Cur.Start();--i;Cur.Next())   // Copy 'begin' first groups
	{
			grp=ReserveGroup();
			grp->CopyObjs(Cur());
			grp->CopyInfos(Cur());
	}
	for(i=end+1,Cur.GoTo(pos1);--i;Cur.Next())  // Copy 'end' groups from pos1
	{
			grp=ReserveGroup();
			grp->CopyObjs(Cur());
			grp->CopyInfos(Cur());
	}

	// Insert groups from parent2<pos2 and verify that they don't contains "new"
	// objects insert from parent1.
	CopyGroups(parent1,parent2,pos1,begin,end,pos2,parent2->Used.GetNb()-pos2);

	// Insert missing objects after a local optimization
	if(Instance->DoLocalOptimisation)
		LocalOptimisation();
	Heuristic->Run(static_cast<cChromo*>(this));
	if(ObjsNoAss.GetNb())
		throw RGAException(RString::Number(ObjsNoAss.GetNb())+" objects are not assigned",RGAException::eGACrossover);
	if(Instance->DoOptimisation)
	{
		Optimisation();
		if(ObjsNoAss.GetNb())
			throw RGAException(RString::Number(ObjsNoAss.GetNb())+" objects are not assigned",RGAException::eGACrossover);
	}
	ComputeOrd();

	if(Instance->Debug)
		Instance->Debug->EndFunc("Crossover","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Mutation(void)
{
	size_t nb;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("Mutation","RChromoG");

	// Compute number of groups to eliminate
	if(Used.GetNb()>10)
		nb=Used.GetNb()/5;
	else
		nb=2;
	while(--nb)
		ReleaseGroup(Used[Instance->RRand(Used.GetNb())]);

	// Insert missing objects after a local optimization
	if(Instance->DoLocalOptimisation)
		LocalOptimisation();
	Heuristic->Run(static_cast<cChromo*>(this));
	if(Instance->DoOptimisation)
		Optimisation();
	ComputeOrd();

	if(Instance->Debug)
		Instance->Debug->EndFunc("Mutation","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Inversion(void)
{
	size_t g1,g2,hold;

	if(Used.GetNb()<3)
		return;

	if(Instance->Debug)
		Instance->Debug->BeginFunc("Inversion","RChromoG");

	g1=Instance->RRand(Used.GetNb());
	hold=g2=g1+Instance->RRand(Used.GetNb()-2)+1;
	if(g2>Used.GetNb()-1)
		g2-=Used.GetNb()-1;
	RReturnIfFail(g2!=g1);

	// Exchange them in Used
	Used.Exchange(g1,g2);

	if(Instance->Debug)
		Instance->Debug->EndFunc("Inversion","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Modify(void)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("Modify","RChromoG");

	Mutation();

	if(Instance->Debug)
		Instance->Debug->EndFunc("Modify","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Verify(void)
{
	R::RGroups<cGroup,cObj,cChromo>::Verify();
	if(!Used.GetNb())
		throw RGAException("No Group used.",RGAException::eGAVerify);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Copy(const cChromo& chromo)
{
	R::RChromo<cInst,cChromo,cFit,cThreadData>::Copy(chromo);
	R::RGroups<cGroup,cObj,cChromo>::CopyGrouping(chromo);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	R::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::~RChromoG(void)
{
}
