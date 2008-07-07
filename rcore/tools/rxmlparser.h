/*

	R Project Library

	RXMLParser.h

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
#ifndef RXMLParser_H
#define RXMLParser_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextfile.h>
#include <rstack.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// forward declaration
class RXMLAttr;


//------------------------------------------------------------------------------
/**
* This class represents a XML parser. By default, it does nothing.
* @code
* #include <rxmlparser.h>
* using namespace R;
*
* RXMLParser In("/home/user/data.xml",&XML);
* In.Open(RIO::Read);
* @endcode
* @short XML Parser.
* @author Pascal Francq.
*/
class RXMLParser : public RTextFile
{
protected:
	class Namespace;

	/**
	 * Define the section of the XML actually treated.
	 */
	enum SectionType
	{
		Header                       /** The Tag "<!xml >".*/,
		StyleSheet                   /** The Tag "<?xml-stylesheet >".*/,
		DOCTYPE                      /** The Tag "<!DOCTYPE >.*/,
		Body                         /** The body of the XML file.*/
	};

	/**
	* Type of the document as defined in the XML file <!DOCTYPE >. If the tag
	* is omitted, the string is empty.
	*/
	RString DocType;

	/**
	* Determine if the current closing tag is a closing one.
	*/
	bool CurTagClosing;

	/**
	 * Namespaces defined in the XML file.
	 */
	RContainer<Namespace,true,true> Namespaces;

	/**
	 * Default namespace (if any).
	 */
	RStack<RString,true,true,true> DefaultNamespace;

	/**
	 * Avoid spaces in the XML file when creating it.
	 */
	bool AvoidSpaces;

	/**
	 * Current Depth.
	 */
	size_t CurDepth;

	/**
	 * Position of the last "token" extracted.
	 */
	size_t LastTokenPos;

	/**
	 * Current attributes.
	 */
	RContainer<RXMLAttr,true,true> Attrs;

	/**
	 * Determine what is currently treated.
	 */
	SectionType Section;

public:

	/**
	* Construct a XML file. If the pointer to the XML structure is null, a
	* default structure is created and destroy when the file is closed.
	* @param uri            URI of the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLParser(const RURI& uri,const RString& encoding="UTF-8");

	/**
	* Construct a XML file.
	* @param file           A generic input/output file that should be treated
	*                       as XML file.
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLParser(RIOFile& file,const RString& encoding="UTF-8");

	/**
	 * Avoid spaces when a XML file is created.
	 */
	void SetAvoidSpaces(bool as) { AvoidSpaces=as;}

	/**
	 * Get the current depth of the XML tree parsed.
	 */
	size_t GetCurrentDepth(void) const;

	/**
	 * Get the position of the last token extracted
	 */
	size_t GetLastTokenPos(void) const;

	/**
	* Open the file.
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

	/**
	 * Get the current section treated.
	 */
	SectionType GetSection(void) const {return(Section);}

protected:

	/**
	* This function transform a given string that is supposed to represent a
	* HTML code (ex: "Ucirc") into the corresponding character (ex: û).
	* @param code           HTML Code.
	* @returns A RChar corresponding to the code or 0 if the code is not
	* identified.
	*/
	virtual RChar CodeToChar(RString& code);

	/**
	* This function transform a given character (ex: <) into a string that
	* represents a HTML code (ex: "lt").
	* @param car            Character.
	* @param strict          If strict is true, the quotes are also transform,
	*                        else on < and > are transformed.
	* @returns A RString corresponding to the character or the character itself
	* if a code is not identified.
	*/
	RString CharToCode(RChar car,bool strict=true);

	/**
	* This function transform a string containing some XML or HTML code into a
	* string with normal characters.
	* @param str            XML string.
	* @returns A RString containing a normal string.
	* @exception RIOException An exception is generated if the string contains
	*                         an invalid XML or HTML code.
	*/
	RString XMLToString(const RString& str);

	/**
	* This function transform a normal string into a valid XML string where some
	* characters are replaced by codes.
	* @param str             Normal string.
	* @param strict          If strict is true, the quotes are also transform,
	*                        else on < and > are transformed.
	* @returns A RString containing a valid XML string.
	*/
	RString StringToXML(const RString& str,bool strict=true);

private:

	/**
	* Load the Header of a XML file (or nothing if it seems to be a HTML file).
	*/
	void LoadHeader(void);

	/**
	* Load the next XML tag from a XML file.
	*/
	void LoadNextTag(void);

	/**
	* Load the attributes of the current tag and put them in a container. By
	* default, the tag is supposed to be a normal XML tag ending with either
	* '/>' or '>'.
	* @param popdefault     A default namespace is defined for this tag
	* @param popuri         Namespaces with prefixes defined for this tag.
	* @param EndTag1        Character than can delimited the tag.
	* @param EndTag2        Another character than can delimited the tag.
	*/
	void LoadAttributes(bool& popdefault,RContainer<Namespace,false,false>& popuri,RChar EndTag1='/',RChar EndTag2='>');

protected:

	/**
	* Set the doctype of the XML document.
	* @param docType        Name of the encoding.
	*/
	virtual void SetDocType(const RString& docType);

	/**
	* Method called each time a tag will be treated when reading a XML file.
	* Actually, a XMLTag is created and inserted as child of the current tag. At
	* the end, the current tag is set to the new created one.
	* @param namespaceURI    Name of the namespace (if any).
	* @param lName           Local name of the tag.
	* @param name            Complete name of the tag.
	*/
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time an attribute will be treated when reading a XML
	* file. Actually, the attribute is added to the current tag if one is
	* defined or to an internal container.
	* @param namespaceURI    Name of the namespace (if any).
	* @param lName           Local name of the attribute.
	* @param name            Complete name of the attribute.
	* @param value           Value of the attribute (if any).
	 */
	virtual void AddAttribute(const RString& namespaceURI,const RString& lName,const RString& name);

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
	virtual void EndTag(const RString& namespaceURI,const RString& lName,const RString& name);

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

	/**
	* Method that specify if only quotes are allowed to delimit a parameter in
	* a tag. By default, this function return true which is the syntax of XML.
	*/
	virtual bool OnlyQuote(void);

	/**
	* Method that specify if invalid XML codes (sequences beginning with a '&'
	* are accepted. By default, this function return false which is the syntax
	* of XML.
	*/
	virtual bool InvalidXMLCodeAccept(void);

public:

	/**
	* Destruct the XML file.
	*/
	virtual ~RXMLParser(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
