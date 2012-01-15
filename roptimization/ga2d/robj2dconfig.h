/*

	R Project Library

	RObj2DConfig.h

	Configuration of an object for 2D placement GA - Header

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
#ifndef RObj2DConfig_H
#define RObj2DConfig_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga2d.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RObj2DPinConfig represents a possible configuration of a pin for a
 * particular connector.
 * @author Pascal Francq.
 * @short Configuration of a Pin.
 */
class RObj2DConfigPin
{
	/**
	 * The corresponding pin.
	 */
	RObj2DPin* Pin;

	/**
	 * Rectangle representing the pin.
	 */
	RRect Rect;

	/**
	 * Position of the pin.
	 */
	RPoint Pos;

public:

	/**
	 * Constructor of a particular configuration of a pin.
	 * @param pin            Pin.
	 * @param rect           Rectangle representing the pin.
	 */
	RObj2DConfigPin(RObj2DPin* pin,const RRect& rect);

	/**
	 * Compare two pin configuration.
	 * @param pin            Pin to compare with.
	 * @return a value used by RContainer.
	 */
	int Compare(const RObj2DConfigPin& pin) const;

	/**
	 * Compare a pin configuration with an identifier.
	 * @param pin            Identifier of the pin.
	 * @return a value used by RContainer.
	 */
	int Compare(const size_t pin) const;

	/**
	 * @return the rectangle of the pin for the particular configuration.
	 */
	inline RRect GetRect(void) const {return(Rect);}

	/**
	 * @return the position of the pin for the particular configuration.
	 */
	inline RPoint GetPos(void) const {return(Pos);}

	/**
	 * @return return the pin.
	 */
	inline RObj2DPin* GetPin(void) const {return(Pin);}
};


//------------------------------------------------------------------------------
/**
* The RObj2DConfigConnector  class represent a possible configuration of a
* connector.
* @author Pascal Francq
* @short Configuration of a Connector.
*/
class RObj2DConfigConnector : public RContainer<RObj2DConfigPin,true,true>
{
	/**
	 * Corresponding connector.
	 */
	RObj2DConnector* Connector;

public:

	/**
	 * Constructor a particular configuration of a connector.
	 * @param con            Connector.
	 */
	RObj2DConfigConnector(RObj2DConnector* con);

	/**
	 * Compare two connector configuration.
	 * @param con            Connector configuration.
	 * @return a value used by RContainer.
	 */
	int Compare(const RObj2DConfigConnector& con) const;

	/**
	 * Compare a connector configuration with an identifier.
	 * @param con            Identifier of the connector.
	 * @return a value used by RContainer.
	 */
	int Compare(const size_t con) const;

	/**
	 * Compare a connector configuration with a name (if called with a
	 * container of RObj2DConfigConnector, it is not sort key).
	 * @param name           name of the connector.
	 * @return a value used by RContainer.
	 */
	int Compare(const RString& name) const;

	/**
	* Return the minimum distance between two connectors.
	* @param c               The second connector.
	* @param layout          Layout.
	* @param pin1            The first pin used (set by the method).
	* @param pin2            The second lin used (set by the method).
	*/
	double GetMinDist(RObj2DConfigConnector* c,RLayout* layout,RObj2DConfigPin* &pin1,RObj2DConfigPin* &pin2);

	/**
	 * @return the connector.
	 */
	inline RObj2DConnector* GetConnector(void) const {return(Connector);}
};


//------------------------------------------------------------------------------
/**
 * The RObj2DConfig provides a representation for a particular configuration of
 * an object.
 * @author Pascal Francq
 * @short 2D object configuration.
 */
class RObj2DConfig
{
	/**
	 * The object.
	 */
	RObj2D* Obj;

	/**
	* Polygon representing the object.
	*/
	RPolygon Polygon;

	/**
	* Rectangular decompositions of the polygon.
	*/
	RRects Rects;

	/**
	* Rectangle holding the polygon.
	*/
	RRect Rect;

	/**
	 * Orientation.
	 */
	tOrientation Orientation;

	/**
	* The connectors of this object.
	*/
	RContainer<RObj2DConfigConnector,true,true> Connectors;

	/**
	* Represent the area of the object.
	*/
	tCoord Area;

public:

	/**
	 * Constructor.
	 * @param obj            Object.
	 * @param ori            Orientation.
	 */
	RObj2DConfig(RObj2D* obj,tOrientation ori);

	/**
	 * Compare two configuration of object.
	 * @param obj            Configuration to compare with.
	 * @return a value used by RContainer.
	 */
	int Compare(const RObj2DConfig& obj) const;

	/**
	 * Compare the current configuration with a possible orientation.
	 * @param ori            Orientation
	 * @return a value used by RContainer.
	 */
	int Compare(const tOrientation ori) const;

	/**
	 * Set the polygon. The area and the decomposition in rectangles are
	 * computed.
	 * @param poly           Polygon.
	 */
	void Set(const RPolygon& poly);

	/**
	 * Set the rectangles corresponding to the decomposition of the polygon.
	 * The area is computed.
	 * @param rects          Rectangles. The container is emptied by this
	 *                       method.
	 */
	void Set(RContainer<RRect,false,false>& rects);

	/**
	* Return the Width of the object represented.
	*/
	inline tCoord GetWidth(void) const {return(Rect.GetWidth());}

	/**
	* Return the Height of the object represented.
	*/
	inline tCoord GetHeight(void) const {return(Rect.GetHeight());}

	/**
	* Return the area of the object.
	*/
	inline tCoord GetArea(void) {return(Area);}

	/**
	 * @return the orientation of the configuration.
	 */
	inline tOrientation GetOrientation(void) const {return(Orientation);}

	/**
	 * @return the polygon.
	 */
	inline const RPolygon& GetPolygon(void) const {return(Polygon);}

	/**
	 * @return a cursor over the connectors.
	 */
	RCursor<RObj2DConfigConnector> GetConnectors(void) const;

	/**
	 * @return a cursor over the rectangles representing the decomposition of
	 * the polygon.
	 */
	RCursor<RRect> GetRects(void) const;

	/**
	 * Add a connector to the configuration.
	 * @param con            Connector.
	 */
	void Add(RObj2DConfigConnector* con);

	/**
	 * @return the actual configuration of a given connector.
	 * @param con            Connector.
	 */
	RObj2DConfigConnector* GetConnector(RObj2DConnector* con) const;

	/**
	 * @return the actual configuration of a given connector.
	 * @param con            Name of the connector.
	 */
	RObj2DConfigConnector* GetConnector(const RString& con) const;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
