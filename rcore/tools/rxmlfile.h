/*

    R Project Library

    RXMLFile.h

    XML file - Header.

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



//-----------------------------------------------------------------------------
#ifndef RXMLFileH
#define RXMLFileH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rtextfile.h>
#include <rstd/rxmlstruct.h>
using namespace RStd;



//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represents a XML file.
* @short XML File.
* @author Pascal Francq and Thomas L'Eglise.
*/

class RXMLFile : public RTextFile
{
    /**
    * The structure associated with the XML file.
    */
    RXMLStruct *XMLStruct;

public:

    /**
    * Construct a XML file.
    * @param name       Name of the file.
    * @param xmlstruct      The XML tree associated with the file
    * @param mode       The open mode for the file.
    */
    RXMLFile(const char* name,RXMLStruct *xmlstruct,ModeType mode=RTextFile::Read);

    /**
    * Test if a new tag begins.
    * @returns true if a new tag begins.
    */
    bool BeginTag(void);

    /**
    * Test if a tag is ending.
    * @returns true if the tags is ending.
    */
    bool EndTag(void);

    /**
    * Return the next tag in the file.
    * @returns Pointer to a C string containing the tag;
    */
    char* GetTag(void);

    /**
    * Return the contains of the current tag in the file.
    * @returns Pointer to a C string.
    */
    char *GetContains(void);

    /**
    * Destruct the XML file.
    */
    ~RXMLFile(void);
};


}  //-------- End of namespace RStd -----------------------------------------


//---------------------------------------------------------------------------
#endif
