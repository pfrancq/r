/*

	R Project Library

	RFirstNodeHeuristic.h

	First Fit Heuristic adapted for Trees - Header

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
#ifndef RFirstNodeHeuristic_H
#define RFirstNodeHeuristic_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rtreeheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RFitstNodeHeuristic class provides a first fit heuristic for trees.
*
* Starting from the root node, the heuristic tries to find a branch where an
* object can be attached. It tries to find the highest node in the tree. If
* necessary, an intermediate node is created.
* @author Pascal Francq
* @short First Node Heuristic.
*/
template<class cNode,class cObj,class cNodes>
	class RFirstNodeHeuristic : public RTreeHeuristic<cNode,cObj,cNodes>
{
	using RTreeHeuristic<cNode,cObj,cNodes>::Nodes;
	using RTreeHeuristic<cNode,cObj,cNodes>::CurObj;
	using RTreeHeuristic<cNode,cObj,cNodes>::Random;

	/**
	* Temporary array of nodes.
	*/
	cNode** tmpNodes;

	/**
	 * Temporary list of attributes.
	 */
	RAttrList tmpAttrs;

	R::RTextFile Test;

public:

	/**
	* Construct the grouping heuristic.
	* @param r              The random generator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	RFirstNodeHeuristic(RRandom& r,RCursor<cObj> objs,RDebug* debug=0);

	/**
	* Initialize the heuristic.
	* @param nodes          Pointer to the nodes.
	*/
	virtual void Init(cNodes* nodes);

	/**
	 * Create a new child node to the current node with the set of attributes
	 * to place.
	 * @param attr           Attributes for the node.
	 * @param parent         Parent node.
	 */
	cNode* NewNode(const RAttrList& attr,cNode* parent);

	/**
	* Find a node where to attach the next object to treat.
	* @return Pointer to the node.
	*/
	virtual cNode* FindNode(void);

	/**
	* Destruct the heuristic.
	*/
	virtual ~RFirstNodeHeuristic(void);
};


//------------------------------------------------------------------------------
// Definitions of templates
#include <rfirstnodeheuristic.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
