/*

	R Project Library

	RXMLTag.h

	XML tag - Header.

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
	long with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef RXMLTagH
#define RXMLTagH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rnode.h>
using namespace RStd;
#include <rxml/rxmlattr.h>
using namespace RXML;


//-----------------------------------------------------------------------------
namespace RXML{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RXMLStruct;
class RXMLFile;


//-----------------------------------------------------------------------------
/**
* This class provides a representation for a XML tag.
* @short XML Tag.
* @author Pascal Francq and Thomas L'Eglise.
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
	* Construct a XML Tag.
	* @param _name          The name of the tag.
	*/
	RXMLTag(RString _name);

	/**
	* Load a XML tag from a XML file.
	* @param f              The XML file.
	* @param xmlstruct      The XML Structure of the tag.
	*/
	void Load(RXMLFile *f,RXMLStruct *xmlstruct) throw(RString);

	/**
	* Save a XML tag to a XML file.
	* @param f              The XML file.
	* @param xmlstruct      The XML Structure of the tag.
	*/
	void Save(RXMLFile *f,int depth) throw(RString);

	/**
	* Compare a tag with a given name.
	* @param name           Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RString &name) {return(Name.Compare(name));}

	/**
	* Compare a tag with a given name.
	* @param name           Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const char* name) {return(Name.Compare(name));}

	/**
	* Compare two tags.
	* @param tag            Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RXMLTag *tag) {return(Name.Compare(tag->Name));}

	/**
	* Compare two tags.
	* @param tag            Tag used for the comparaison.
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
	* param name            Name of the attribute.
	* @returns a string containing the name.
	*/
	RString& GetAttrValue(const char* name);

	/**
	* Test if an attribute is defined.
	* param name            Name of the attribute.
	* @returns True if the attribute exists.
	*/
	bool IsAttrDefined(const char* name);

	/**
	* Find the first tag with a gtiven name.
	* @param name           Name of the tag to find.
	* @return Pointer to the element.
	*/
	RXMLTag* GetTag(const char* name);

	/**
	* Insert an attribute.
	* @param Attr           Attribute to insert
	*/
	void InsertAttr(RXMLAttr *Attr);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const RString& name,const RString& value);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const char* name,const RString& value);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const RString& name,const char* value);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const char* name,const char* value);

	/**
	* Add a tag to this one.
	* @param tag            Tag to add.
	*/
	void AddTag(RXMLTag* tag);

	/**
	* Add a string to the content of the tag.
	* @param text           Text to add.
	*/
	void AddContent(const char* text);

	/**
	* @returns the text containning in the tag.
	*/
	const char* GetContent(void) const {return(Contains());}

	/**
	* See if the tag has a content.
	* @returns bool.
	*/
	bool HasContent(void) const {return(Contains.GetLen());}

	/**
	* Start the iterator to go trough the word.
	*/
	inline void AttrsStart(void)
		{Attrs.Start();}

	/**
	* Test if the end of the container of words is reached.
	*/
	inline bool AttrsEnd(void) const
		{return(Attrs.End());}

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	inline void AttrsNext(void)
		{Attrs.Next();}

	/**
	* Get the current word.
	* @returns Pointer to the current word.
	*/
	RXMLAttr* GetCurAttrs(void) {return(Attrs());}

	/**
	* Destruct the XML Tag.
	*/
	~RXMLTag(void);

	// friend classes
	friend class RXMLStruct;
};


}  //-------- End of namespace RXML -----------------------------------------


//---------------------------------------------------------------------------
#endif
