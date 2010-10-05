/*

	R Project Library

	RSHeuristicSC.h

	Similarity-based Clustering Heuristic - Header

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RHeuristicSC_H
#define RHeuristicSC_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroupingheuristic.h>
#include <scga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RHeuristicSC class provides a specific heuristic for the similarity-based
* clustering algorithm.
* @author Pascal Francq
* @short Similarity-based Clustering Heuristic.
*/
template<class cGroup,class cObj,class cGroups>
	class RHeuristicSC : public RGroupingHeuristic<cGroup,cObj,cGroups>
{
	using RGroupingHeuristic<cGroup,cObj,cGroups>::Objs;
	using RGroupingHeuristic<cGroup,cObj,cGroups>::NbObjs;
	using RGroupingHeuristic<cGroup,cObj,cGroups>::Groups;
	class Local;

	/**
	 * Groups to delete because they contain only one "social" object.
	 */
	RContainer<cGroup,false,false> ToDel;

	/**
	 * Objects to reassigned.
	 */
	RContainer<cObj,false,false> ToReAssign;

	/**
	 * Parameters of the SCGA.
	 */
	RParamsSC* Params;

	/**
	 * Container that count the number of nearest neighbors
	 */
	RContainer<Local,true,false> NbNearestNeighbors;

	/**
	 * Hold the number of possible groups for all objects to group.
	 */
	size_t NbPossibleGrps;

public:

	/**
	* Construct the heuristic.
	* @param r               The random generator to use.
	* @param objs            Pointer to the objects.
	* @param params          Parameters.
	* @param debug           Debugger.
	*/
	 RHeuristicSC(R::RRandom& r,R::RCursor<cObj> objs,RParamsSC* params,R::RDebug* debug=0);

	/**
	* Initialize the heuristic.
	* @param groups          Pointer to the groups.
	*/
	virtual void Init(cGroups* groups);

	/**
	* Find a group for the next object.
	* @param obj             Object.
	*/
	virtual cGroup* FindGroup(cObj* obj);

	/**
	* Verify that no social object is alone and that all the groups has to
	* minimum number of objects.
	*/
	virtual void PostRun(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rheuristicsc.hh>


}  //------- End of namespace R ------------------------------------------------


//-----------------------------------------------------------------------------
#endif
