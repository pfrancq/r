/*

	RGroups.h

	Groups - Header

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
#ifndef RGroupsH
#define RGroupsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rga/robjs.h>
#include <rgga/rgga.h>
#include <rgga/rgroup.h>


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RGroups class provides a representation for a set of groups that have to
* contain objects.
* @author Pascal Francq
* @short Groups.
*/
template<class cGroup,class cObj,class cGroupData>
	class RGroups : public RStd::RContainer<cGroup,unsigned int,true,false>
{
public:

	/**
	*  Groups used.
	*/
	RStd::RContainer<cGroup,unsigned int,false,false> Used;

	/**
	* Data used to construct a group.
	*/
	cGroupData* GroupData;

protected:

	/**
	* The Objects to group.
	*/
	RGA::RObjs<cObj>* Objs;

	/**
	* Assignment of the objects.
	*/
	unsigned int* ObjectsAss;

	/**
	* Container of objects already assigned.
	*/
	RStd::RContainer<cObj,unsigned int,false,false> ObjsAss;

	/**
	* Container of objects not assigned.
	*/
	RStd::RContainer<cObj,unsigned int,false,false> ObjsNoAss;

public:

	/**
	* Construct the groups.
	* @param objs           Objects to group.
	* @param max            Maximal number of groups to create.
	*/
	RGroups(RGA::RObjs<cObj>* objs,const unsigned int max) throw(bad_alloc);

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
	* Reserve a Node.
	* @returns A pointer to the node reserved.
	*/
	cGroup* ReserveGroup(void);

	/**
	* Release a node.
	* @param node           Identificator of the group.
	*/
	void ReleaseGroup(const unsigned int group);

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
	virtual bool Verify(void);

	/**
	* Return a pointer of the group of an object.
	* @param id             Identificator of the object.
	*/
	cGroup* GetGroup(const unsigned int id) {return(Tab[ObjectsAss[obj]]);}

	/**
	* Return a pointer to an object at a given position of a group.
	*/
	cObj* GetObj(const unsigned int idx) {return(ObjsAss.Tab[idx]);}

	/**
	* The assigment operator.
	* @param chromo         The chromosome used as source.
	*/
	RGroups& operator=(const RGroups& grps);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RGroups(void);

	// friend classes
	friend class RGroup<cGroup,cObj,cGroupData>;
	friend class RGroupingHeuristic<cGroup,cObj,cGroupData>;
};


//-----------------------------------------------------------------------------
// inline implementation
#include <rgga/rgroup.hh>
#include <rgga/rgroups.hh>


}//------- End of namespace RGGA ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
