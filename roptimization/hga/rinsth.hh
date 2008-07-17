/*

	R Project Library

	RInstH.hh

	Class representing an instance of a HGA - Inline Implementation

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
//
// RThreadDataH<cInst,cChromo,cFit,cThreadData,cNode,cObj>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RThreadDataH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::RThreadDataH(cInst *owner)
		: RThreadData<cInst,cChromo>(owner), tmpNodes1(0), tmpNodes2(0), tmpObjects(owner->Objs.GetNb()),
		  Heuristic(0)
{
	tmpNodes1 = new cNode*[owner->MaxNodes];
	tmpNodes2 = new cNode*[owner->MaxNodes];
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RThreadDataH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Init(void)
{
	RThreadData<cInst,cChromo>::Init();
	Heuristic=this->Owner->CreateHeuristic();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RThreadDataH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::~RThreadDataH(void)
{
	delete[] tmpNodes1;
	delete[] tmpNodes2;
	delete Heuristic;
}



//------------------------------------------------------------------------------
//
// RInstH<cInst,cChromo,cFit,cThreadData,cNode,class cObj>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RInstH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::RInstH(size_t popsize,RCursor<cObj> objs,HeuristicType h,const RString& name,RDebug* debug)
		: RInst<cInst,cChromo,cFit,cThreadData>(popsize,name,debug), Heuristic(h), Objs(objs)
{
	ControlAttr=MaxAttr=MaxNodes=0;
	for(Objs.Start();!Objs.End();Objs.Next())
	{
		size_t tmp=Objs()->Attr.GetNbAttr();
		size_t tmp2=Objs()->Attr[tmp-1];
		if(tmp>MaxAttr) MaxAttr=tmp;
		if(tmp2>ControlAttr) ControlAttr=tmp2;
		for(tmp++;--tmp;)
			MaxNodes+=tmp;
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RInstH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::Init(void)
{
	cChromo **C;
	size_t i;

	RInst<cInst,cChromo,cFit,cThreadData>::Init();
	for(i=this->GetPopSize()+1,C=this->Chromosomes;--i;C++)
		(static_cast<RNodesGA<cNode,cObj,cChromo>*>(*C))->Init();
	(static_cast<RNodesGA<cNode,cObj,cChromo>*>(this->BestChromosome))->Init();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RTreeHeuristic<cNode,cObj,cChromo>* RInstH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::CreateHeuristic(void)
{
	RTreeHeuristic<cNode,cObj,cChromo>* h;

	switch(Heuristic)
	{
		case FirstFit:
			h = new RFirstNodeHeuristic<cNode,cObj,cChromo>(this->Random,Objs,this->Debug);
			break;
		default:
			h=0;
			break;
	}
	return(h);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RInstH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::PostRun(void)
{
	this->BestChromosome->Verify();
//  ptr->CompactNodes();
//  ptr->MoveUpNodes();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	void RInstH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::DisplayInfos(void)
{
/*	tTree i;
	cTree *ptr;

	cout<<"  Gen "<<Gen<<" ";
  for(i=0,ptr=Trees;i<NbTrees;i++,ptr++) cout<<ptr->Fitness<<" ";
  cout<<endl;*/
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	RInstH<cInst,cChromo,cFit,cThreadData,cNode,cObj>::~RInstH(void)
{
}
