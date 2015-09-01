/*

	R Project Library

	RDebug.h

	Debugging structure and file in XML format (eXtended Markup Language) - Header

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RDebug_H
#define RDebug_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rtextfile.h>
#include <rboolvector.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* \ingroup GA
* This file represent a generic debug output for the GA.
* @short Generic Debug Output.
*/
class RDebug
{
	/**
	* String containing the current tag.
	*/
	RString CurTag;

protected:

	/**
	* Depth (if -1 -> Nothing).
	*/
	int Depth;

	/**
	* If Depths[i]=true -> i+1 not null.
	*/
	RBoolVector Depths;

public:

	/**
	* Construct a debugger output.
	*/
	RDebug(void);

	/**
	 * Begin a Tag with attributes.
	 * @param tag           Name of the Tag.
	 * @param attrs         Attributes.
	 */
	void BeginTag(const RString& tag,const RString& attrs=RString::Null);

	/**
	 * Print some Text in the current tag.
	 * @param text           Text to add.
	*/
	void PrintComment(const RString& text);

	/**
	 * End a Tag.
	 * @param text           Name of the Tag.
	 */
	void EndTag(const RString& text);

	/**
	 * Get the name of the current tag.
	 */
	const RString& GetCurrentTag(void) const {return(CurTag);}

	/**
	 * Print an Info Tag.
	 * @param text           Text to add.
	 */
	void PrintInfo(const RString& text);

	/**
	 * Begin a member function tag.
	 * @param name           Name of the member function.
	 * @param object         Name of the object.
	 */
	void BeginFunc(const RString& name,const RString& object);

	/**
	 * End a member function tag.
	 * @param name           Name of the member function.
	 * @param object         Name of the object.
	 */
	void EndFunc(const RString& name,const RString& object);

	/**
	 * Begin an application tag.
	 * @param app             Name of the application.
	 */
	void BeginApp(const RString& app);

	/**
	 * End an application tag.
	 * @param app             Name of the application.
	 */
	void EndApp(const RString& app);

protected:

	/**
	 * Begin to write a Tag. This function must be implement.
	 * @param tag            Name of the tag.
	 * @param attrs          Attributes.
	 */
	virtual void WriteBeginTag(const RString& tag,const RString& attrs)=0;

	/**
	 * Write Text associate with current tag. This function must be implement.
	 * @param text           Text to add.
	 */
	virtual void WriteText(const RString& text)=0;

	/**
	 * End to write a Tag. This function must be implement.
	 * @param tag            Name of the tag.
	 */
	virtual void WriteEndTag(const RString& tag)=0;

public:

	/**
	 * Destruct the debugger output.
	 */
	virtual ~RDebug(void);
};


//------------------------------------------------------------------------------
/**
* \ingroup GA
* This class implement a debugger file in XML format.
* @short Debug XML file.
*/
class RDebugXML : public RDebug
{
	/**
	* The name of the file.
	*/
	RURI Name;

	/**
	* The handle of the file.
	*/
	RTextFile File;

	/**
	* Temporary Variables for Internal use.
	*/
	char* tmpTab;

	/**
	 * Size of the array.
	 */
	size_t tmpSizeTab;

public:

	/**
	 * Construct the debug file.
	 * @param name           The name of the file.
	 */
	RDebugXML(const RURI& name);

	/**
	 * Get the name of the file.
	 */
	RURI GetName(void) const;

protected:

	/**
	 * @param tag            Name of the tag.
	 * @param attrs          Attributes.
	 */
	virtual void WriteBeginTag(const RString& tag,const RString& attrs);

	/**
	 * Write Text associate with current tag.
	 * @param text           Text to add.
	 */
	virtual void WriteText(const RString& text);

	/**
	 * End to write a Tag.
	 * @param tag            Name of the tag.
	 */
	virtual void WriteEndTag(const RString& tag);

	/**
	 * Go to the next line in the file and write a given number of tabs.
	 * @param nb             Number of tabs.
	 */
	void WriteTabs(size_t nb);

public:

	/**
	 * Destruct the debug file.
	 */
	virtual ~RDebugXML(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
