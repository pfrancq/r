/*

	R Project Library

	RXMLFile.h

	XML file - Header.

	Copyright 2000-2004 by the Université Libre de Bruxelles.

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
#include <rstd/rtextfile.h>
#include <rstd/rxmlstruct.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a XML file.
* @short XML File.
* @author Pascal Francq.
*/
class RXMLFile : public RTextFile
{
protected:
	/**
	* The structure associated with the XML file.
	*/
	RXMLStruct* XMLStruct;

	/**
	* Type of the document as defined in the XML file <!DOCTYPE >. If the tag
	* is omitted, the string is empty.
	*/
	RString DocType;

	/**
	* The current tag treated.
	*/
	RXMLTag* CurTag;

	/**
	* Determine if the current closing tag is a closing one.
	*/
	bool CurTagClosing;

public:

	/**
	* Construct a XML file.
	* @param name           Name of the file.
	* @param xmlstruct      The XML tree associated with the file
	* @param mode           The open mode for the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLFile(const RString& name,RXMLStruct* xmlstruct,ModeType mode=Read,const RString& encoding="UTF-8") throw(std::bad_alloc,RIOException);

	/**
	* Construct a XML file.
	* @param name           Name of the file.
	* @param xmlstruct      The XML tree associated with the file
	* @param mode           The open mode for the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLFile(const RString& name,RXMLStruct& xmlstruct,ModeType mode=Read,const RString& encoding="UTF-8") throw(std::bad_alloc,RIOException);

	/**
	* Set the encoding of the XML document.
	* @param name           Name of the encoding.
	*/
	void SetEncoding(const RString& name) throw(RIOException);

	/**
	* Set the doctype of the XML document.
	* @param docType        Name of the encoding.
	*/
	virtual void SetDocType(const RString& docType) throw(RIOException);

	/**
	* Process the XML file.
	*/
	void Process(void) throw(RIOException);

	/**
	* This function transform a given string that is supposed to represent a
	* HTML code (ex: "Ucirc") into the corresponding character (ex: ï¿½.
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
	RString XMLToString(const RString& str) throw(RIOException);

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
	void LoadHeader(void) throw(RIOException);

	/**
	* Load the next XML tag from a XML file.
	*/
	void LoadNextTag(void) throw(RIOException);

	/**
	* Load the attributes and put them in a container. By default, the tag is
	* supposed to be a normal XML tag ending with either '/>' or '>'.
	* @param attrs          Container that will hold the attributes.
	* @param EndTag1        Character than can delimited the tag.               
	* @param EndTag2        Another character than can delimited the tag.
	*/
	void LoadAttributes(RContainer<RXMLAttr,unsigned,true,true>& attrs,RChar EndTag1='/',RChar EndTag2='>') throw(RIOException);

	/**
	* Save the next XML tag into the XML file.
	* @param depth          The depth of the tag in the XML structure.
	*/
	void SaveNextTag(int depth) throw(RIOException);

	/**
	* Add the next character to the string. "CR" / "LF" is converted to "LF".
	* @param str            String to which to add the character.
	*/
	void AddNextCharacter(RString& str) throw(RIOException);

protected:

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
	* Function that specify if only quotes are allowed to delimit a parameter in
	* a tag. By default, this function return true which is the syntax of XML.
	*/
	virtual bool OnlyQuote(void);

	/**
	* Function that specify if invalid XML codes (sequences beginning with a '&'
	* are accepted. By default, this function return false which is the syntax
	* of XML.
	*/
	virtual bool InvalidXMLCodeAccept(void);

public:

	/**
	* Destruct the XML file.
	*/
	~RXMLFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
