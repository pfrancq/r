/*

  rxmltag.h

  Description - Header.

  (c) 2000 by P. Francq.

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
#ifndef RXMLTAG_H
#define RXMLTAG_H



//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstring.h>
#include <rstd/rnode.h>
#include <rstd/rxmlattr.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RXMLStruct;
class RXMLFile;


//-----------------------------------------------------------------------------
/**
* This class provides a representation for a XML tag.
* @short XML Tag.
* @author Pascal Francq
*/
class RXMLTag : public RNode<RXMLTag,false>
{

	/**
	* The name of the tag.
	*/
	RString Name;

	/**
	* The contain of the tag
	*/
	RString Contains;

	/**
	* The attributes of the tag.
	*/
	RContainer<RXMLAttr,unsigned,true,true> Attrs;

public:

	/**
	* Construct a XML Tag.
	*/
	RXMLTag(void);

	/**
	* Load a XML tag from a XML file.
	* @param f					The XML file.
	* @param xmlstruct		The XML Structure of the tag.
	*/
	void Load(RXMLFile *f,RXMLStruct *xmlstruct) throw(RString);

	/**
	* Compare a tag with a given name.
	* @param name	Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RString &name) {return(Name.Compare(name));}

	/**
	* Compare a tag with a given name.
	* @param name	Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const char* name) {return(Name.Compare(name));}

	/**
	* Compare two tags.
	* @param tag	Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RXMLTag *tag) {return(Name.Compare(tag->Name));}

	/**
	* Compare two tags.
	* @param tag	Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RXMLTag &tag) {return(Name.Compare(tag.Name));}

	/**
	* Return the name of the tag.
	* @returns a string containing the name.
	*/
	RString& GetName(void);

	/**
	* Return the name of the tag.
	* @returns a string containing the name.
	*/
	RString& GetAttrValue(const char *name);


	/**
	* Destruct the XML Tag.
	*/
	~RXMLTag(void);
};


}  //-------- End of namespace RStd -----------------------------------------


//---------------------------------------------------------------------------
#endif
