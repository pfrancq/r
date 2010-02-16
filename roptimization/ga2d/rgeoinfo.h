/*

	R Project Library

	RGeoInfo.h

	Geometric information - Header

	Copyright 1998-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef RGeoInfo_H
#define RGeoInfo_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rpoint.h>
#include <rsize.h>
#include <rrect.h>
#include <rpolygon.h>
#include <rstd.h>
#include <robj2d.h>
#include <robj2dconfig.h>
#include <rgrid.h>
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RObj2D;
class RGrid;
class RGeoInfo;


//------------------------------------------------------------------------------
/**
 * The RGeoInfoPin provides a representation for a particular pin of a specific
 * geometric information. It represents a connection point used by a "real"
 * connection.
 * @author Pascal Francq
 * @short Connection Point of a Geometric Information.
 */
class RGeoInfoPin
{
	/**
	 * The pin.
	 */
	RObj2DConfigPin* Pin;

	/**
	 * The geometric information.
	 */
	RGeoInfo* Info;

public:

	/**
	 * Constructor of a particular position of a pin.
	 * @param pin            Pin.
	 * @param info           Geometric information.
	 */
	RGeoInfoPin(RObj2DConfigPin* pin,RGeoInfo* info);

	/**
	 * Compare two pin instances. The pin are sorted in order to represent
	 * the path of a connection : The first pin is always the most left one and
	 * the most at the top. The last pin is always the most right one and the
	 * most at the bottom.
	 * @param pin            Pin used for the comparison.
	 * @return a value usable by RContainer.
	 */
	int Compare(const RGeoInfoPin& pin) const;

	/**
	 * @return the position of the pin.
	 */
	RPoint GetPos(void) const;

	/**
	 * @return return the pin.
	 */
	inline RObj2DConfigPin* GetPin(void) const {return(Pin);}

	/**
	 * @return return the geometric information.
	 */
	inline RGeoInfo* GetInfo(void) const {return(Info);}

};


//------------------------------------------------------------------------------
/**
* The RGeoInfoCursor class provides a way to go trough a set of points with a
* base point. In particular, it is adapted to parse a geometric information.
* @short Relative Points Cursor
*/
class RGeoInfoCursor : private RCursor<RPoint>
{
	/**
	* Base point.
	*/
	RPoint Base;

public:

	/**
	* Construct the cursor.
	*/
	RGeoInfoCursor(void);

	/**
	* Construct the cursor.
	* param info             Geometric information to parser.
	*/
	RGeoInfoCursor(RGeoInfo& info);

	/**
	* Assignment operator using a "Cursor".
	*/
	RGeoInfoCursor& operator=(const RGeoInfoCursor& c);

	/**
	* Set the geometric information.
	* param info             Geometric information to parser.
	*/
	void Set(RGeoInfo& info);

	/**
	 * Start the cursor.
	 */
	inline void Start(void) {RCursor<RPoint>::Start();}

	/**
	 * Go to the next element of the cursor.
	 */
	inline void Next(void) {RCursor<RPoint>::Next();}

	/**
	 * @return true if the cursor has reached the end.
	 */
	inline bool End(void) const {return(RCursor<RPoint>::End());}

	/**
	* Return the current element.
	*/
	RPoint operator()(void) const;
};


//------------------------------------------------------------------------------
/**
* The RGeoInfo class provides a geometric information concerning the placement
* of an object.
* @author Pascal Francq
* @short Geometric Information.
*/
class RGeoInfo
{
protected:

	/**
	* Pointer to the the object.
	*/
	RObj2D* Obj;

	/**
	 * The particular configuration of the object used.
	 */
	RObj2DConfig* Config;

	/**
	* Position of the object.
	*/
	RPoint Pos;

	/**
	* Indicate the container of the object (if any). This is used to aggregate
	* a set of objects.
	*/
	RObj2DContainer* Container;

	/**
	* Order of the geometric information.
	*/
	size_t Order;

	/**
	* Construct a geometric information.
	*/
	RGeoInfo(void);

public:

	/**
	* Construct a geometric information.
	* @param obj             Object represented.
	*/
	RGeoInfo(RObj2D* obj);

	/**
	* Construct a geometric information from another one.
	*/
	RGeoInfo(const RGeoInfo& info);

	/**
	* Compare function use for the RContainer class. Compare only the addresses of the
	* pointers.
	* @param info           Pointer used for the comparison.
	*/
	int Compare(const RGeoInfo& info) const;

	/**
	* Compare function use for the RContainer class. Compare the identifier
	* with the one of the corresponding object.
	* @param id             Identifier used for the comparison.
	*/
	int Compare(const size_t id) const { return(Obj->Compare(id)); }

	/**
	* Clears the geometric information.
	*/
	virtual void ClearInfo(void);

	/**
	* Area used by the polygon.
	*/
	tCoord GetArea(void) const;

	/**
	* Calculate the boundary rectangle of the polygon according to the position.
	* @param rect           Rectangle that will hold the result.
	*/
	void Boundary(RRect& rect);

	/**
	* @return a pointer to the container or null if the object is not
	* aggregated.
	*/
	inline RObj2DContainer* GetContainer(void) const {return(Container);}

	/**
	* Set the container that hold the object.
	* @param container       Container.
	*/
	void SetContainer(RObj2DContainer* container);

	/**
	* Return true if the geometric information is a valid one.
	*/
	inline bool IsValid(void) const {return(Pos.IsValid());}

	/**
	* Return the object.
	*/
	RObj2D* GetObj(void) const {return(Obj);}

	/**
	 * Set the configuration used.
	 * @param ori            Orientation.
	 */
	void SetConfig(tOrientation ori);

	/**
	* Return the configuration used.
	*/
	RObj2DConfig* GetConfig(void) const {return(Config);}

	/**
	* Assign the geometric information to the position and update the grids with the
	* identifier of the object.
	* @param pos            Position to place.
	* @param grid           Grid.
	* @param order          Order of the corresponding geometric information.
	*/
	virtual void Assign(const RPoint& pos,RGrid* grid,size_t order);

	/**
	* Return true if the geometric information is a valid one in given limits.
	* @param pos            Position to verify.
	* @param limits         Limits.
	*/
	bool IsValid(const RPoint& pos,const RSize& limits) const;

	/**
	* Test if the shape of the object can be placed at a specific position
	* based on a given grid.
	* @param pos            Position to test.
	* @param grid           Grid.
	*/
	bool Test(RPoint& pos,RGrid* grid);

	/**
	* Calculate the position where the object represented can be placed, if it
	* is pushed to the bottom and to the left.
	* @param pos            Initial position, and then the final one.
	* @param limits         Limits for the placement.
	* @param grid           Grid.
	*/
	void PushBottomLeft(RPoint& pos,const RSize& limits,RGrid* grid);

	/**
	* Calculate the position where the object represented can be placed, if it
	* is pushed in direction of the center.
	* @param pos            Initial position, and then the final one.
	* @param limits         Limits for the placement.
	* @param grid           Grid.
	*/
	void PushCenter(RPoint& pos,const RSize& limits,RGrid* grid);

	/**
	* This function returns true if the two objects represented by this geometric information
	* overlap (Rects have to be calculated.
	*/
	bool Overlap(RGeoInfo* info) const;

	/**
	* Return the number of points contained in the polygon.
	*/
	inline size_t GetNbPoints(void) const {return(Config->GetPolygon().GetNb());}

	/**
	* @return the position of the geometric information.
	*/
	RPoint GetPos(void) const;

	/**
	* Make a translation of the point.
	* @param pt             Point representing the vector used.
	*/
	RGeoInfo& operator+=(const RPoint& pt) {Pos+=pt;return(*this);}

	/**
	* Make a translation of the point.
	* @param pt             The point representing the vector used.
	*/
	RGeoInfo& operator-=(const RPoint& pt)
	{
		Pos-=pt;
		return(*this);
	}

	/**
	* Assignment Operator.
	*/
	RGeoInfo& operator=(const RGeoInfo& info);

	/**
	* Return true if a point is in the polygon.
	*/
	bool IsIn(RPoint pos) const;

	/**
	* Add the polygon representing the object in the container of polygons.
	*/
	void Add(RPolygons& polys);

	/**
	* Return the polygon representing the object placed.
	*/
	RPolygon GetPlacedPolygon(void) const;

	/**
	* Get the order.
	*/
	size_t GetOrder(void) const {return(Order);}

	/**
	* Destruct the geometric information.
	*/
	virtual ~RGeoInfo(void);

	// friend classes
	friend class RRelPointCursor;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
