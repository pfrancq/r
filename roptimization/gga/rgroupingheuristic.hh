/*

	R Project Library

	RGroupingHeuristic.hh

	Generic Heuristic for Grouping - Inline Implemenation

	Copyright 1998-2003 by the Université Libre de Bruxelles.

	Authors:
		

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
//
// class RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>::RGroupingHeuristic(const char* n,RRandom* r,RCursor<cObj,unsigned int>* objs)
		: Name(n), Random(r), Objs(objs), Groups(0)
{
	Order=new cObj*[Objs->GetNb()];
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>::Init(cGroups* groups)
{
	unsigned int* ass;

	// Assign
	Groups=groups;
	NbObjs=0;

	// Init the data for a grouping
	NbObjsOk=0;

	// Calculate an order
	for(Objs->Start(),ass=groups->ObjectsAss;!Objs->End();ass++,Objs->Next())
	{
		if((*ass)==NoGroup)
			Order[NbObjs++]=(*Objs)();
	}
	if(NbObjs)
		RandOrder<cObj*>(Order,NbObjs);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>::SelectNextObject(void) throw(eGA)
{
	CurObj=Order[NbObjsOk];
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>::PutNextObject(void) throw(eGA)
{
	SelectNextObject();
	CurGroup=FindGroup();
	CurGroup->Insert(CurObj);
	NbObjsOk++;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>::Run(cGroups* groups) throw(eGA)
{
	Init(groups);
	while(NbObjsOk<NbObjs)
	{
		PutNextObject();
	}
	PostRun();
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>::PostRun(void) throw(eGA)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>::~RGroupingHeuristic(void)
{
	if(Order) delete[] Order;
}
