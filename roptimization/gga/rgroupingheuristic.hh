/*

	R Project Library

	RGroupingHeuristic.hh

	Generic Heuristic for Grouping - Inline Implemenation

	(C) 1998-2001 by By P. Francq.

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



//-----------------------------------------------------------------------------
//
//	RGroupingHeuristic<cGroup,cObj,cGroupData>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroupingHeuristic<cGroup,cObj,cGroupData>::RGroupingHeuristic(RRandom* r,const unsigned int maxobjs)
		: Random(r), Groups(0), Objs(0)
{
	Order=new cObj*[maxobjs];
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroupingHeuristic<cGroup,cObj,cGroupData>::Init(cObj** objs,RGroups<cGroup,cObj,cGroupData>* groups,const unsigned int nbobjs)
{
	unsigned int i;
	unsigned int* ass;
	cObj** obj;

	// Assign
	Groups=groups;
	Objs=objs;
	NbObjs=0;
	
	// Init the data for a grouping
	NbObjsOk=0;

	// Calculate an order
	for(i=nbobjs+1,ass=groups->ObjectsAss,obj=Objs;--i;ass++,obj++)
	{
		if((*ass)==NoGroup)
			Order[NbObjs++]=(*obj);
	}
	RandOrder<cObj*>(Order,NbObjs);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroupingHeuristic<cGroup,cObj,cGroupData>::SelectNextObject(void) throw(RGroupingHeuristicException)
{
	CurObj=Order[NbObjsOk];
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroupingHeuristic<cGroup,cObj,cGroupData>::PutNextObject(void) throw(RGroupingHeuristicException)
{
	SelectNextObject();
	CurGroup=FindGroup();
	CurGroup->Insert(CurObj);
	NbObjsOk++;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroupingHeuristic<cGroup,cObj,cGroupData>::Run(cObj** objs,RGroups<cGroup,cObj,cGroupData>* groups,const unsigned int nbobjs) throw(RGroupingHeuristicException)
{
	Init(objs,groups,nbobjs);
	while(NbObjsOk<NbObjs)
	{
		PutNextObject();
	}
	PostRun();
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroupingHeuristic<cGroup,cObj,cGroupData>::PostRun(void)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroupingHeuristic<cGroup,cObj,cGroupData>::~RGroupingHeuristic(void)
{
	if(Order) delete[] Order;
}
