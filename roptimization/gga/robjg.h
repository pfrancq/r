/*

	R Project Library

	RObjg.h

	Object to place in a specific group - Header.

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
#ifndef RObjgH
#define RObjgH


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constance
const unsigned int NoObject=0xFFFFFFFF;


//-----------------------------------------------------------------------------
/**
* The Robjg class provides a representation of an object to place in a group.
* @author Pascal Francq
* @short Object.
*/
class RObjG
{
protected:

	/**
	* The identificator of the object.
	*/
	unsigned int Id;

public:

	/**
	* Construct the object.
	* @param id             Identificator.
	*/
	RObjG(const unsigned int id);

	/**
	* Return the identificator of the object.
	*/
	unsigned int GetId(void) {return(Id);}

	/**
	* Destruct the object.
	*/
	virtual ~RObjG(void);
};


}  //------- End of namespace RGGA --------------------------------------------


//-----------------------------------------------------------------------------
#endif
