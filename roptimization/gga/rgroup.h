/*

	R Project Library

	RGroup.h

	Group of a GGA - Header.

	(C) 2001 by P. Francq.

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
#ifndef RGroupH
#define RGroupH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/robjs.h>
#include <rgga/robjg.h>


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
template<class cGroup,class cObj,class cGroupData,class cGroups> class RGroups;


//-----------------------------------------------------------------------------
// Constance
const unsigned int NoGroup=0xFFFFFFFF;


//-----------------------------------------------------------------------------
/**
* The RGroup class provides a representation for a group that will contain
* objects.
* @author Pascal Francq.
* @short Group.
*/
template<class cGroup,class cObj,class cGroupData,class cGroups>
	class RGroup
{
protected:

	/**
	* Identificator of the Group.
	*/
	unsigned int Id;

	/**
	* Owner of the group.
	*/
	cGroups* Owner;

	/**
	* Index of the first object attached to the group.
	*/
	unsigned int SubObjects;

	/**
	* Number of objects attached to the group.
	*/
	unsigned int NbSubObjects;

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
	* @param id             Identificator of the group.
	* @param data           Specific data to construct the group.
	*/
	RGroup(cGroups* owner,const unsigned int id,const cGroupData* data=0);

	/**
	* Verify if the group is not violating the integrity of the system.
	* @return true if the group is correct, false else.
	*/
	virtual bool Verify(void);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Put an object in the group.
	* @param obj            Pointer to the object to insert.
	*/
	void Insert(const cObj* obj) {Owner->InsertObj(static_cast<cGroup*>(this),obj);}

	/**
	* Put all the objects of a group. The two groups have to be of two
	* different owner.
	* @param objs           Objects manipulated.
	* @param grp            Group to copy from.
	*/
	void Insert(RStd::RCursor<cObj,unsigned int>* objs,const cGroup* grp);

	/**
	* Delete an object in the group.
	* @param obj            Pointer to the object to delete.
	*/
	void Delete(const cObj* obj) {Owner->DeleteObj(static_cast<cGroup*>(this),obj);}

	/**
	* Method call after an object was inserted in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual void PostInsert(const cObj* /*obj*/) {}

	/**
	* Method call after an object was deleted from the group.
	* @param obj            Pointer to the object to delete.
	*/
	virtual void PostDelete(const cObj* /*obj*/) {}

	/**
	* Look if an object can be insert in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual bool CanInsert(const cObj* /*obj*/) const {return(true);}

	/**
	* Look if an object is "compatible" with a specific group.
	* @param obj            Pointer to the object to verify.
	*/
	virtual bool IsCompatible(const cObj* /*obj*/) const {return(true);}

	/**
	* Look if an object can be delete from the group.
	* @param obj            Pointer to the object to delete.
	*/
	virtual bool CanDelete(const cObj* /*obj*/) const {return(true);}

	/**
	* Assignment operator.
	* @param grp            The group used as source.
	*/
	RGroup& operator=(const RGroup& grp);

	/**
	* Return the identificator of the group.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Get the number of objects in the group.
	* @return unsigned int.
	*/
	unsigned int GetNbObjs(void) const {return(NbSubObjects);}

	/**
	* Return true if an object is in the group.
	* @param id             Identificator of the object to test.
	*/
	bool IsIn(const unsigned int id) const;

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
	virtual bool IsCompatible(const cGroup* /*grp*/) const {return(true);}

	/**
	* Return true if the two groups have the same objects.
	* @param grp            Pointer to the group.
	*/
	bool SameObjs(const cGroup* grp) const;

	/**
	* Return the list of the objects. The list is created by RGroup, but must
	* be deleted by the caller. The list is ended by a NoObject value.
	*/
	unsigned int* GetObjectsId(void) const;

	/**
	* Compare two groups. Used for RContainer.
	*/
	int Compare(const cGroup* grp) const {return(Id-grp->Id);}

	/**
	* Destruct the group.
	*/
	virtual ~RGroup(void);

	// friend classes
	friend RGroups<cGroup,cObj,cGroupData,cGroups>;
};


}  //------- End of namespace RGGA --------------------------------------------


//-----------------------------------------------------------------------------
#endif
