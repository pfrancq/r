/*

	R Project Library

	RXMLParser.h

	XML Parser - Header.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
* This class represents a parser for a XML stream read from a file. By default,
* it does nothing. It should be inherited by child classes that actually treat
* the information parsed (tags, attributes and content).
*
* The RXMLFile is an example of a child class that fill a RXMLStruct from a XML
* file.
* @code
* #include <rxmlparser.h>
* using namespace R;
*
* RXMLParser In("/home/user/data.xml");
* In.Open(RIO::Read);
* @endcode
* @short XML Stream Parser.
* @author Pascal Francq.
*/
class RXMLParser : public RTextFile
{
public:
	/**
	 * Define the section of the XML actually treated.
	 */
	enum SectionType
	{
		Header                       /** The Tag "<!xml >".*/,
		StyleSheet                   /** The Tag "<?xml-stylesheet >".*/,
		DOCTYPE                      /** The Tag "<!DOCTYPE >".*/,
		Body                         /** The body of the XML file.*/
	};

protected:

	/**
	 * The HTMLTag provides a representation for a HTML Tag.
	 */
	class HTMLTag
	{
	public:
		/**
		 *  Name of the HTML Tag.
		 */
		RString Name;

		/**
		 * Integer representing the depth of the tag in HTML structure.
		 */
		int Level;

	    /**
	     * A single Tag (ex: br)?.
	     */
		bool Single;

		/**
		 * The tag may contain itself as child (false by default).
		 */
		bool SelfContained;

		/**
		 * Construct a HTML tag.
		 * @param name       Name of the tag.
		 * @param level      Level of the tag.
		 * @param single     Single tag.
		 * @param self       Self contained.
		 * @return
		 */
		HTMLTag(const RString& name,int level,bool single,bool self=false);

		/**
		 * Compare two tags.
		 * @param t          Tag to compare with.
		 * @return the "difference" between the names.
		 */
		int Compare(const HTMLTag& t) const;

		/**
		 * Compare a tag with a name of a tag.
		 * @param t          Tag name.
		 * @return the "difference" between the names.
		 */
		int Compare(const RString& t) const;
	};

private:

	class Namespace;
	class Attribute;

	/**
	* HTML Tags.
	*/
	static R::RContainer<HTMLTag,true,true> Tags;

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
	 * Current attributes treated.
	 */
	RContainer<Attribute,true,false> Attributes;

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
	 * Determine what is currently treated.
	 */
	SectionType Section;

	/**
	 * Specify if only invalid XML codes are accepted.
	 */
	bool InvalidXMLCodes;

	/**
	 * Specify if the encoding is currently treated.
	 */
	bool TreatEncoding;

	/**
	 * HTMLMode active.
	 */
	bool HTMLMode;

	/**
	* Is the "</html>" found?
	*/
	bool FoundClosingHTML;

	/**
	 * Pointer to the current HTML tag.
	 */
	HTMLTag* CurHTMLTag;

public:

	/**
	 * Default constructor.
	 */
	RXMLParser(void);

	/**
	* Construct a XML file. If the pointer to the XML structure is null, a
	* default structure is created and destroy when the file is closed.
	* @param uri            URI of the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLParser(const RURI& uri,const RCString& encoding="UTF-8");

	/**
	* Construct a XML file.
	* @param file           A generic input/output file that should be treated
	*                       as XML file.
	* @param encoding       The encoding scheme of the file.
	*/
	RXMLParser(RIOFile& file,const RCString& encoding="UTF-8");

private:

	/**
	* This method creates all the tags valid for the HTML version supported.
	*/
	void InitValidTags(void);

public:

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
	 * Set the HTML mode.
	 * @param html           HTML mode active ?
	 */
	void SetHTMLMode(bool html);

	/**
	* Open the file.
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

	/**
	 * Open the file
	 * @param uri            URI of the file.
	 * @param mode           The open mode for the file.
	 * @param encoding       The encoding scheme of the file.
	 */
	void Open(const RURI& uri,RIO::ModeType mode=RIO::Read,const RCString& encoding="UTF-8");

	/**
	 * Get the current section treated.
	 */
	SectionType GetSection(void) const {return(Section);}

protected:

	/**
	* This function transform a given string that is supposed to represent a
	* character. For example, the code #quot; is a quote.
	* @param code           Code.
	* @returns A RChar corresponding to the code or 0 if the code is not a valid
	* one.
	*/
	virtual RChar CodeToChar(RString& code);

	/**
	* This function transform a given character (ex: <) into a string that
	* represents a HTML code (ex: "lt").
	* @param car             Character.
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
	* @exception A RIOException exception is generated if the string contains
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

	/**
	 * @return the current HTML Tag (if any).
	 */
	inline const HTMLTag* GetCurHTMLTag(void) const {return(CurHTMLTag);}

	/**
	 * @return a pointer to a given tag.
	 * @param name           Name of the tag.
	 */
	const HTMLTag* GetHTMLTag(const RString& name) const;

	/**
	 * @return true if the current tag is a closing one.
	 */
	inline bool IsCurTagClosing(void) const {return(CurTagClosing);}

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

	/**
	* Method called each time an attribute will be treated when reading the XML
	* header. Actually, it just catches the "encoding" attribute.
	* @param namespaceURI    Namespace (if any).
	* @param lName           Local name of the attribute.
	* @param name            Complete name of the attribute.
	 */
	void HeaderAttribute(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time some attribute value elements (words or spaces)
	* are parsed when reading the XML header. Actually, it only set the
	* encoding of the file using the value associated to the "encoding"
	* attribute.
	* @param value           Value processed.
	 */
	void HeaderValue(const RString& value);

public:

	//-----------------------------------------------------
	/** @name Stream Input Methods
	 * These methods are automatically called when a stream is read from a
	 * file.
	 */
	// @{

	/**
	 * Initialize the parser.
	 */
	void InitParser(void);

	/**
	 * Set the section type. It is necessary to specify where the XML stream
	 * is.
	 * @param section        Type of the section.
	 */
	inline void SetSection(SectionType section) {Section=section;}

	/**
	 * Reset the depth. Each time, the XML stream is on the top of the XML
	 * structure, this method must be called
	 */
	inline void ResetDepth(void) {CurDepth=0;}

	/**
	 * @return true of the spaces must be avoided.
	 */
	inline bool MustAvoidSpaces(void) const {return(AvoidSpaces);}

	/**
	 * @return true if the closing "</html>" tag was found.
	 */
	inline bool HasFoundClosingHTML(void) const {return(FoundClosingHTML);}

	/**
	 * @return the document type of the XML document.
	 */
	inline RString GetDocType(void) const {return(DocType);}

	/**
	* Set the document type of the XML document.
	* @param docType        Name of the type.
	*/
	virtual void SetDocType(const RString& docType);

	/**
	 * Add a entity found.
	 * @param name           Name of the entity.
	 * @param value          Corresponding value.
	 */
	virtual void AddEntity(const RString& name,const RString& value);

	/**
	* Method called each time a tag will be treated when reading a XML file.
	* @param namespaceURI    Namespace (if any).
	* @param lName           Local name of the tag.
	* @param name            Complete name of the tag.
	*/
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time a beginning tag is parsed (after the parsing of
	* the attributes).
	* @param namespaceURI    Namespace (if any).
	* @param lName           Local name of the tag.
	* @param name            Complete name of the tag.
	*/
	virtual void BeginTagParsed(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time a tag defines a unknown namespace which is
	* resolved by one of its attributes.
	* @param namespaceURI    Namespace to assign to the current tag.
	*/
	virtual void ResolveNamespace(const RString& namespaceURI);

	/**
	* Method called each time an attribute will be treated when reading a XML
	* file.
	* @param namespaceURI    Namespace (if any).
	* @param lName           Local name of the attribute.
	* @param name            Complete name of the attribute.
	 */
	virtual void AddAttribute(const RString& namespaceURI,const RString& lName,const RString& name);

	/**
	* Method called each time some attribute value elements (words or spaces)
	* are parsed when reading a XML file.
	* @param value           Value processed.
	 */
	virtual void Value(const RString& value);

	/**
	* Method called each time a tag was treated when reading a XML file.
	* @param namespaceURI  Namespace (if any).
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
	//@} Stream Input Methods

	/**
	* Method that specify if invalid XML codes (sequences beginning with a '&'
	* and finish with a ';') are accepted.
	* By default, this function return false which is the syntax of XML. This
	* can be changed with the method 'SetInvalidXMLCodes'.
	*/
	bool AcceptInvalidXMLCodes(void) {return(InvalidXMLCodes);}

	/**
	 * Specify if invalid XML codes should be accepted.
	 * @param accepted       Yes/No.
	 */
	void SetInvalidXMLCodes(bool accepted) {InvalidXMLCodes=accepted;}

	/**
	* Destruct the XML file.
	*/
	virtual ~RXMLParser(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
