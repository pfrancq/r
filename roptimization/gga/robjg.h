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
#ifndef RObjGH
#define RObjGH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


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

	/**
	* Name of the objects.
	*/
	RString Name;

public:

	/**
	* Construct the object.
	* @param id             Identificator.
	* @param name           Name of the object.
	*/
	RObjG(const unsigned int id,const RString& name);

	/**
	* Copy Construct.
	* @param obj            Source used.
	*/
	RObjG(const RObjG* obj);

	/**
	* Comparaison function
	*/
	int Compare(const unsigned int &id) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const RObjG& obj) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const RObjG* obj) const;

	/**
	* Return the identificator of the object.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Return the Name of the object.
	*/
	RString& GetName(void) const;

	/**
	* Destruct the object.
	*/
	virtual ~RObjG(void);
};


}  //------- End of namespace RGGA --------------------------------------------


//-----------------------------------------------------------------------------
#endif
