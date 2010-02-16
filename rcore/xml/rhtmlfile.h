/*

	R Project Library

	RHTMLFile.h

	HTML file - Header.

	Copyright 2004-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rxmlfile.h>


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
public:

	/**
	 * Default constructor.
	 */
	RHTMLFile(void);

	/**
	* Construct a HTML file to read.
	* @param uri            URI of the file.
	* @param xmlstruct      The XML tree associated with the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RHTMLFile(const RURI& uri,RXMLStruct* xmlstruct,const RCString& encoding="Latin1");

	/**
	* Construct a HTML file to read.
	* @param file           A generic input/output file that should be treated
	*                       as XHML file.
	* @param xmlstruct      The XML tree associated with the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RHTMLFile(RIOFile& file,RXMLStruct* xmlstruct,const RCString& encoding="Latin1");

	/**
	 * Open the file
	 * @param mode           The open mode for the file.
	 */
	void Open(RIO::ModeType mode);

	/**
	 * Open the file
	 * @param uri            URI of the file.
	 * @param xmlstruct      The XML tree associated with the file
	 * @param mode           The open mode for the file.
	 * @param encoding       The encoding scheme of the file.
	 */
	void Open(const RURI& uri,RXMLStruct* xmlstruct,RIO::ModeType mode=RIO::Read,const RCString& encoding="UTF-8");

	/**
	* Set the doctype of the XML document. The doctype is transform in lowercase
	* and XHTML correspond to the open tag 'html'. If html is not the result, an
	* exception occurs.
	* @param docType        Name of the encoding.
	*/
	virtual void SetDocType(const RString& docType);

public:

	/**
	* Function called each time a tag will be treated when reading a XML file.
	* Actually, a XMLTag is created and inserted as child of the current tag. At
	* the end, the current tag is set to the new created one.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void BeginTag(const RString& namespaceURI, const RString& lName, const RString& name);

	/**
	* Function called each time a tag was treated when reading a XML file.
	* Actually, the current tag is set its parent.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	* @remarks The namespace are not treated for the moment, so the namespaceURI
	*          and lname parameters are always empty.
	*/
	virtual void EndTag(const RString& namespaceURI, const RString& lName, const RString& name);

	/**
	* Function called each time a text is processed when reading a XML file.
	* Actually, the text is added as content to the current tag.
	* @param text          Text processed.
	*/
	virtual void Text(const RString& text);

	/**
	* Function that specify if only quotes are allowed to delimit a parameter in
	* a tag. This function return false since quote can be omitted in HTML.
	*/
	virtual bool OnlyQuote(void);

	/**
	* Destruct the XML file.
	*/
	virtual ~RHTMLFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
