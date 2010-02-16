/*

	R Project Library

	RGroups.h

	Groups - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef RGroups_H
#define RGroups_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rgga.h>
#include <rgroup.h>


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
template<class cGroup,class cObj,class cGroups>
	class RGroups : public RContainer<cGroup,true,false>
{
public:

	using RContainer<cGroup,true,false>::GetMaxNb;
	using RContainer<cGroup,true,false>::GetIncNb;
	using RContainer<cGroup,true,false>::VerifyTab;

private:

	// Internal class used to compute the adjusted Rand Index
	class GroupId
	{
	public:
		int GrpId;       // Group Identifier.
		int position;    // Position of the group.

		// Constructor and Compare methods.
		GroupId(int RealId,int Position) : GrpId(RealId), position(Position) {}
		int Compare(const int ID) const {return(GrpId-ID);}
		int Compare(const GroupId& grp) const {return(GrpId-grp.GrpId);}
	};

public:

	/**
	* Groups used.
	*/
	RContainer<cGroup,false,false> Used;

protected:

	/**
	* The Objects to group.
	*/
	RCursor<cObj> Objs;

	/**
	* Assignment of the objects.
	*/
	size_t* ObjectsAss;

	/**
	* Container of objects already assigned.
	*/
	RContainer<cObj,false,false> ObjsAss;

	/**
	* Container of objects not assigned.
	*/
	RContainer<cObj,false,false> ObjsNoAss;

	/**
	* Assignment of the objects where the identifier of the first group
	* found is always 1. This array is used to compare if two solutions are
	* representing the same "real" clustering.
	*/
	size_t* OrdObjectsAss;

	/**
	* Array representing the identifiers assigned to each group to compute
	* OrdObjectsAss.
	*/
	size_t* NewUsedId;

public:

	/**
	* Construct the groups.
	* @param objs           Objects to group.
	* @param max            Maximal number of groups to create.
	*/
	RGroups(RCursor<cObj> objs,const size_t max);

	/**
	* Init the groups.
	*/
	void Init(void);

	/**
	* Clear all the information of the chromosome.
	*/
	void ClearGroups(void);

private:

	void VerifyGroups(size_t id);

public:

	/**
	* Reserve a group.
	* @param id              Identifier of the group to create. If cNoRef is
	*                        passed, the first free group is chosen.
	* @returns A pointer to the node reserved.
	*/
	cGroup* ReserveGroup(size_t id=cNoRef);

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
	* @param grp            Pointer to the group.
	*/
	void DeleteObjs(cGroup* grp);

	/**
	* Verify the validity of the groups.
	* @return True if the groups are valid one, false else.
	*/
	virtual void Verify(void);

	/**
	* Get the group of an object.
	* @param id             Identifier of the object.
	* @returns Pointer to cGroup*.
	*/
	cGroup* GetGroup(size_t id) const;

	/**
	* Get the group of an object.
	* @param obj            Pointer to the object.
	* @returns Pointer to cGroup*.
	*/
	cGroup* GetGroup(const cObj* obj) const;

	/**
	* Get a pointer to an object at a given index.
	* @param idx            Global Index.
	* @returns cObj*
	*/
	cObj* GetObj(size_t idx) const {return(const_cast<RContainer<cObj,false,false>&>(ObjsAss)[idx]);}

	/**
	* Return a cursor over the objects of a group. The cursor cannot iterate
	* after the last object.
	* @param grp            Group.
	*/
	RCursor<cObj> GetObjs(const RGroup<cGroup,cObj,cGroups>& grp) const;

	/**
	* The assignment operator.
	* @param grps           Source groups.
	*/
	void CopyGrouping(const RGroups& grps);

	/**
	* Compute OrdObjectsAss.
	*/
	void ComputeOrd(void);

	/**
	* This method look if the two configurations are representing the same
	* clustering.
	* @param grps           Configuration used for the comparison.
	*/
	bool SameGroupment(const RGroups* grps) const;

	/**
	* Get a cursor over the objects already assigned.
	*/
	RCursor<cObj> GetObjsCursor(void) const;

	/**
	* Method to compute the adjusted Rand index with another clustering in
	* order to determine a similarity between two clusterings. The results is
	* between -1 (nothing in common) and +1 (identical).
	*
	* This method can be used to verify if two clusterings are similar.
	* @param groups          Groups to compare.
	* @return Adjusted Rand Index.
	*/
	double ComputeAdjustedRandIndex(const cGroups& groups) const;

	/**
	* Destruct the groups.
	*/
	virtual ~RGroups(void);

	// friend classes
	friend class RGroup<cGroup,cObj,cGroups>;
	friend class RGroupingHeuristic<cGroup,cObj,cGroups>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rgroups.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
