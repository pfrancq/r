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
	RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RChromoG(cInst *inst,unsigned id)
		: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id),
		  RGroups<cGroup,cObj,cGroupData,cChromo>(inst->Objs,inst->MaxGroups),
		  Heuristic(0)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(cThreadData *thData)
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
	double RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Similarity(const cChromo& chromo) const
{
	unsigned int NbRows,NbCols;                   // Rows and Cols for matrix
	unsigned int NbObjs;                          // Total Number of objects
	unsigned int col;
	cGroup* Group2;
	double a,b,c,d,num,den;
	double* VectorRows=0;                         // Sum of the rows of the matrix
	double* VectorCols=0;                         // Sum of the columns of the matrix
	double* VectorColsTemp=0;                     // temp sum of the columns of the matrix
	double* ptr;
	double Total;

	// Init part
	Total=0.0;
	NbObjs=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	NbRows=this->Used.GetNb();
	NbCols=chromo.Used.GetNb();
	if((!NbRows)||(!NbCols))
		return(-1.0);
	VectorRows=new double[NbRows];
	VectorCols=new double[NbCols];
	VectorColsTemp=new double[NbCols];

	// Construction of the container for relation between id and column in the matrix.
	RContainer<GroupId,true,true> GroupsId(NbCols,NbCols/2);
	RCursor<cGroup> Cur2(chromo.Used);
	for(Cur2.Start(),col=0;!Cur2.End();Cur2.Next())
		GroupsId.InsertPtr(new GroupId(Cur2()->GetId(),col++));

	// Initialisation of the variable used for computing the subtotal
	a=b=c=d=0.0;

	// Initalisation of the vectors
	memset(VectorRows,0,NbRows*sizeof(double));
	memset(VectorCols,0,NbCols*sizeof(double));

	// For each group of the current chromosome and for each object in this group
	// -> Compute the differents terms of the total
	int row,position;
	row=0;

	RCursor<cGroup> Cur1(this->Used);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		memset(VectorColsTemp,0,NbCols*sizeof(double));
		RCursor<cObj> Objs(GetObjs(*Cur1()));
		for(Objs.Start();!Objs.End();Objs.Next())
		{
			if(!GetGroup(Objs()))
				continue;
			VectorRows[row]++;
			NbObjs++;
			Group2=chromo.GetGroup(Objs());
			if(!Group2)
				continue;
			position=GroupsId.GetPtr(Group2->GetId())->position;
			VectorCols[position]++;
			VectorColsTemp[position]++;
		}
		row++;
		for(col=NbCols+1,ptr=VectorColsTemp;--col;ptr++)
			a+=(((*ptr)*((*ptr)-1))/2);
	}

	// Finish the computation
	for(col=NbCols+1,ptr=VectorCols;--col;ptr++)
		b+=(((*ptr)*((*ptr)-1))/2);
	for(row=NbRows+1,ptr=VectorRows;--row;ptr++)
		c+=(((*ptr)*((*ptr)-1))/2);
	d=(NbObjs*(NbObjs-1))/2;
	num=a-((b*c)/d);
	den=(0.5*(b+c))-(b*c/d);
	if(den<0.0000001)
		return(-1.0);
	Total=num/den;

	// Delete the vectors
	delete[] VectorRows;
	delete[] VectorCols;
	delete[] VectorColsTemp;

	// Return the result
	return(Total);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RandomConstruct(void)
{
	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("RamdomConstruct","RChromoG");
	Heuristic->Run(static_cast<cChromo*>(this));
	this->ComputeOrd();
	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("RamdomConstruct","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::CopyGroups(cChromo* parent1,cChromo* parent2,unsigned int pos1,unsigned int begin,unsigned int end,unsigned int pos2,unsigned int nb2)
{
	unsigned int i,j;
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
			if((this->Instance->EmptyModifiedGroups&&(Cur1()->CommonObjs(Cur2())))||(!Cur1()->IsCompatible(Cur2())))
				bInsertIt=false;

		// Verify if the 'end' groups of parent1 after pos1
		for(j=end+1,Cur1.GoTo(pos1);(--j)&&bInsertIt;Cur1.Next())
			if((this->Instance->EmptyModifiedGroups&&(Cur1()->CommonObjs(Cur2())))||(!Cur1()->IsCompatible(Cur2())))
				bInsertIt=false;

		// If group can be inserted -> do it
		if(bInsertIt)
		{
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
					this->ReleaseGroup(grp);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Crossover(cChromo* parent1,cChromo* parent2)
{
	unsigned int pos1;              // Position in parent1
	unsigned int end;               // Number of groups after pos1 to copy from parent1
	unsigned int begin;             // Number of first groups to copy from parent1
	unsigned int pos2;              // Position where the groups must be copied in parent2
	unsigned int i;
	cGroup* grp;

	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("Crossover","RChromoG");

	// Clear the chromosome
	Clear();

	// Select two crossing sites
	pos1=this->Instance->RRand(parent1->Used.GetNb());
	//len1=this->Instance->RRand(parent1->Used.GetNb()-pos1-1)+1;
	end=this->Instance->RRand(parent1->Used.GetNb()-1)+1;
	if(end>parent1->Used.GetNb()-pos1)
	{
		i=parent1->Used.GetNb()-pos1;
		begin=end-i;
		end=i;
	}
	else
		begin=0;
	pos2=this->Instance->RRand(parent2->Used.GetNb());

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects insert from parent1.
	CopyGroups(parent1,parent2,pos1,begin,end,0,pos2);

	// Insert the selected group from parent1
	RCursor<cGroup> Cur(parent1->Used);
	for(i=begin+1,Cur.Start();--i;Cur.Next())   // Copy 'begin' first groups
	{
			grp=this->ReserveGroup();
			grp->Copy(Cur());
			(*grp)=(*Cur());
	}
	for(i=end+1,Cur.GoTo(pos1);--i;Cur.Next())  // Copy 'end' groups from pos1
	{
			grp=this->ReserveGroup();
			grp->Copy(Cur());
			(*grp)=(*Cur());
	}

	// Insert groups from parent2<pos2 and verify that they dont contains "new"
	// objects insert from parent1.
	CopyGroups(parent1,parent2,pos1,begin,end,pos2,parent2->Used.GetNb()-pos2);

	// Insert missing objects after a local optimisation
	LocalOptimisation();
	Heuristic->Run(static_cast<cChromo*>(this));
	Optimisation();
	this->ComputeOrd();

	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("Crossover","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Mutation(void)
{
	unsigned int nb;

	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("Mutation","RChromoG");

	// Compute number of groups to eliminate
	if(this->Used.GetNb()>10)
		nb=this->Used.GetNb()/5;
	else
		nb=2;
	while(--nb)
		ReleaseGroup(this->Used[this->Instance->RRand(this->Used.GetNb())]);

	// Insert missing objects after a local optimisation
	Heuristic->Run(static_cast<cChromo*>(this));
	this->ComputeOrd();

	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("Mutation","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Inversion(void)
{
	unsigned int g1,g2,hold;

	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("Inversion","RChromoG");

	if(this->Used.GetNb()<3) return;
	g1=this->Instance->RRand(this->Used.GetNb());
	hold=g2=g1+this->Instance->RRand(this->Used.GetNb()-2)+1;
	if(g2>this->Used.GetNb()-1)
		g2-=this->Used.GetNb()-1;
	RReturnIfFail(g2!=g1);

	// Exchange them in Used
	this->Used.Exchange(g1,g2);

	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("Inversion","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Modify(void)
{
	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("Modify","RChromoG");

	Mutation();

	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("Modify","RChromoG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Verify(void)
{
	RGroups<cGroup,cObj,cGroupData,cChromo>::Verify();
	if(!this->Used.GetNb())
		throw RGAException("No Group used.",RGAException::eGAVerify);
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
