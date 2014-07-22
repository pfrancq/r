/*

	R Project Library

	RTreeHeuristic.hh

	Generic Heuristic for Tree - Inline Implemenation

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
//
// class RTreeHeuristic<cNode,cObj,cNodes>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RTreeHeuristic<cNode,cObj,cNodes>::RTreeHeuristic(const RString& n,RRandom& r,RCursor<cObj> objs,RDebug* debug)
		: Name(n), Random(r), Objs(objs), Nodes(0), Debug(debug)
{
	Order=new cObj*[Objs.GetNb()];
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RTreeHeuristic<cNode,cObj,cNodes>::Init(cNodes* nodes)
{
	size_t *Cur;

	// Init the data for a grouping
	NbObjsOk=0;
	Nodes=nodes;
	NbObjs=0;

	// Calculate an order
	for(Objs.Start(),Cur=Nodes->ObjectsAss;!Objs.End();Cur++,Objs.Next())
	{
		if((*Cur)==cNoRef)
			Order[NbObjs++]=Objs();
	}
	RandOrder<cObj*>(Order,NbObjs);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RTreeHeuristic<cNode,cObj,cNodes>::BuildFile(const RString& name,cObj* obj)
{
	RTextFile File(name);
	File.Open(RIO::Create);
	if(obj)
	{
		File<<"Obj"<<obj->GetId()<<"[";
		RNumCursor<size_t> List(obj->GetAttr());
		for(List.Start();!List.End();List.Next())
		{
			File<<List();
		}
		File<<"]"<<endl<<endl<<endl;
	}
	RCursor<cNode> Cur(Nodes->GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->PrintNode(File,0);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RTreeHeuristic<cNode,cObj,cNodes>::SelectNextObject(void)
{
	CurObj=Order[NbObjsOk];
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RTreeHeuristic<cNode,cObj,cNodes>::PutNextObject(void)
{
	SelectNextObject();
	CurNode=FindNode();
	Nodes->InsertObj(CurNode,CurObj);
	NbObjsOk++;
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RTreeHeuristic<cNode,cObj,cNodes>::Run(cNodes* nodes)
{
	Init(nodes);
	//std::cout<<std::endl<<"Chromo "<<nodes->Id<<std::endl;;
	while(NbObjsOk<NbObjs)
	{
		PutNextObject();
	}
	//cout<<std::endl;
	PostRun();
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RTreeHeuristic<cNode,cObj,cNodes>::PostRun(void)
{
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RTreeHeuristic<cNode,cObj,cNodes>::~RTreeHeuristic(void)
{
	if(Order) delete[] Order;
}
