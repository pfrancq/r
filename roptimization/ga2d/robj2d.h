/*

	R Project Library

	RObj2D.h

	Object for 2D placement GA - Header

	(C) 1999-2001 by P. Francq.

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
#ifndef RObj2DH
#define RObj2DH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
#include <rgeometry/rrect.h>
#include <rgeometry/rrects.h>
#include <rgeometry/rpolygon.h>
#include <rgeometry/rpolygons.h>
using namespace RGeometry2D;


//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RGeoInfo;
class RObj2D;
class RConnection;


//-----------------------------------------------------------------------------
/**
* This constance represent a non-identificator. It is used, for example, to
* specify that a variable containing an identificator of an object. is
* referencing no object.
*/
const unsigned int NoObject=UINT_MAX;



//-----------------------------------------------------------------------------
/**
* This basic class represent a connection point for an object to place by using
* the 2D placement GA.
* @author Pascal Francq
* @short Connector of a 2D Object.
*/
class RObj2DConnector
{
public:

	/**
	* Owner of the connector.
	*/
	RObj2D* Owner;
	
	/**
	* Identificator of the connector.
	*/
	unsigned int Id;

	/**
	* Name of the connector.
	*/
	RString Name;

	/**
	* Number of position for the connector.
	*/
	unsigned int NbPos;

	/**
	* Point representing the positions of the connector.
	*/
	RPoint* Pos;

	/**
	* Position of the connector for the possible orientations.
	*/
	RPoint** Poss;

	/**
	* Connections of which the connector is involved.
	*/
	RContainer<RConnection,unsigned int,false,false> Connections;

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param pos            Position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, unsigned int id,const RPoint pos);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param pos            Position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, unsigned int id,const RString& name,const RPoint pos);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param pos            Position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, unsigned int id,const char* name,const RPoint pos);

	/**	
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param x              X Coordinate of the position of the connector.
	* @param y              Y Coordinate of the position of the connector.	
	*/
	RObj2DConnector(RObj2D* owner, unsigned int id,const unsigned int x,unsigned y);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param x              X Coordinate of the position of the connector.
	* @param y              Y Coordinate of the position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, unsigned int id,const RString& name,const unsigned int x,unsigned y);

	
	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param x              X Coordinate of the position of the connector.
	* @param y              Y Coordinate of the position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, unsigned int id,const char* name,const unsigned int x,unsigned y);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param nb             Number of the connector.
	*/
	RObj2DConnector(RObj2D* owner, unsigned int id,const char* name,const unsigned int nb);

	/**
	* This function compares two connectors and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param c              Connector used for the comparaison.
	*/
	int Compare(const RObj2DConnector* c) {return(Id-c->Id);}

	/**
	* This function compares two connectors and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param c              Connector used for the comparaison.
	*/
	int Compare(const RObj2DConnector& c) {return(Id-c.Id);}

	/**
	* This function compares a connector and an identificator and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param id             Identificator used for the comparaison.
	*/
	int Compare(const unsigned int id) {return(Id-id);}
	
	/**
	* This function compares a connector and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparaison.
	*/
	int Compare(const RString& name) {return(Name.Compare(name));}

	/**
	* This function compares a connector and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparaison.
	*/
	int Compare(const char* name) {return(Name.Compare(name));}

	/**
	* Return the object that's own the connector.
	*/
	RObj2D* GetObj(void) {return(Owner);}

	/**
	* Return the identificator of the connector.
	*/
	unsigned int GetId(void) {return(Id);}

	/**
	* Return the position of the connector relativ to the object.
	*/
	RPoint& GetPos(void);

	/**
	* Return the position of the connector of the oth orientation.
	* @return i             The number.
	* @return o             The orientation.
	*/
	RPoint& GetPos(unsigned int i,char o);

	/**
	* Add a connection to this connector.
	*/
	void AddConnection(RConnection* con);

	/**
	* Return the name of the connector.
	*/
	const char* GetName(void) {return(Name());}

	/**
	* Return the minimum distance between two connectors.
	* @param c              The second connector.
	* @param infos          The goemetric information representing the placed objects.
	* @param pt1            The first connector point used.
	* @param pt2            The second connector point used.
	*/
	double GetMinDist(RObj2DConnector* c,RGeoInfo** infos,RPoint& pt1,RPoint& pt2);

	// friend classes
	friend class RObj2D;
	friend class RGeoInfoConnector;
	friend class RConnections;
	friend class RProblem2D;
};



//-----------------------------------------------------------------------------
/**
* This basic class represent an object to place by using the 2D placement GA.
* @author Pascal Francq
* @short 2D Object.
*/
class RObj2D
{
public:

	/**
	* Identificator of the object.
	*/
	unsigned int Id;

	/**
	* Name of the object.
	*/
	RString Name;

	/**
	* Polygon that define the object.
	*/
	RPolygon Polygon;

	/**
	* Represent the area of the object.
	*/
	RCoord Area;

	/**
	* Number of possible Orientations.
	*/
	char NbPossOri;

	/**
	* Different Orientations accepted.
	*/
	ROrientation PossOri[8];

	/**
	* Polygons for the possible orientations.
	*/
	RPolygon Polygons[8];

	/**
	* Rectangular decompositions for the possible orientations.
	*/
	RRects Rects[8];

	/**
	* Specify if the object is deformable or rigid.
	*/
	bool Deformable;

	/**
	* The connectors of this object
	*/
	RContainer<RObj2DConnector,unsigned int,true,true> Connectors;

	/**
	* Construct an 2D object.
	* @param id             The identificator of the object.
	* @param deformable     Specify if the object is deformable.
	*/
	RObj2D(unsigned int id,bool deformable);

	/**
	* Construct an 2D object.
	* @param id             Identificator of the object.
	* @param name           Name of the object.
	* @param deformable     Specify if the object is deformable.
	*/
	RObj2D(unsigned int id,const RString& name,bool deformable);

	/**
	* Construct an 2D object.
	* @param id             Identificator of the object.
	* @param name           Name of the object.
	* @param deformable     Specify if the object is deformable.
	*/
	RObj2D(unsigned int id,const char* name,bool deformable);

	/**
	* This function compares two cobjects and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param o              Object used for the comparaison.
	*/
	int Compare(const RObj2D* o) {return(Id-o->Id);}

	/**
	* This function compares two cobjects and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param o              Object used for the comparaison.
	*/
	int Compare(const RObj2D& o) {return(Id-o.Id);}

	/**
	* This function compares an object and an identificator and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param id             Identificator used for the comparaison.
	*/
	int Compare(const unsigned int id) {return(Id-id);}

	/**
	* This function compares an object and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparaison.
	*/
	int Compare(const RString& name) {return(Name.Compare(name));}

	/**
	* This function compares an object and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparaison.
	*/
	int Compare(const char* name) {return(Name.Compare(name));}

	/**
	* Initialize the object when all information are entered. In particular,
	* it calculates the different polygon based on the possible orientations,
	* the rectangular decompositions and the area of the object.
	*/
	void Init(void);

	/**
	* Calculate all the polygons based on the possible orientations of the object.
	* @return The function returns true if the calculation has be done without errors.
	*/
	void CalcPolygons(void);

	/**
	* Set a specific orientation as possible.
	*/
	void SetOri(ROrientation o);

	/**
	* Return true if the orientation is possible.
	*/
	bool IsOriSet(ROrientation o);

	/**
	* Return the identificator of the object.
	*/
	inline unsigned int GetId(void) const {return(Id);}

	/**
	* Return the area of the object.
	*/
	inline RCoord GetArea(void) {return(Area);}

	/**
	* Return a pointer to the polygon representing the ith orientation.
	*/
	RPolygon* GetPolygon(char i);

	/**
	* Return a pointer to the rectangular decomposition of the ith orientation.
	*/
	RRects* GetRects(char i);

	/**
	* The assignment operator.
	*/
	RObj2D& operator=(const RObj2D &obj);

	/**
	* Add a connector to this object
	* @param id             Identificator of the connector.
	* @param x              X-Coordinate of the connection point.
	* @param y              Y-Coordinate of the connection point.	
	*/
	void AddConnector(unsigned int id,unsigned x,unsigned y);

	/**
	* Return the connector corresponding to a given identificator.
	* @param id             Identificator used for the search.
	*/
	RObj2DConnector* GetConnector(unsigned int id) {return(Connectors.GetPtr<unsigned int>(id));}

	/**
	* Copy the connectors from a given object.
	*/
	void CopyConnectors(RObj2D* o);
};


//-----------------------------------------------------------------------------
/**
* This class represent an object containg other objects to place. It is use,
* for example, in the 2D placement GA crossover.
* @author Pascal Francq
* @short Container of objects.
*/
class RObj2DContainer : public RObj2D
{
public:

	/**
	* Identificators of the objects contained.
	*/
	unsigned int *Ids;

	/**
	* Pointer to all geometric infos of the objects.
	*/
	RGeoInfo** Infos;

	/**
	* Maximum number of objects contained.
	*/
	unsigned int NbMax;

	/**
	* Number of objects contained.
	*/
	unsigned int Nb;

	/**
	* Polygons of the objects.
	*/
	RPolygons SPolygons;

	/**
	* Point the most left-bottom.
	*/
	RCoord MinX,MinY;

	/**
	* Construct the container of objects.
	* @param id             The identificator of the object.
	* @param max            Maxmimum number of objects that can be contained.
	*/
	RObj2DContainer(unsigned int id,unsigned int max);

	/**
	* Clear the container.
	*/
	void Clear(void);

	/**
	* Add an object and his information to the container.
	* @param obj            The object to add.
	* @param info           The geometric information of the object.
	*/
	void AddObj(RObj2D *obj,RGeoInfo *info);

	/**
	* Does some calculation after each objects were added to the container.
	*/
	void EndObjs(void);

	/**
	* Assign this container to a position and replace it in the grid with all the
	* identificators of the objects contained.
	* @param pos            Position.
	* @param infos          The geometric information of the objects.
	* @param OccY           Grid with X as entry.
	* @param OccY           Grid with Y as entry.
	*/
	void Assign(RPoint &pos,RGeoInfo **infos,unsigned int **OccX,unsigned int **OccY);

	/**
	* Destruct the container.
	*/
	~RObj2DContainer(void);
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
