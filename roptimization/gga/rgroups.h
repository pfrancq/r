/*

	R Project Library

	RGroups.h

	Groups - Header

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef RGroups_H
#define RGroups_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rga/robjs.h>
#include <rgga/rgga.h>
#include <rgga/rgroup.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RGroups class provides a representation for a set of groups that have to
* contain objects.
* @author Pascal Francq
* @short Groups.
*/
template<class cGroup,class cObj,class cGroupData,class cGroups>
	class RGroups : public RContainer<cGroup,unsigned int,true,false>
{
public:

	/**
	*  Groups used.
	*/
	RContainer<cGroup,unsigned int,false,false> Used;

	/**
	* Data used to construct a group.
	*/
	cGroupData* GroupData;

protected:

	/**
	* The Objects to group.
	*/
	RCursor<cObj,unsigned int>* Objs;

	/**
	* Assignment of the objects.
	*/
	unsigned int* ObjectsAss;

	/**
	* Container of objects already assigned.
	*/
	RContainer<cObj,unsigned int,false,false> ObjsAss;

	/**
	* Container of objects not assigned.
	*/
	RContainer<cObj,unsigned int,false,false> ObjsNoAss;

	/**
	* Assignment of the objects where the identificator of the first group
	* found is always 1. This array is used to compare if two solutions are
	* representing the same "real" groupment.
	*/
	unsigned int* OrdObjectsAss;

	/**
	* Array representing the identificators assigned to each group to compute
	* OrdObjectsAss.
	*/
	unsigned int* NewUsedId;

public:

	/**
	* Construct the groups.
	* @param objs           Objects to group.
	* @param max            Maximal number of groups to create.
	*/
	RGroups(RCursor<cObj,unsigned int>* objs,const unsigned int max) throw(bad_alloc);

	/**
	* Init the groups.
	* @param data           Data used to construct the groups.
	*/
	void Init(cGroupData* data);

	/**
	* Clear all the information of the chromosome.
	*/
	void ClearGroups(void);

	/**
	* Reserve a group.
	* @returns A pointer to the node reserved.
	*/
	cGroup* ReserveGroup(void);

	/**
	* Release a group.
	* @param group          Identificator of the group.
	*/
	void ReleaseGroup(const unsigned int group);

	/**
	* Release a group.
	* @param group          Pointer to the group.
	*/
	void ReleaseGroup(cGroup* group);

	/**
	* Insert an object to a group.
	* @param to             Pointer of the group.
	* @param obj            Pointer of the object to insert.
	*/
	void InsertObj(cGroup* to,const cObj* obj);

	/**
	* Delete an object from a node.
	* @param from           Pointer of the node.
	* @param obj            Pointer of the object to insert.
	*/
	void DeleteObj(cGroup* from,const cObj* obj);

	/**
	* Delete all the objects attached to a given group.
	* @param node           Pointer to the group.
	*/
	void DeleteObjs(cGroup* node);

	/**
	* Verify the validity of the groups.
	* @return True if the groups are valid one, false else.
	*/
	virtual void Verify(void) throw(eGA);

	/**
	* Get the group of an object.
	* @param id             Identificator of the object.
	* @returns Pointer to cGroup*.
	*/
	cGroup* GetGroup(const unsigned int id);

	/**
	* Get the group of an object.
	* @param obj            Pionter to the object.
	* @returns Pointer to cGroup*.
	*/
	cGroup* GetGroup(const cObj* obj);

	/**
	* Get a pointer to an object at a given index.
	* @param idx            Global Index.
	* @returns cObj*
	*/
	cObj* GetObj(const unsigned int idx) {return(ObjsAss.Tab[idx]);}

	/**
	* Return a pointer to the objects of a group.
	*/
	cObj** GetObjs(const unsigned int idx) {return(&ObjsAss.Tab[idx]);}

	/**
	* The assigment operator.
	* @param chromo         The chromosome used as source.
	*/
	RGroups& operator=(const RGroups& grps);

	/**
	* Compute OrdObjectsAss.
	*/
	void ComputeOrd(void);

	/**
	* This method look if the two configurations are representing the same
	* groupement.
	* @param grps           Configuration used for the comparaison.
	*/
	bool SameGroupment(const RGroups* grps) const;

	/**
	* Destruct the chromosome.
	*/
	virtual ~RGroups(void);

	// friend classes
	friend class RGroup<cGroup,cObj,cGroupData,cGroups>;
	friend class RGroupingHeuristic<cGroup,cObj,cGroupData,cGroups>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rgga/rgroups.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
