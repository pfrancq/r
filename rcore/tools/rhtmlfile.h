/*

	R Project Library

	RHTMLFile.h

	HTML file - Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef RHTMLFile_H
#define RHTMLFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmlfile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a HTML file. The file is loaded in a XML structure that
* should be compatible with XHTML.
* @short HTML File.
* @author Pascal Francq.
*/
class RHTMLFile : public RXMLFile
{
	class Tag;

	/**
	* HTML Tags.
	*/
	R::RContainer<Tag,unsigned int,true,true> Tags;

	/**
	* Is the </html> found?
	*/
	bool FoundClosingHTML;

public:

	/**
	* Construct a HTML file to read.
	* @param name           Name of the file.
	* @param xmlstruct      The XML tree associated with the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RHTMLFile(const char* name,RXMLStruct* xmlstruct,const RString& encoding="Latin1") throw(std::bad_alloc,RIOException);

	/**
	* Construct a HTML file to read.
	* @param name           Name of the file.
	* @param xmlstruct      The XML tree associated with the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RHTMLFile(const char* name,RXMLStruct& xmlstruct,const RString& encoding="Latin1") throw(std::bad_alloc,RIOException);

	/**
	* Set the doctype of the XML document. The doctype is transform in lowercase
	* and XHTML correspond to the open tag 'html'. If html is not the result, an
	* exception occurs.
	* @param docType        Name of the encoding.
	*/
	virtual void SetDocType(const RString& docType) throw(RIOException);

protected:

	/**
	* This methode creates all the tags valid for the HTML version supported.
	*/
	void InitValidTags(void) throw(std::bad_alloc);

public:

	/**
	* Function called each time a tag will be treated when reading a XML file.
	* Acutally, a XMLTag is created and inserted as child of the current tag. At
	* the end, the current tag is set to the new created one.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @param attrs         Attributes associated to the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void BeginTag(const RString& namespaceURI, const RString& lName, const RString& name,RContainer<RXMLAttr,unsigned,true,true>& attrs) throw(RIOException);

	/**
	* Function called each time a tag was treated when reading a XML file.
	* Acutally, the current tag is set its parent.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void EndTag(const RString& namespaceURI, const RString& lName, const RString& name) throw(RIOException);

	/**
	* Function called each time a text is processed when reading a XML file.
	* Actually, the text is added as content to the current tag.
	* @param text          Text processed.
	*/
	virtual void Text(const RString& text) throw(RIOException);

	/**
	* Function to find the end of the tag and to skip all the content
	* @param tag           The name of the tag to close
	*/
	virtual void SkipTagContent(const RString& tag)throw(RIOException);
	
	/**
	* Function that specify if only quotes are allowed to delimit a parameter in
	* a tag. This function return false since quote can be omitted in HTML.
	*/
	virtual bool OnlyQuote(void);

	/**
	* Function that specify if invalid XML codes (sequences beginning with a '&'
	* are accepted. This function return true since it is allowed in HTML.
	*/
	virtual bool InvalidXMLCodeAccept(void);
       
	/**
	* Destruct the XML file.
	*/
	~RHTMLFile(void);
};                          


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
