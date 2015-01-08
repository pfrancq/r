/*

	R Project Library

	RGroupNN.h

	NNGGA Group - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RGroupNN_H
#define RGroupNN_H

//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroup.h>
#include <rquicksort.h>
#include <rmaxvector.h>
class GCAInst;
class GCAObj;


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RGroupNN provides a representation for a group for the Nearest Neighbors
* Grouping Genetic Algorithm.
* @author Pascal Francq
* @short NNGGA Group.
*/
template<class cGroup,class cObj,class cGroups>
	class RGroupNN : public RGroup<cGroup,cObj,cGroups>
{
	using RGroup<cGroup,cObj,cGroups>::Id;
	using RGroup<cGroup,cObj,cGroups>::Owner;
	using RGroup<cGroup,cObj,cGroups>::NbSubObjects;

public:

	using RGroup<cGroup,cObj,cGroups>::GetObjs;

protected:

	/**
	 * Must the group be re-evaluated?
	 */
	bool ToEval;

public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	RGroupNN(RGroupNN* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	*/
	RGroupNN(cGroups* owner,const size_t id);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Verify if an object has the same parent as another object of the group.
	* @param obj            Pointer to the object to insert.
	*/
	bool HasSameUser(const cObj* obj) const;

	/**
	* Look if an object can be insert in the group. In practice, the object
	* must have a minimum similarity and maximum disagreement with the objects
	* of the group. Moreover, two objects having the same parents cannot be
	* grouped together.
	* not allowed.
	* @param obj             Object to insert.
	*/
	virtual bool CanInsert(const cObj* obj);

	/**
	* Method call after an object was inserted in the group.
	* @param obj             Object to insert.
	*/
	virtual void PostInsert(const cObj* obj);

	/**
	 * Method call before all the objects of a given group are inserted into
	 * the current one. This method can be used to copy internal information
	 * from one group to the other.
	 * @param from           The group used as source.
	 * @return false (the values are update in the method).
	 */
	virtual bool PreInsertAll(const cGroup* from);

	/**
	* Method call after an object was deleted from the group.
	* @param obj             Object to delete.
	*/
	virtual void PostDelete(const cObj* obj);

	/**
	 * Method call each time all the objects are deleted from the group.
	 * @return false (the values are update in the method).
	 */
	virtual bool PreDeleteAll(void);

	/**
	* Destruct the group.
	*/
	virtual ~RGroupNN(void);

	friend class OrderBySim;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rgroupnn.hh>


}  //------- End of namespace R ------------------------------------------------


//-----------------------------------------------------------------------------
#endif
