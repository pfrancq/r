/*

	R Project Library

	RGroupingHeuristic.hh

	Generic Heuristic for Grouping - Inline Implemenation

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// class RGroupingHeuristic<cGroup,cObj,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RGroupingHeuristic<cGroup,cObj,cGroups>::RGroupingHeuristic(const R::RString& n,R::RRandom& r,R::RCursor<cObj> objs,R::RDebug* debug)
		: Name(n), Random(r), Objs(objs), Groups(0), Debug(debug)
{
	Order=new cObj*[Objs.GetNb()];
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingHeuristic<cGroup,cObj,cGroups>::Init(cGroups* groups)
{
	Groups=groups;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingHeuristic<cGroup,cObj,cGroups>::BuildOrder(void)
{
	NbObjsOk=0;
	NbObjs=Groups->ObjsNoAss.GetTab(Order);
	RandOrder<cObj*>(Order,NbObjs);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingHeuristic<cGroup,cObj,cGroups>::Run(cGroups* groups)
{
	// Initialize
	Init(groups);
	BuildOrder();

	// Group resting objects
	for(cObj** CurObj=Order;NbObjsOk<NbObjs;CurObj++,NbObjsOk++)
	{
		cGroup* CurGroup=FindGroup(*CurObj);
		if(!CurGroup)
			throw RGAException("No Group find.",RGAException::eGAHeuristic);
		CurGroup->Insert(*CurObj);
	}
	PostRun();
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingHeuristic<cGroup,cObj,cGroups>::PostRun(void)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RGroupingHeuristic<cGroup,cObj,cGroups>::~RGroupingHeuristic(void)
{
	if(Order) delete[] Order;
}
