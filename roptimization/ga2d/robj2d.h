/*

	R Project Library

	RObj2D.h

	Object for 2D placement GA - Header

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RObj2D_H
#define RObj2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rpoint.h>
#include <rrect.h>
#include <rrects.h>
#include <rpolygon.h>
#include <rpolygons.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RGeoInfo;
class RGeoInfos;
class RObj2D;
class RConnection;


//------------------------------------------------------------------------------
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
	* Identifier of the connector.
	*/
	size_t Id;

	/**
	* Name of the connector.
	*/
	RString Name;

	/**
	* Number of position for the connector.
	*/
	size_t NbPos;

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
	RContainer<RConnection,false,false> Connections;

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param pos            Position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, size_t id,const RPoint pos);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param pos            Position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, size_t id,const RString& name,const RPoint pos);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param pos            Position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, size_t id,const char* name,const RPoint pos);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param x              X Coordinate of the position of the connector.
	* @param y              Y Coordinate of the position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, size_t id,tCoord x,tCoord y);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param x              X Coordinate of the position of the connector.
	* @param y              Y Coordinate of the position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, size_t id,const RString& name,tCoord x,tCoord y);


	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param x              X Coordinate of the position of the connector.
	* @param y              Y Coordinate of the position of the connector.
	*/
	RObj2DConnector(RObj2D* owner, size_t id,const char* name,tCoord x,tCoord y);

	/**
	* Construct a connector.
	* @param owner          Owner of the connector.
	* @param id             Id of the connector.
	* @param name           Name of the connector.
	* @param nb             Number of the connector.
	*/
	RObj2DConnector(RObj2D* owner, size_t id,const char* name,const size_t nb);

	/**
	* This function compares two connectors and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param c              Connector used for the comparison.
	*/
	int Compare(const RObj2DConnector* c) const {return(CompareIds(Id,c->Id));}

	/**
	* This function compares two connectors and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param c              Connector used for the comparison.
	*/
	int Compare(const RObj2DConnector& c) const {return(CompareIds(Id,c.Id));}

	/**
	* This function compares a connector and an identifier and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param id             Identifier used for the comparison.
	*/
	int Compare(const size_t id) const {return(CompareIds(Id,id));}

	/**
	* This function compares a connector and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparison.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* This function compares a connector and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparison.
	*/
	int Compare(const char* name) const {return(Name.Compare(name));}

	/**
	* Return the object that's own the connector.
	*/
	RObj2D* GetObj(void) {return(Owner);}

	/**
	* Return the identificator of the connector.
	*/
	size_t GetId(void) {return(Id);}

	/**
	* Return the position of the connector relative to the object.
	*/
	RPoint GetPos(void);

	/**
	* Return the position of the connector of the oth orientation.
	* @param i             The number.
	* @param o             The orientation.
	*/
	RPoint GetPos(size_t i,char o);

	/**
	* Add a connection to this connector.
	* @param con            Connection.
	*/
	void AddConnection(RConnection* con);

	/**
	* Return the name of the connector.
	*/
	RString GetName(void) {return(Name);}

	/**
	* Return the minimum distance between two connectors.
	* @param c              The second connector.
	* @param infos          The geometric information representing the placed objects.
	* @param pt1            The first connector point used.
	* @param pt2            The second connector point used.
	*/
	double GetMinDist(RObj2DConnector* c,RGeoInfos* infos,RPoint& pt1,RPoint& pt2);

	/**
	* Destruct the connectors
	*/
	virtual ~RObj2DConnector(void);

	// friend classes
	friend class RObj2D;
	friend class RGeoInfoConnector;
	friend class RConnections;
	friend class RProblem2D;
};



//------------------------------------------------------------------------------
/**
* This basic class represent an object to place by using the 2D placement GA.
* @author Pascal Francq
* @short 2D Object.
*/
class RObj2D
{
public:

	/**
	* Identifier of the object.
	*/
	size_t Id;

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
	tCoord Area;

	/**
	* Number of possible Orientations.
	*/
	int NbPossOri;

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
	RContainer<RObj2DConnector,true,true> Connectors;

	/**
	* Construct an 2D object.
	* @param id             The identifier of the object.
	* @param deformable     Specify if the object is deformable.
	*/
	RObj2D(size_t id,bool deformable);

	/**
	* Construct an 2D object.
	* @param id             Identifier of the object.
	* @param name           Name of the object.
	* @param deformable     Specify if the object is deformable.
	*/
	RObj2D(size_t id,const RString& name,bool deformable);

	/**
	* Construct an 2D object.
	* @param id             Identifier of the object.
	* @param name           Name of the object.
	* @param deformable     Specify if the object is deformable.
	*/
	RObj2D(size_t id,const char* name,bool deformable);

	/**
	* This function compares two objects and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param o              Object used for the comparison.
	*/
	int Compare(const RObj2D* o) const {if(IsIn(o->Id)) return(0); else return(CompareIds(Id,o->Id));}

	/**
	* This function compares two objects and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param o              Object used for the comparison.
	*/
	int Compare(const RObj2D& o) const {if(IsIn(o.Id)) return(0); else return(CompareIds(Id,o.Id));}

	/**
	* This function compares an object and an identifier and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param id             Identifier used for the comparison.
	*/
	int Compare(const size_t id) const {if(IsIn(id)) return(0); else return(CompareIds(Id,id));}

	/**
	* This function compares an object and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparison.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* This function compares an object and a name and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param name           Name used for the comparison.
	*/
	int Compare(const char* name) const {return(Name.Compare(name));}

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
	* @param o              Orientation.
	*/
	void SetOri(ROrientation o);

	/**
	* Return true if the orientation is possible.
	* @param o              Orientation.
	*/
	bool IsOriSet(ROrientation o);

	/**
	* Return the identifier of the object.
	*/
	inline size_t GetId(void) const {return(Id);}

	/**
	* Return the area of the object.
	*/
	inline tCoord GetArea(void) {return(Area);}

	/**
	* Return a pointer to the polygon representing the ith orientation.
	* @param i              Index of the orientation.
	* @return Pointer to a RPolygon.
	*/
	RPolygon* GetPolygon(int i);

	/**
	* Return a pointer to the rectangular decomposition of the ith orientation.
	* @param i              Index of the orientation.
	* @return pointer to a RRects.
	*/
	RRects* GetRects(int i);

	/**
	* The assignment operator.
	* @param obj            Source object.
	*/
	RObj2D& operator=(const RObj2D &obj);

	/**
	* Add a connector to this object
	* @param id             Identifier of the connector.
	* @param x              X-Coordinate of the connection point.
	* @param y              Y-Coordinate of the connection point.
	*/
	void AddConnector(size_t id,unsigned x,unsigned y);

	/**
	* Return the connector corresponding to a given identifier.
	* @param id             Identifier used for the search.
	*/
	RObj2DConnector* GetConnector(size_t id) {return(Connectors.GetPtr<size_t>(id));}

	/**
	* Copy the connectors from a given object.
	* @param obj            Source object.
	*/
	void CopyConnectors(RObj2D* obj);

	/**
	* Verify if a given object is in the container or not.
	* @param id             Identifier of the object.
	* @return true if the object is in, else false.
	*/
	virtual bool IsIn(size_t id) const {return(Id==id);}

	/**
	* Destruct the object.
	*/
	virtual ~RObj2D(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
