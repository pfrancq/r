/*

	R Project Library

	RGroupingHeuristic.h

	Generic Heuristic for Grouping - Header

	(C) 1998-2001 by P. Francq.

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
#ifndef RFirstFitHeuristicH
#define RFirstFitHeuristicH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rgroupingheuristic.h>


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RFitstFitHeuristic class provides a first fit heuristic.
* @author Pascal Francq
* @short Generic Grouping Heuristic class.
*/
template<class cGroup,class cObj,class cGroupData>
	class RFirstFitHeuristic : public RGroupingHeuristic<cGroup,cObj,cGroupData>
{

public:

	/**
	* Construct the grouping heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	*/
	RFirstFitHeuristic(RRandom* r,RGA::RObjs<cObj>* objs);

	/**
	* Find a group for the next object.
	*/
	virtual cGroup* FindGroup(void) throw(RGroupingHeuristicException);

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~RFirstFitHeuristic(void);
};


//-----------------------------------------------------------------------------
// Definitions of templates
#include <rgga/rfirstfitheuristic.hh>


}  //------- End of namespace RGGA --------------------------------------------


//-----------------------------------------------------------------------------
#endif
