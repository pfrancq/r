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
// RGroupingHeuristic<cGroup,cObj,cGroupData>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RFirstFitHeuristic<cGroup,cObj,cGroupData>::RFirstFitHeuristic(RRandom* r,RStd::RCursor<cObj,unsigned int>* objs)
	: RGroupingHeuristic<cGroup,cObj,cGroupData>(r,objs)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	cGroup* RFirstFitHeuristic<cGroup,cObj,cGroupData>::FindGroup(void) throw(RGGA::RGroupingHeuristicException)
{
	cGroup* grp;

	for(Groups->Used.Start();!Groups->Used.End();Groups->Used.Next())
		if(Groups->Used()->CanInsert(CurObj))
			return(Groups->Used());
	grp=Groups->ReserveGroup();
	return(grp);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RFirstFitHeuristic<cGroup,cObj,cGroupData>::~RFirstFitHeuristic(void)
{
}
