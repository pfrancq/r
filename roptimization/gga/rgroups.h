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


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template<class cGroup>
	class RGroups : public RStd::RContainer<cGroup,unsigned int,true,false>
{
protected:

	/**
	*  Groups used.
	*/
	RStd::RContainer<cGroup,unsigned int,false,false> Used;

public:

	/**
	* Construct the groups.
	* @param max            Maximal number of groups.
	*/
	RGroups(const unsigned int max) throw(bad_alloc);

	/**
	* Clear all the information of the chromosome.
	*/
	void Clear(void);

	/**
	* Reserve a Node.
	* @returns A pointer to the node reserved.
	*/
	cGroup* ReserveGroup(void);

	/**
	* Release a node.
	* @param node           Identificator of the node.
	*/
	void ReleaseGroup(const unsigned int group);

	/**
	* Verify the validity of the chromosome.
	* @return True if the chromosome is a valid one, false else.
	*/
	virtual bool Verify(void);

	/**
	* The assigment operator.
	* @param chromo         The chromosome used as source.
	*/
	RGroups& operator=(const RGroups& grps);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RGroups(void);
};


}//------- End of namespace RGGA ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
