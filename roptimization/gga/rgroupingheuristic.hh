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
//	RGroupingHeuristic<cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	RGroupingHeuristic<cGroup,cObj>::RGroupingHeuristic(RRandom* r,const unsigned int maxobjs)
		: Random(r)
{
	Order=new cObj*[maxobjs];
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	void RGroupingHeuristic<cGroup,cObj>::Init(cObj** objs,cGroup** groups,const unsigned int nbobjs)
{
	// Assign
	Groups=groups;
	Objs=objs;
	NbObjs=nbobjs;
	
	// Init the data for a grouping
	NbObjsOk=0;

	// Calculate an order
	for(unsigned int i=0;i<NbObjs;i++)
		Order[i]=Objs[i];
	//Random->RandOrder<cObj*>(Order,NbObjs);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	void RGroupingHeuristic<cGroup,cObj>::SelectNextObject(void) throw(RGroupingHeuristicException)
{
	CurObj=Order[NbObjsOk];
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	void RGroupingHeuristic<cGroup,cObj>::PutNextObject(void) throw(RGroupingHeuristicException)
{
	SelectNextObject();
	NextObject()->Insert(CurObj);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	void RGroupingHeuristic<cGroup,cObj>::Run(cObj** objs,cGroup** groups,const unsigned int nbobjs) throw(RGroupingHeuristicException)
{
	Init(objss,groups,nbobjs);
	while(NbObjsOk<NbObjs)
	{
		PutNextObject();
	}
	PostRun();
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	void RGroupingHeuristic<cGroup,cObj>::PostRun(void)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	RGroupingHeuristic<cGroup,cObj>::~RGroupingHeuristic(void)
{
	if(Order) delete[] Order;
}
