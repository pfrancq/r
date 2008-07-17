/*

	R Project Library

	RFirstNodeHeuristic.h

	First Fit Heuristic adapted for Trees - Header

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
* @author Pascal Francq
* @short First Node Heuristic class.
*/
template<class cNode,class cObj,class cNodes>
	class RFirstNodeHeuristic : public RTreeHeuristic<cNode,cObj,cNodes>
{
	/**
	* Temporary array of nodes.
	*/
	cNode** tmpNodes;

	/**
	 * Current Node treated.
	 */
	cNode* CurNode;
	
	/**
	 * Current attributes for which a new node must be found.
	 */
	const RAttrList* CurAttr;
	
public:

	/**
	* Construct the grouping heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	RFirstNodeHeuristic(RRandom* r,RCursor<cObj> objs,RDebug* debug=0);

	/**
	* Initialize the heuristic.
	* @param nodes          Pointer to the nodes.
	*/
	virtual void Init(cNodes* nodes);

	/**
	* Random the position of elements of a vector using the current random
	* generator.
	* @param arr            A pointer to the array representing the vector.
	* @param size           The size of the vector.
	*/
	template<class T> inline void RandOrder(T* arr,size_t size)
		{this->Random->RandOrder<T>(arr,size);}

	/**
	 * Create a new child node to the current node with the set of attributes
	 * to place.
	 */
	cNode* NewNode(void);
	
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
