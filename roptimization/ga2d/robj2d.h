/*

	R Project Library

	RObj2D.h

	Object for 2D placement GA - Header

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
#ifndef RObj2D_H
#define RObj2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga2d.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RObj2DPin represents a pin for a particular connector.
 * @author Pascal Francq.
 * @short Pin of a connector.
 */
class RObj2DPin
{
	/**
	 * Connector of the pin.
	 */
	RObj2DConnector* Connector;

	/**
	* Identifier of the pin.
	*/
	size_t Id;

	/**
	* Name of the pin.
	*/
	RString Name;

public:

	/**
	 * Constructor of a pin.
	 * @param con            Corresponding connector.
	 * @param id             Identifier of the pin.
	 * @param name           Name of the pin.
	 */
	RObj2DPin(RObj2DConnector* con,size_t id,const RString& name);

	/**
	 * Compare two pin.
	 * @param pin            Pin to compare with.
	 * @return a value used by RContainer.
	 */
	int Compare(const RObj2DPin& pin) const;

	/**
	 * Compare a pin with an identifier.
	 * @param pin            Identifier of the pin.
	 * @return a value used by RContainer.
	 */
	int Compare(const size_t pin) const;

	/**
	 * @return the connector.
	 */
	inline RObj2DConnector* GetConnector(void) const {return(Connector);}

	/**
	* @return the identifier of the pin.
	*/
	inline size_t GetId(void) {return(Id);}

	/**
	 * @return the name of the pin.
	 */
	inline RString GetName(void) const {return(Name);}
};


//------------------------------------------------------------------------------
/**
* This basic class represent a connection point for an object to place by using
* the 2D placement GA.
* @author Pascal Francq
* @short Connector of a 2D Object.
*/
class RObj2DConnector : public RContainer<RObj2DPin,true,true>
{
	/**
	* Owner of the connector.
	*/
	RObj2D* Obj;

	/**
	* Identifier of the connector.
	*/
	size_t Id;

	/**
	* Name of the connector.
	*/
	RString Name;

	/**
	* Connections of which the connector is involved.
	*/
	RContainer<RConnection,false,false> Connections;

public:

	/**
	* Construct a connector.
	* @param obj             Object of the connector.
	* @param id              Id of the connector.
	* @param name            Name of the connector.
	*/
	RObj2DConnector(RObj2D* obj, size_t id,const RString& name);

	/**
	 * Compare two connector.
	 * @param con            Connector.
	 * @return a value used by RContainer.
	 */
	int Compare(const RObj2DConnector& con) const {return(CompareIds(Id,con.Id));}

	/**
	 * Compare a connector with an identifier.
	 * @param id             Identifier of the connector.
	 * @return a value used by RContainer.
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
	* Return the object that's own the connector. A null object signifies that
	* it is an external connector.
	*/
	RObj2D* GetObj(void) const {return(Obj);}

	/**
	* @return the identifier of the connector.
	*/
	inline size_t GetId(void) const {return(Id);}

	/**
	 * @return the name of the connector.
	 */
	inline RString GetName(void) const {return(Name);}

	/**
	 * @return a cursor over the connections.
	 */
	inline RCursor<RConnection> GetConnections(void) const {return(RCursor<RConnection>(Connections));}

	/**
	* Destruct the connectors
	*/
	virtual ~RObj2DConnector(void);

	friend class RConnection;
};


//------------------------------------------------------------------------------
/**
* The RObj2D class represent an object to place.
* @author Pascal Francq
* @short 2D Object.
*/
class RObj2D : public RContainer<RObj2DConfig,true,true>
{
protected:
	/**
	* Identifier of the object.
	*/
	size_t Id;

	/**
	* Name of the object.
	*/
	RString Name;

	/**
	* The connectors of this object
	*/
	RContainer<RObj2DConnector,true,true> Connectors;

public:

	/**
	* Construct an 2D object.
	* @param id             Identifier of the object.
	* @param name           Name of the object.
	*/
	RObj2D(size_t id,const RString& name);

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
	* Create a specific orientation if possible. The method creates the correct
	* configuration and compute the rectangular decompositions and the
	* area of the object.
	* @param ori             Orientation.
	*/
	void CreateOri(tOrientation ori);

	/**
	* Return the identifier of the object.
	*/
	inline size_t GetId(void) const {return(Id);}

	/**
	 * Set the identifier of the object if the current one is invalid.
	 * @param id             Identifier.
	 */
	void SetId(size_t id);

	/**
	 * @return the name of the connector.
	 */
	inline RString GetName(void) const {return(Name);}

	/**
	* Verify if a given object is in the container or not.
	* @param id             Identifier of the object.
	* @return true if the object is in, else false.
	*/
	virtual bool IsIn(size_t id) const {return(Id==id);}

	/**
	 * The method is used to manage the configuration (the one corresponding to
	 * the oNormal orientation). It is created if necessary.
	 * @return the default configuration.
	 */
	RObj2DConfig* GetDefaultConfig(void);

	/**
	 * @return the number of connectors.
	 */
	inline size_t GetNbConnectors(void) const {return(Connectors.GetNb());}

	/**
	 * @return a cursor over the connectors.
	 */
	inline RCursor<RObj2DConnector> GetConnectors(void) const {return(RCursor<RObj2DConnector>(Connectors));}

	/**
	 * @return a pointer to a particular connector.
	 * @param use            Search criteria.
	 * @param order          Specify if the search criteria is the one ordering
	 *                       the container.
	 */
	template<class TUse> inline RObj2DConnector* GetConnector(const TUse& use,bool order=true) const {return(Connectors.GetPtr(use,order));}

	/**
	 * Copy an object.
	 * @param obj            Object to copy.
	 */
	void Copy(const RObj2D& obj);

	/**
	* Destruct the object.
	*/
	virtual ~RObj2D(void);

	friend class RObj2DConnector;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
