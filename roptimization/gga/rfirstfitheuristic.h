/*

	R Project Library

	RGroupingHeuristic.h

	Generic Heuristic for Grouping - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

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
#ifndef RFirstFitHeuristic_H
#define RFirstFitHeuristic_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rgga/rgroupingheuristic.h>
#include <rga/rdebug.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RFitstFitHeuristic class provides a first fit heuristic.
* @author Pascal Francq
* @short Generic Grouping Heuristic class.
*/
template<class cGroup,class cObj,class cGroupData,class cGroups>
	class RFirstFitHeuristic : public RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>
{

public:

	/**
	* Construct the grouping heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	RFirstFitHeuristic(RRandom* r,RCursor<cObj>* objs,RDebug* debug=0);

	/**
	* Find a group for the next object.
	*/
	virtual cGroup* FindGroup(void) throw(eGA);

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~RFirstFitHeuristic(void);
};


//------------------------------------------------------------------------------
// Definitions of templates
#include <rgga/rfirstfitheuristic.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
