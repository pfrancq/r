/*

	R Project Library

	RGeoInfoConnection.h

	Connection for the 2D Placement (RGeoInfo Part) - Header.

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
#ifndef RGeoInfoConnectionH
#define RGeoInfoConnectionH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rmath/rgraph.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rconnection.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RGeoInfoConnectionPart class provides a representation for an edge of a
* connection.
* @author Pascal Francq
* @short Connection's Part for Geometric Information
*/
class RGeoInfoConnectionPart
{
	struct sSearch
	{
		unsigned int id1;
		unsigned int id2;
	};

public:

	/**
	* Identificator of the first object.
	*/
	unsigned int Id1;

	/**
	* Position of the first connector.
	*/
	RPoint PosCon1;

	/**
	* Identificator of the second object.
	*/
	unsigned int Id2;

	/**
	* Position of the second connector.
	*/
	RPoint PosCon2;

	/**
	* Constructor of the connection part.
	*/
	RGeoInfoConnectionPart(const RPoint& pt1,unsigned int id1,const RPoint& pt2,unsigned int id2);

	/**
	* Copy contructor.
	*/
	RGeoInfoConnectionPart(const RGeoInfoConnectionPart* p);

	/**
	* Compare function.
	*/
	int Compare(const RGeoInfoConnectionPart* p)
		{return(!(((p->Id1==Id1)&&(p->Id2==Id2))||((p->Id1==Id2)&&(p->Id2==Id1))));}
		
	int Compare(const sSearch& s)
			{return(!(((s.id1==Id1)&&(s.id2==Id2))||((s.id1==Id2)&&(s.id2==Id1))));}

	// friend classes
	friend class RGeoInfoConnection;
};


//------------------------------------------------------------------------------
/**
* The RGeoInfoConnection class provides a representation for a connection
* between two geometric information.
* @author Pascal Francq
* @short Connection for Geometric Information.
*/
class RGeoInfoConnection : public RContainer<RGeoInfoConnectionPart,unsigned int,true,false>
{
public:
	/**
	* The connection.
	*/
	RConnection* Con;

	/**
	* Complete connection.
	*/
	bool Complete;

	/**
	* Actual total distance.
	*/
	double Dist;

	/**
	* Construct a connection.
	*/
	RGeoInfoConnection(RConnection* con);

	/**
	* Construct a connection.
	*/
	RGeoInfoConnection(const RGeoInfoConnection& con);

	/**
	* Construct a connection.
	*/
	RGeoInfoConnection(const RGeoInfoConnection* con);

	/**
	* Make this connection uncompleted.
	*/
	void UnComplete(void);

	/**
	* This function compares two connections returns 0 if there are the same.
	* @param c              Connection used for the comparaison.
	*/
	int Compare(const RGeoInfoConnection* c) {return(this-c);}

	/**
	* This function compares two connections returns 0 if there are the same.
	* @param c              Connection used for the comparaison.
	*/
	int Compare(const RGeoInfoConnection& c) {return(this-(&c));}

	/**
	* Verify that at least two objects of the connection are placed.
	*/
	bool MinObjPlaced(RGeoInfos* infos);

	/**
	* Compute the distance of the net for a given set of geometric information.
	* The "Prim's Algorithm for Minimum Spanning Trees" is used.
	* @param infos          Geometric informations.
	* @returns The "minimal" distance of the net.
	*/
	void ComputeMinDist(RGeoInfos* infos);

	/**
	* Return the distance of the nets. The geometric information cur must be
	* considered as placed at a gicen position.
	* @param infos          Geometric informations.
	* @param cur            The geometric information to place.
	* @param pos            Position where to placed the geometric information.
	*/
	double GetDist(RGeoInfos* infos,RGeoInfo* cur,const RPoint& pos);

	/**
	* Return the distance of the nets involving a given geometric information.
	* @param infos          Geometric informations.
	* @param cur            The geometric information to be involved.
	*/
	double GetDist(RGeoInfos* infos,RGeoInfo* cur);

	/**
	* Return true if a geometric information is in the connection.
	*/
	bool IsIn(const RGeoInfo* info);

	/**
	* Return true if a connector is in the connection.
	*/
	bool IsIn(const RGeoInfoConnector* con);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
