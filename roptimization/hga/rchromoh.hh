/*

	R Project Library

	RChromoH.hh

	Class representing a tree (chromosome) - Inline implementation

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::RChromoH(cInst *inst,size_t id)
		: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id), RNodesGA<cNode,cObj,cChromo>(inst->Objs,inst->MaxNodes,inst->MaxAttr)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Init(cThreadData* thData)
{
	// Initialisation of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// Init "thread-dependent" data
	thNodes1=thData->tmpNodes1;
	thNodes2=thData->tmpNodes2;
	thObjects = &thData->tmpObjects;
	Heuristic=thData->Heuristic;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Clear(void)
{
	RNodesGA<cNode,cObj,cChromo>::ClearNodes();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::CleanUp(void)
{
	RContainer<cNode,false,false> ToDel(20);

	// Search for all nodes to remove
	RCursor<cNode> Cur(this->Used);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if((Cur()->GetNbObjs())||(Cur()->GetNbNodes()))
			continue;
		ToDel.InsertPtr(Cur());
	}

	// Remove them.
	Cur.Set(ToDel);
	for(Cur.Start();!Cur.End();Cur.Next())
		ReleaseNode(Cur());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::RandomConstruct(void)
{
	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("RamdomConstruct","RChromoH");
	Heuristic->Run(static_cast<cChromo*>(this));
	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("RamdomConstruct","RChromoH");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Crossover(cChromo* parent1,cChromo* parent2)
{
	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("Crossover","RChromoH");

	size_t i,j;
	cNode** Ref1;
	cNode** Ref2;
	cNode* Node1;
	cNode* Node2;
	cNode* Node;
	cNode* NewNode;

	// 1. Two nodes having the same attributes are choosen in each parent:
	// Node1 from parent1.
	// Node2 from parent2.
	size_t Nb1=parent1->Used.GetTab(thNodes1);
	size_t Nb2=parent2->Used.GetTab(thNodes2);
	this->Instance->RandOrder(thNodes1,Nb1);
	this->Instance->RandOrder(thNodes2,Nb2);
	for(i=Nb1+1,Ref1=thNodes1,Node1=Node2=0;(--i)&&(!Node1);Ref1++)
	{
		for(j=Nb2+1,Ref2=thNodes2;(--j)&&(!Node1);Ref2++)
		{
			if((*Ref1)->IsSame((*Ref2)->GetAttr()))
			{
				Node1 = (*Ref1);
				Node2 = (*Ref2);
			}
		}
	}

	if(Node1)
	{
		// Clear the current chromosome
		Clear();

		// Put information related to the crossover
		if(this->Instance->Debug)
		{
			static char tmp[500];
			static char tmp2[50];
			sprintf(tmp,"Node %u (",Node1->GetId());
			for(i=0;i<Node1->GetAttr().GetNbAttr();i++)
			{
				if(i>0) strcat(tmp,",");
				sprintf(tmp2,"%u",Node1->GetAttr()[i]);
				strcat(tmp,tmp2);
			}
			sprintf(tmp2,") from Chromosome %u used.",parent1->Id);
			strcat(tmp,tmp2);
			this->Instance->Debug->PrintInfo(tmp);
			sprintf(tmp,"Node %u (",Node2->GetId());
			for(i=0;i<Node2->GetAttr().GetNbAttr();i++)
			{
				if(i>0) strcat(tmp,",");
				sprintf(tmp2,"%u",Node2->GetAttr()[i]);
				strcat(tmp,tmp2);
			}
			sprintf(tmp2,") from Chromosome %u used.",parent2->Id);
			strcat(tmp,tmp2);
				this->Instance->Debug->PrintInfo(tmp);
		}

		// 2. parent1 is copied except its nodes having objects attached to Node2 and Node1.
		thObjects->Clear();
		Node2->GetAllObjects(*thObjects); // Put all objects of Node2 in ObjectsNode2
		Node=this->Top->Copy(parent1->Top,Node1,thObjects);

		if(Node)
		{
			// 3. Node2 is copied at the place of Node1.
			NewNode=this->ReserveNode();
			Node->Insert(NewNode);
			NewNode->Copy(Node2);
		}
		else
			RAssertMsg("Crossing node cannot be attached");

		// 4. The objects which are not attached are placed using the heuristic.
		Heuristic->Run(static_cast<cChromo*>(this));
	}
	else
		RAssertMsg("No crossing nodes where found")

	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("Crossover","RChromoH");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Mutation(void)
{
	if(this->Instance->Debug)
		this->Instance->Debug->BeginFunc("Mutation","RChromoH");

	// Choose Ramdomly a node and delete it.
	cNode* Node=this->NodesAss[this->Instance->RRand(this->NodesAss.GetNb())];
	if(this->Instance->Debug)
	{
		static char tmp[50];
		sprintf(tmp,"Node %u use for Mutation",Node->GetId());
		this->Instance->Debug->PrintInfo(tmp);
	}
	ReleaseNode(Node);

	// Used the heuristic to attach the objects attached to the node
	Heuristic->Run(static_cast<cChromo*>(this));

	if(this->Instance->Debug)
		this->Instance->Debug->EndFunc("Mutation","RChromoH");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Print(void)
{
/*	size_t i,*ptr;

	for(i=NbTopNodes,ptr=&NodesList[TopNodes];i--;ptr++)
		Nodes[*ptr]->Print(0);*/
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Verify(void)
{
	RNodesGA<cNode,cObj,cChromo>::VerifyNodes();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>& RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::operator=(const RChromoH& chromo)
{
	RChromo<cInst,cChromo,cFit,cThreadData>::operator=(chromo);
	RNodesGA<cNode,cObj,cChromo>::operator=(chromo);
	return(*this);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::~RChromoH(void)
{
}
