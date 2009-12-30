/*

	R Project Library

	RProblem2D.h

	2D Placement Problem - Header.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RProblem2D_H
#define RProblem2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rxmltag.h>
#include <rpoint.h>
#include <robj2d.h>
#include <rconnections.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RProblem2D class provides a representation for a 2D placement problem.
* @author Pascal Francq
* @short 2D Placement Problem
*/
class RProblem2D
{
	class Parser;

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
	 * Template objects.
	 */
	RContainer<RObj2D,true,true> Templates;

	/**
	* Objects.
	*/
	RContainer<RObj2D,true,true> Objs;

	/**
	* Connections.
	*/
	RConnections Cons;

	/**
	 * URI of the file containing the problem.
	 */
	RURI URI;

	/**
	* Constructor of the problem.
	* @param uri             URI of the XML file.
	*/
	RProblem2D(const RURI& uri);

	/**
	 * @return the URI of the problem.
	 */
	RURI GetURI(void) const {return(RURI(URI));}

private:

	/**
	* Load a problem from a XML file.
	*/
	void Load(void);

public:

	/**
	* Clears the problem.
	*/
	void Clear(void);

	/**
	* Determine the limits of the problem based of the shape.
	*/
	void DetermineLimit(void);

	/**
	 * @return the number of objects.
	 */
	size_t GetNbObjs(void) const {return(Objs.GetNb());}

	/**
	* Destructor of the problem.
	*/
	~RProblem2D(void);

	friend class Parser;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
