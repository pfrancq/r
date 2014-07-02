/*

	R Project Library

	RGroup.h

	Group of a GGA - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
#ifndef RGroup_H
#define RGroup_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcursor.h>
#include <rgga.h>
#include <robjg.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
template<class cGroup,class cObj,class cGroups> class RGroups;


//------------------------------------------------------------------------------
/**
* The RGroup class provides a representation for a group that will contain
* objects.
* @author Pascal Francq.
* @short Group.
*/
template<class cGroup,class cObj,class cGroups>
	class RGroup
{
protected:

	/**
	* Identifier of the Group.
	*/
	size_t Id;

	/**
	* Owner of the group.
	*/
	cGroups* Owner;

	/**
	* Index of the first object attached to the group.
	*/
	size_t SubObjects;

	/**
	* Number of objects attached to the group.
	*/
	size_t NbSubObjects;

	/**
	* Is the group reserved?
	*/
	bool Reserved;

public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	RGroup(RGroup* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	*/
	RGroup(cGroups* owner,const size_t id);

	/**
	* Verify if the group is not violating the integrity of the system.
	*/
	virtual void Verify(void);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Put an object in the group.
	* @param obj            Pointer to the object to insert.
	*/
	void Insert(cObj* obj) {Owner->InsertObj(static_cast<cGroup*>(this),obj);}

	/**
	* Delete an object in the group.
	* @param obj            Pointer to the object to delete.
	*/
	void Delete(cObj* obj) {Owner->DeleteObj(static_cast<cGroup*>(this),obj);}

	/**
	* Method call after an object was inserted in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual void PostInsert(const cObj* obj);

	/**
	 * Method call before all the objects of a given group are inserted into
	 * the current one. This method can be used to copy internal information
	 * from one group to the other.
	 * @param from           The group used as source.
	 * @return true if the PostInsert method should be called after each object
	 * is inserted from the group (true is the default value).
	 */
	virtual bool PreInsertAll(const cGroup* from);

	/**
	* Method call after one object was deleted from the group.
	* @param obj            Pointer to the object to delete.
	*/
	virtual void PostDelete(const cObj* obj);

	/**
	 * Method call each time all the objects are deleted from the group.
	 * @return true if the PostDelete method should be called after each object
	 * was deleted from the group (true if the default value).
	 */
	virtual bool PreDeleteAll(void);

	/**
	* Look if an object can be insert in the group. By default, the method
	* returns always true.
	* @param obj            Pointer to the object to insert.
	*/
	virtual bool CanInsert(const cObj* obj);

	/**
	* Look if an object is "compatible" with a specific group. By default, the
	* method returns always true.
	* @param obj            Pointer to the object to verify.
	*/
	virtual bool IsCompatible(const cObj* obj);

	/**
	* Look if an object can be delete from the group. By default, the method
	* returns always true.
	* @param obj            Pointer to the object to delete.
	*/
	virtual bool CanDelete(const cObj* obj);

	/**
	* Return the identifier of the group.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Get the number of objects in the group.
	* @return size_t.
	*/
	size_t GetNbObjs(void) const {return(NbSubObjects);}

	/**
	* Get the position of the first object in the group.
	* @return size_t.
	*/
	size_t GetObjsPos(void) const {return(SubObjects);}

	/**
	* Return a cursor over the objects of a group.
	*/
	RCursor<cObj> inline GetObjs(void) const {return(Owner->GetObjs(static_cast<const cGroup&>(*this)));}

	/**
	* Return true if an object is in the group.
	* @param id             Identifier of the object to test.
	*/
	bool IsIn(const size_t id) const;

	/**
	* Look if the two groups have common objects.
	* @param grp            Pointer to the group.
	* @return boolean.
	*/
	bool CommonObjs(const cGroup* grp) const;

	/**
	* Look if two groups are "compatible".
	* @param grp            Pointer to the group.
	* @return boolean.
	*/
	virtual bool IsCompatible(const cGroup* grp) const;

	/**
	* Return true if the two groups have the same objects.
	* @param grp            Pointer to the group.
	*/
	bool SameObjs(const cGroup* grp) const;

	/**
	* Return the list of the objects. The list is created by RGroup, but must
	* be deleted by the caller. The list is ended by a cNoRef value.
	*/
	size_t* GetObjectsId(void) const;

	/**
	* Compare two groups. Used for RContainer.
	*/
	int Compare(const cGroup& grp) const {return(CompareIds(Id,grp.Id));}

	/**
	* Get a pointer to an object at a given position in the group.
	* @param pos            Position.
	* @returns cObj*
	*/
	cObj* GetObjPos(const size_t pos);

	/**
	* Destruct the group.
	*/
	virtual ~RGroup(void);

	// friend classes
	friend class RGroups<cGroup,cObj,cGroups>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rgroup.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
