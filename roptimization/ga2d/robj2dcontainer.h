/*

	R Project Library

	RGeoInfo.h

	Geometric information - Header

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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
#ifndef RGeoInfoContainerH
#define RGeoInfoContainerH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rga2d/robj2d.h>
#include <rga2d/rgrid.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RGeoInfoContainer class provides a set of geometric information to be
* considered like one entity.
* @author Pascal Francq
* @short Geometric Information Container.
*/
class RObj2DContainer : public RObj2D, public RContainer<RGeoInfo,unsigned int,false,false>
{
	/**
	* Translation of the geometric information contained.
	*/
	RPoint Translation;

public:

	/**
	* Construct a geometric information.
	* @param id             The identificator of the object.
	* @param name           Name of the object.
	* @param nb             Number of geometric information that can be
	*                       contained.
	*/
	RObj2DContainer(const unsigned int id,const char* name,const unsigned int nb);

	/**
	* Clear the geometric information.
	*/
	virtual void ClearInfo(void);

	/**
	* Add a geometric information information to the container.
	* @param infos          Geometric Informations.
	* @param info           The geometric information of the object.
	*/
	void Add(RGeoInfos* infos,RGeoInfo *info);

	/**
	* Does some calculation after each geometric informartion were added to the
	* container.
	*/
	void Complete(void);

	/**
	* Assign the geometric information to the position and update the grids with the
	* identicator of the object.
	* @param infos          Geometric Informations.
	* @param pos            Position to place.
	* @param grid           Grid.
	* @param order          Order of the corresponding geometric information.
	*/
	virtual void Assign(RGeoInfos* infos,const RPoint &pos,RGrid *grid,const unsigned int order);

	/**
	* Verify if a given object is in the container or not.
	* @unsigned id          Identificator of the object.
	* @return true if the object is in, else false.
	*/
	virtual bool IsIn(unsigned int id);

	/**
	* Destruct the container.
	*/
	virtual ~RObj2DContainer(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
