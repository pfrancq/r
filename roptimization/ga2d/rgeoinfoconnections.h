/*

	R Project Library

	RGeoInfoConnections.h

	Connections for the 2D Placement (Geometric Information Part) - Header.

	Copyright 2000-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RGeoInfoConnections_H
#define RGeoInfoConnections_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga2d/rconnections.h>
#include <rga2d/rgeoinfoconnection.h>
#include <rga2d/rgeoinfo.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RConnections class provides a representation for the connections of the
* problem.
* @short Connections.
* @author Pascal Francq
*/
class RGeoInfoConnections : public RContainer<RGeoInfoConnection,unsigned int,true,false>
{
	/**
	* Pointer to the connections.
	*/
	RConnections* Cons;

	/**
	* Pointer to the geometric information.
	*/
	RGeoInfos* Infos;

public:

	/**
	* Construct the connections.
	* @param o              General connections.
	* @param i              Geometric information.
	*/
	RGeoInfoConnections(RConnections* c,RGeoInfos* i);

	/**
	* Calculate the total length of all connections for the placed objects.
	* @param info           Current information to place.
	* @param pos            Position where to place.
	*/
	double GetDistances(RGeoInfo* info,const RPoint& pos);

	/**
	* Calculate the total length of all connections involving a give geometric
	* information.
	* @param info           Geometric information to be involved.
	*/
	double GetDistances(RGeoInfo* info);

	/**
	* Return the two object that have the most connected objects and that
	* are in a given rectangle and that are not selected.
	* @param i1             Pointer to the first object.
	* @param i2             Pointer to the second object.
	* @param bound          Rectangle.
	* @param selected       Array representing selected objects.
	*/
	void GetBestsConnected(RGeoInfo* (&i1),RGeoInfo* (&i2),const RRect& bound,const bool* selected);

	/**
	* Make all connections uncompleted.
	*/
	void UnComplete(void);

	/**
	* The assignement operator.
	*/
	RGeoInfoConnections& operator=(const RGeoInfoConnections& cons) throw(std::bad_alloc);

	// friend classes
	friend class RConnections;
	friend class RGeoInfos;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
