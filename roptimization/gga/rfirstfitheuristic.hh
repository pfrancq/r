/*

	R Project Library

	RGroupingHeuristic.hh

	Generic Heuristic for Grouping - Inline Implemenation

	Copyright 1998-2007 by the Université Libre de Bruxelles.

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
// class RGroupingHeuristic<cGroup,cObj,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RFirstFitHeuristic<cGroup,cObj,cGroups>::RFirstFitHeuristic(RRandom* r,RCursor<cObj>* objs,RDebug* debug)
	: RGroupingHeuristic<cGroup,cObj,cGroups>("First-Fit Heuristic",r,objs,debug)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cGroup* RFirstFitHeuristic<cGroup,cObj,cGroups>::FindGroup(void)
{
	cGroup* grp;

	RCursor<cGroup> G(this->Groups->Used);
	for(G.Start();!G.End();G.Next())
		if(G()->CanInsert(this->CurObj))
			return(G());
	grp=this->Groups->ReserveGroup();
	return(grp);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RFirstFitHeuristic<cGroup,cObj,cGroups>::~RFirstFitHeuristic(void)
{
}
