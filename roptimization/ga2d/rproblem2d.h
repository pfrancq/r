/*

	R Project Library

	RProblem2D.h

	2D Placement Problem - Header.

	(c) 2001 by P. Francq.

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



//-----------------------------------------------------------------------------
#ifndef RProblem2DH
#define RProblem2DH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rxmltag.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
using namespace RGeometry2D;
#include <rga2d/robj2d.h>
#include <rga2d/rconnections.h>
using namespace RGA;


//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RProblem2D class provides a representation for a 2D placement problem.
* @author Pascal Francq
* @short 2D Placement Problem
*/
class RProblem2D
{	
public:
	
	/**
	* Limits for the construction.
	*/
	RPoint Limits;

	/**
	* Global limits for the construction.
	*/
	RPoint GlobalLimits;

	/**
	* Object representing the "Problem".
	*/
	RObj2D Problem;

	/**
	* Translation to be done to adapt the placement on the global objects.
	*/
	RPoint Translation;
	
	/**
	* Objects.
	*/
	RContainer<RObj2D,unsigned int,true,true> Objs;
	
	/**
	* Connections.
	*/
	RConnections Cons;

	/**
	* Constructor of the problem.
	*/
	RProblem2D(void);
	
	/**
	* Load a problem from a XML file.
	* @param name		Name of the XML file.
	*/
	void Load(const char* name);

	/**
	* Clears the problem.
	*/
	void Clear(void);
	
	/**
	* Determine the limits of the problem based of the shape.
	*/
	void DetermineLimit(void);

protected:

	/**
	* Create an object from a definition tag.
	* @param o		Tag representing an object.
	* @param ts		Container holding the templates.
	*/
	void CreateObj(RXMLTag* o,RContainer<RObj2D,unsigned int,true,true>& ts);

	/**
	* Create a connector from a tag.
	* @param c		Tag representing the connector.
	* @param obj	Object holding a connector (if 0, connector of problem).
	* @param t		Transalation to do for the place.
	*/
	void CreateConnector(RXMLTag* c,RObj2D* obj,const RPoint& t);

	/**
	* Create a net from a tag.
	* @param n		Tag representing the net.
	*/
	void CreateNet(RXMLTag* n);

public:
	/**
	* Destructor of the problem.
	*/
	~RProblem2D(void);
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
