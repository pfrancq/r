/*

	R Project Library

	RGeoInfo.h

	Geometric information - Header

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
#ifndef RGeoInfoH
#define RGeoInfoH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
#include <rgeometry/rrect.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;
#include <rga2d/robj2d.h>
#include <rga2d/rgrid.h>
using namespace RGA2D;


//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RObj2D;
class RGrid;
class RGeoInfo;



//-----------------------------------------------------------------------------
/**
* The RGeoInfoConnector class provides a connector of a geometric information
* concerning the placement of an object.
* @author Pascal Francq
* @short Connector of a Geometric Information.
*/
class RGeoInfoConnector
{
	/**
	* Pointer to the real connector.
	*/
	RObj2DConnector* Con;

	/**
	* Owner of the connector.
	*/
	RGeoInfo* Owner;

	/**
	* Number of position for the connector.
	*/
	unsigned int NbPos;

	/**
	* Positions of the connector in the geometric information.
	*/
	RPoint* Pos;

public:

	/**
	* Constructor of the connector.
	* @param con		"Real" Connector of this one.
	* @param owner		Geometric Information of the connector.
	*/	
	RGeoInfoConnector(RObj2DConnector *con,RGeoInfo* owner);
	
	/**
	* Constructor of the connector.
	* @param con		"Real" Connector of this one.
	* @param owner		Geometric Information of the connector.
	* @param pos		The position of the connector
	*/	
	RGeoInfoConnector(RObj2DConnector *con,RGeoInfo* owner,const RPoint& pos);
	
		
	/**
	* This function compares two connectors and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param c		Connector used for the comparaison.
	*/
	int Compare(const RGeoInfoConnector* c) {return(Con->Id-c->Con->Id);}
	
	/**	
	* This function compares two connectors and returns 0 if there are the same.
	* This function is used for the class RContainer.
	* @param c		Connector used for the comparaison.
	*/
	int Compare(const RGeoInfoConnector& c) {return(Con->Id-c.Con->Id);}
	
	/**
	* This function compares a connector and an identificator and returns 0 if
	* there are the same.
	* This function is used for the class RContainer.
	* @param id		Identificator used for the comparaison.
	*/
	int Compare(const unsigned int id) {return(Con->Id-id);}
		
	/**
	* Return the position of the connector in absolute (not relativ to the
	* object).
	*/
	RPoint& GetPos(void);
	
	// friend classes
	friend class RGeoInfo;
	friend class RConnections;
};



//-----------------------------------------------------------------------------
/**
* \ingroup 2DGA
* The RGeoInfo class provides a geometric information concerning the placement
* of an object.
* @author Pascal Francq
* @short Geometric Information.
*/
class RGeoInfo
{
	/**
	* Pointer to the the object.
	*/
	RObj2D *Obj;

	/**
	* Indicate if the object was selected for the crossover.
	*/
	bool Selected;

	/**
	* Position of the object.
	*/
	RPoint Pos;    			

	/**
	* Index of the orientation used for this info.
	*/
	char Ori;

	/**
	* Polygon representing the object.
	*/
	RPolygon *Bound;			

	/**
	* Rectangles Decomposition of the polygon.
	*/
	RRects *Rects;				

	/**
	* Rectangle holding the polygon.
	*/
	RRect Rect;

public:
	
	/**
	* The connectors of this object
	*/
	RContainer<RGeoInfoConnector,unsigned int,true,true> Connectors;
	
	/**
	* Construct a geometric information.
	*/
	RGeoInfo(void);
	
	/**
	* Construct a geometric information.
	*/
	RGeoInfo(RPolygon* poly);
		
	/**
	* Construct a geometric information.
	*/
	RGeoInfo(RObj2D *obj);
	
	/**
	* Construct a geometric information from another one.
	*/
	RGeoInfo(RGeoInfo& info);
	
	/**
	* Construct a geometric information from another one.
	*/
	RGeoInfo(RGeoInfo *info);

	/**
	* Clears the geometric information.
	*/
	void Clear(void);

	/**
	* Return the index of the orientation.
	*/
	inline char GetOri(void) {return(Ori);}

	/**
	* Set to ith Orienation of the object.
	*/
	void SetOri(char i);

	/**
	* Area used by the polygon.
	*/
	RCoord GetArea(void);

	/**
	* Calculate the boundary rectangle of the polygon according to the position.
	* @param rect 	The rectangle that will hold the result.
	*/
	void Boundary(RRect &rect);

	/**
	* Return true if the geometric information is a valid one.
	*/	
	bool IsValid(void) const;

	/**
	* Return true if the object represented by the geometric information is
	* selected.
	*/
	bool IsSelect(void) const {return(Selected);}

	/**
	* Set that the object represented by the geometric information is selected.
	*/
	void SetSelect(void) {Selected=true;}

	/**
	* Return the object.
	*/
	RObj2D* GetObj(void) const {return(Obj);}

	/**
	* Assign the geometric information to the position and update the grids with the
	* identicator of the object.
	* @param pos				Position to place.
	* @param grid				The grid.
	*/
	void Assign(const RPoint &pos,RGrid *grid);
	
	/**
	* Assign the geometric information to the position.
	* @param pos				Position to place.
	*/
	void Assign(const RPoint &pos) {Pos=pos;}

	/**
	* Compare function use for the RContainer class. Compare only the adress of the
	* pointers.
	* @param info			The pointer used for the comparaison.
	*/
	int Compare(RGeoInfo *info) { return(info!=this); }

	/**
	* Test if the object can be placed at a specific position in regards of the
	* occupation.
	* @param pos				The position to test.
	* @param limits    	The limits for the placement.
	* @param grid				Grid.
	*/
	bool Test(RPoint &pos,RPoint &limits,RGrid *grid);

	/**
	* Calculate the position where the object represented can be placed, if it
	* is pushed to the bottom and to the left.
	* @param pos				The initial position, and then the final one.
	* @param limits			The limits for the placement.
	* @param grid				Grid.
	*/
	void PushBottomLeft(RPoint &pos,RPoint &limits,RGrid *grid);

	/**
	* Calculate the position where the object represented can be placed, if it
	* is pushed in direction of the center.
	* @param pos				The initial position, and then the final one.
	* @param limits	    The limits for the placement.
	* @param grid				Grid.
	*/
	void PushCenter(RPoint &pos,RPoint &limits,RGrid *grid);
	
	/**
	* This function returns true if the two objects represented by this geometric information
	* overlap (Rects have to be calculated.*/
	bool Overlap(RGeoInfo *info);

	/**
	* Return the Width of the object represented.
	*/
	inline RCoord Width(void) {return(Rect.Width());}

	/**
	* Return the Height of the object represented.
	*/
	inline RCoord Height(void) {return(Rect.Height());}

	/**
	* Return the number of points contained in the polygon.
	*/
	inline unsigned int NbPoints(void) {return(Bound->NbPtr);}

	/**
	* Return the position of the geometric information.
	*/
	RPoint& GetPos(void);

	/**
	* Make a translation of the point.
	* @param pt 	The point representing the vector used.
	*/
	RGeoInfo& operator+=(const RPoint &pt) {Pos+=pt;return(*this);}

	/**
	* Make a translation of the point.
	* @param pt 	The point representing the vector used.
	*/
	RGeoInfo& operator-=(const RPoint &pt)
	{
		Pos-=pt;
		return(*this);
	}

	/**
	* Start the iterator to go trough the points.
	*/
	inline void Start(void) {Bound->Start();}

	/**
	* Test if the end of the points is reached.
	*/
	inline bool End(void) {return(Bound->End());}

	/**
	* Goto the next point, if the end is reached, go to the beginning.
	*/
	inline void Next(void) {Bound->Next();}

	/**
	* Return the current point.
	*/
	RPoint& operator()(void);

	/**
	* Assignment Operator.
	*/
	RGeoInfo& operator=(const RGeoInfo &info);

	/**
	* Return true if a point is in the polygon.
	*/
	bool IsIn(RPoint pos);

	/**
	* Add the polygon representing the object in the container of polygons.
	*/
	void Add(RPolygons &polys);

	/**
	* Return the polygon representing the object placed.
	*/
	RPolygon& GetPolygon(void);

	/**
	* Return the polygon representing the object to place.
	*/
	RPolygon* GetBound(void) {return(Bound);}
	
	/**
	* Start the iterator to go through the connections.
	*/
	void StartCon(void);
	
	/**
	* Test if the end of the connections are reached.
	*/
	bool EndCon(void);
	
		
	/**
	* Go to the next connections, if the end is reached, go to the beginning.
	*/
	void NextCon(void);
	
	/**
	* Return the current connection.
	*/
	RConnection* GetCurrentCon(void);
	
	// friend classes
	friend class RGeoInfos;
};


//-----------------------------------------------------------------------------
/**
* The RGeoInfoContainer class provides a set of geometric information to be
* considered like one entity.
* @author Pascal Francq
* @short Geometric Information Container.
*/
class RGeoInfoContainer : public RGeoInfo
{
public:	
	
	/**
	* Construct a geometric information.
	*/
	RGeoInfoContainer(void);	
	
	/**
	* Construct a geometric information.
	*/
	RGeoInfoContainer(RObj2D* obj);
		
	/**
	* Add a geometric information information to the container.
	* @param info		The geometric information of the object.
	*/
	void Add(RGeoInfo *info);

	/**
	* Does some calculation after each geometric informartion were added to the
	* container.
	*/
	void End(void);
};


}  //-------- End of namespace RGA2D ------------------------------------------


//-----------------------------------------------------------------------------
#endif
