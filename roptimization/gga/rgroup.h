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
#include <rgga/rgroups.h>


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constance
const unsigned int NoGroup=0xFFFFFFFF;


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	class RGroup
{
	/**
	* Identificator of the Group.
	*/
	unsigned int Id;

	/**
	* Owner of the group.
	*/
	RGroups<cGroup>* Owner;

	/**
	* Array of groups for the current chromosome (Reference).
	*/
	cGroup** Groups;

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
	* @param owner          Owner of the group.
	* @param id             Identificator of the group.
	*/
	RGroup(RGroups<cGroup>* owner,unsigned id);

	/**
	* Verify if the group is not violating the integrity of the system.
	* @return true if the group is correct, false else.
	*/
	bool Verify(void);

	/**
	* Clear the information container in a group.
	*/
	void Clear(void);

	/**
	* Compare two groups.
	* @returns always true.
	*/
	bool IsSameObjs(const RGroup* grp) const;

	/**
	* Put an object in the group.
	* @param obj            Pointer to the object to insert.
	*/
	void Insert(const cObj* obj) {Owner->InsertObj(this,obj->GetId());}

	/**
	* Delete an object in the group.
	* @param obj            Pointer to the object to delete.
	*/
	void Delete(const cObj* obj) {Owner->DeleteObj(this,obj->GetId());}

	/**
	* Look if an object can be insert in the group.
	* @param obj            Pointer to the object to insert.
	*/
	bool CanInsert(const cObj* /*obj*/) {return(true);}

	/**
	* Look if an object can be delete from the group.
	* @param obj            Pointer to the object to delete.
	*/
	bool CanDelete(const cObj* /*obj*/) {return(true);}

	/**
	* Assignment operator.
	* @param grp            The group used as source.
	*/
	RGroup& operator=(const RGroup& grp);

    /**
    * Destruct the group.
    */
	virtual ~RGroup(void);
	
	friend class RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
};


}  //------- End of namespace RGGA --------------------------------------------


//-----------------------------------------------------------------------------
#endif
