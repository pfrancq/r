/*

	R Project Library

	RXMLStruct.h

	XML structure - Header.

	(c) 2000-2001 by P. Francq and T. L'Eglise.

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



//---------------------------------------------------------------------------
#ifndef RXMLStructH
#define RXMLStructH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rtree.h>
#include <rstd/rxmltag.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class provides a representation of a XML structure.
* @short XML Structure.
* @author Pascal Francq and Thomas L'Eglise.
*/
class RXMLStruct : public RTree<RXMLTag,true,false>
{
public:

	/**
	* Construct the XML Structure.
	*/
	RXMLStruct(void);

	/**
	* Find the first tag with a given name.
	* @param name		Name of the tag to find.
	* @returns a pointer to the tag of 0 if no tag exists.
	*/
	RXMLTag* GetTag(const char* name);

	/**
	* Destruct the XML Structure.
	*/
	~RXMLStruct(void);
};


}  //-------- End of namespace RStd -------------------------------------------


//-----------------------------------------------------------------------------
#endif
