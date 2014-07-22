/*

	R Project Library

	RChromoH.hh

	Class representing a tree (chromosome) - Inline implementation

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
	// Initialization of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// Initialize "thread-dependent" data
	thNodes1=thData->tmpNodes1;
	thNodes2=thData->tmpNodes2;
	thObjects = &thData->tmpObjects;
	Heuristic=thData->Heuristic;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Clear(void)
{
	RNodesGA<cNode,cObj,cChromo>::Clear();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::CleanUp(void)
{
	RContainer<cNode,false,false> ToDel(20);

	// Search for all nodes to remove
	RCursor<cNode> Cur(Nodes);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if((Cur()->GetNbObjs())||(Cur()->GetNbNodes()))
			continue;
		ToDel.InsertPtr(Cur());
	}

	// Remove them.
	Cur.Set(ToDel);
	for(Cur.Start();!Cur.End();Cur.Next())
		DeleteNode(Cur());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::RandomConstruct(void)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("RamdomConstruct","RChromoH");
	Heuristic->Run(dynamic_cast<cChromo*>(this));
	if(Instance->Debug)
		Instance->Debug->EndFunc("RamdomConstruct","RChromoH");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Crossover(cChromo* parent1,cChromo* parent2)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("Crossover","RChromoH");

	size_t i,j;
	cNode** Ref1;
	cNode** Ref2;
	cNode* Node1(0);        // Node from parent1 that will be used for the crossover
	cNode* Node2(0);        // Node from parent2 that will be used for the crossover

	// 1. Two nodes having the same attributes are chosen in each parent:
	size_t Nb1=parent1->Nodes.GetTab(thNodes1);
	size_t Nb2=parent2->Nodes.GetTab(thNodes2);
	Instance->RandOrder(thNodes1,Nb1);
	Instance->RandOrder(thNodes2,Nb2);
	for(i=Nb1+1,Ref1=thNodes1;(--i)&&(!Node1);Ref1++)
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

	if(Node1&&Node2)
	{
		// Clear the current chromosome
		Clear();

		// Put information related to the crossover
		if(Instance->Debug)
		{
			RString tmp("Node "+RString::Number(Node1->GetId())+" (");
			for(i=0;i<Node1->GetAttr().GetNbAttr();i++)
			{
				if(i>0)
					tmp+=",";
				tmp+=RString::Number(Node1->GetAttr()[i]);
			}
			tmp+=") from Chromosome "+RString::Number(parent1->Id)+" used.";
			Instance->Debug->PrintInfo(tmp);
			tmp="Node "+RString::Number(Node2->GetId())+" (";
			for(i=0;i<Node2->GetAttr().GetNbAttr();i++)
			{
				if(i>0)
					tmp+=",";
				tmp+=RString::Number(Node2->GetAttr()[i]);
			}
			tmp+=") from Chromosome "+RString::Number(parent2->Id)+" used.";
			Instance->Debug->PrintInfo(tmp);
		}

		// 2. parent1 is copied except the branch starting with Node1 and the objects attached to Node2 and its child nodes.
		thObjects->Clear();
		Node2->GetAllObjects(*thObjects); // Put all objects of Node2 and child nodes in ObjectsNode2
		cNode* Node(CopyExceptBranch(parent1,Node1,thObjects));

		// 3. Create a new node and use it to get the branch of Node2
		cNode* NewNode(ReserveNode());
		InsertNode(Node,NewNode);
		CopyExceptBranch(NewNode,Node2); // (Branch to exclude is null)

		// 4. The objects which are not attached are placed using the heuristic.
		Heuristic->Run(reinterpret_cast<cChromo*>(this));
	}
	else
		std::cerr<<"No crossing nodes where found"<<std::endl;

	if(Instance->Debug)
		Instance->Debug->EndFunc("Crossover","RChromoH");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Mutation(void)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("Mutation","RChromoH");

	// Choose Randomly a node and delete it.
	cNode* Node(Nodes[Instance->RRand(GetNbNodes())]);
	if(Instance->Debug)
		Instance->Debug->PrintInfo("Node "+RString::Number(Node->GetId())+" use for Mutation");
	DeleteNode(Node);

	// Used the heuristic to attach the objects attached to the node
	Heuristic->Run(reinterpret_cast<cChromo*>(this));

	if(Instance->Debug)
		Instance->Debug->EndFunc("Mutation","RChromoH");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Print(void)
{
//	RNodeCursor<cChromo,cNode> Cur(this);
//	for(Cur.Start();!Cur.End();Cur.Next)
//		Cur()->Print(0);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Verify(void)
{
	RNodesGA<cNode,cObj,cChromo>::VerifyNodes(true);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Copy(const cChromo& chromo)
{
	RChromo<cInst,cChromo,cFit,cThreadData>::Copy(chromo);
	RNodesGA<cNode,cObj,cChromo>::CopyTree(chromo);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::~RChromoH(void)
{
}
