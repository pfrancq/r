/*

	R Project Library

	RXMLFile.h

	XML file - Header.

	Copyright 2000-2008 by the Université Libre de Bruxelles.

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
#ifndef RXMLFile_H
#define RXMLFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlparser.h>
#include <rxmlstruct.h>
#include <rstack.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a XML file.
* @code
* #include <rxmlfile.h>
* #include <rxmlstruct.h>
* using namespace R;
*
* RXMLStruct XML;
* RXMLFile In("/home/user/data.xml",&XML);
* In.Open(RIO::Read);
* RXMLFile Out("/home/user/data.xml.backup",&XML);
* Out.Open(RIO::Create);
* @endcode
* @short XML File.
* @author Pascal Francq.
*/
class RXMLFile : public RXMLParser
{
protected:
	/**
	* The structure associated with the XML file.
	*/
	RXMLStruct* XMLStruct;

	/**
	 * The XML Structure was created by RXMLFile.
	 */
	bool NewStruct;

	/**
	* Current tag treated.
	*/
	RXMLTag* CurTag;

	/**
	 * Current attribute treated.
	 */
	RXMLAttr* CurAttr;

	/**
	 * Name of an attribute in the headers.
	 */
	RString AttrName;

public:

	/**
	* Construct a XML file. If the pointer to the XML structure is null, a
	* default structure is created and destroy when the file is closed.
	* @param uri            URI of the file.
	* @param xmlstruct      The XML tree associated with the file
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLFile(const RURI& uri,RXMLStruct* xmlstruct,const RString& encoding="UTF-8");

	/**
	* Construct a XML file.
	* @param uri            URI of the file.
	* @param xmlstruct      The XML tree associated with the file
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLFile(const RURI& uri,RXMLStruct& xmlstruct,const RString& encoding="UTF-8");

	/**
	* Construct a XML file.
	* @param file           A generic input/output file that should be treated
	*                       as XML file.
	* @param xmlstruct      The XML tree associated with the file
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLFile(RIOFile& file,RXMLStruct* xmlstruct,const RString& encoding="UTF-8");

	/**
	* Construct a XML file.
	* @param file           A generic input/output file that should be treated
	*                       as XML file.
	* @param xmlstruct      The XML tree associated with the file
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLFile(RIOFile& file,RXMLStruct& xmlstruct,const RString& encoding="UTF-8");

	/**
	* Set the encoding of the XML document.
	* @param name           Name of the encoding.
	*/
	void SetEncoding(const RString& name);

	/**
	* Open the file.
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

	/**
	 * Close the file. If the XML structure was created by the file, it is
	 * deleted.
	 */
	virtual void Close(void);

private:

	/**
	* Save the next XML tag into the XML file.
	* @param depth          The depth of the tag in the XML structure.
	*/
	void SaveNextTag(int depth);

protected:

	/**
	 * Add a entity found.
	 * @param name           Name of the entity.
	 * @param value          Corresponding value.
	 */
	virtual void AddEntity(const RString& name,const RString& value);

	/**
	* Method called each time a tag will be treated when reading a XML file.
	* Actually, a XMLTag is created and inserted as child of the current tag. At
	* the end, the current tag is set to the new created one.
	* @param namespaceURI    Name of the namespace (if any).
	* @param lName           Local name of the tag.
	* @param name            Complete name of the tag.
	*/
	virtual void BeginTag(const RString& namespaceURI, const RString& lName, const RString& name);

	/**
	* Method called each time an attribute will be treated when reading a XML
	* file. Actually, the attribute is added to the current tag if one is
	* defined or to an internal container.
	* @param namespaceURI    Name of the namespace (if any).
	* @param lName           Local name of the attribute.
	* @param name            Complete name of the attribute.
	* @param value           Value of the attribute (if any).
	 */
	virtual void AddAttribute(const RString& namespaceURI, const RString& lName, const RString& name);

	/**
	* Method called each time some attribute value elements (words or spaces)
	* are parsed when reading a XML file. Actually, the text is added as
	* content to value of the current attribute.
	* @param value           Value processed.
	 */
	virtual void Value(const RString& value);

	/**
	* Method called each time a tag was treated when reading a XML file.
	* Actually, the current tag is set its parent.
	* @param namespaceURI  Name of the namespace (if any).
	* @param lName         Local name of the tag.
	* @param name          Complete name of the tag.
	*/
	virtual void EndTag(const RString& namespaceURI, const RString& lName, const RString& name);

	/**
	* Method called each time some text elements (words or spaces) are parsed
	* when reading a XML file. Actually, the text is added as content to the
	* current tag.
	* @param text          Text processed.
	*/
	virtual void Text(const RString& text);

	/**
	 * Method called to define the DTD of the XML file.
	 * @param dtd            DTD.
	 */
	virtual void SetDTD(const RString& dtd);

public:

	/**
	* Destruct the XML file.
	*/
	virtual ~RXMLFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
