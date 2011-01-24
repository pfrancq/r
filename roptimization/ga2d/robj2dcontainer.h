/*

	R Project Library

	RGeoInfo.h

	Geometric information - Header

	Copyright 1998-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef RGeoInfoContainer_H
#define RGeoInfoContainer_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rgeoinfo.h>
#include <rgrid.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RGeoInfoContainer class provides an aggregator of geometric information
* to be considered like one entity.
* @author Pascal Francq
* @short Geometric Information Aggregator.
*/
class RObj2DContainer : public RGeoInfo, private RContainer<RGeoInfo,false,false>
{
	/**
	* Origin of the aggregator (Left-bottom).
	*/
	RPoint Origin;

	/**
	 * Layout associated to the container.
	 */
	RLayout* Layout;

public:

	/**
	* Construct an aggregator of geometric information relative to a given
	* layout.
	* @param id             The identifier of the object.
	* @param layout          Layout.
	*/
	RObj2DContainer(const size_t id,RLayout* layout);

	/**
	 * @return the number of objects contained.
	 */
	inline size_t GetNbObjs(void) const {return(RContainer<RGeoInfo,false,false>::GetNb());}

	/**
	* Clear the geometric information.
	*/
	virtual void ClearInfo(void);

	/**
	* Add a geometric information information to the container.
	* @param info           The geometric information.
	*/
	void Add(RGeoInfo *info);

	/**
	* Does some calculation after each geometric information were added to the
	* container.
	*/
	void Complete(void);

	/**
	* Assign the geometric information to the position and update the grid with the
	* identifiers of the objects.
	* @param pos            Position to place.
	* @param grid           Grid.
	* @param order          Order of the corresponding geometric information.
	*/
	virtual void Assign(const RPoint& pos,RGrid* grid,size_t order);

	/**
	* Verify if a given object is in the container or not.
	* @param id             Identifier of the object.
	* @return true if the object is in, else false.
	*/
	virtual bool IsIn(size_t id) const;

	/**
	* Destruct the container.
	*/
	virtual ~RObj2DContainer(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
