/*

	R Project Library

	RDebug.h

	Debugging structure and file in XML format (eXtended Markup Language) - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* \ingroup GA
* This file represent a generic debug output for the GA.
* @author Pascal Francq
* @short Generic Debug Output.
*/
class RDebug
{
	/**
	* Temporay Data for internal use.
	*/
	RString tmpOpt;

	/**
	* Number of options.
	*/
	unsigned int NbOptions;

	/**
	* String containing the current tag.
	*/
	RString CurTag;

protected:

	/**
	* Deep Level (if -1 -> Nothing).
	*/
	int Deep;

	/**
	* If LevelOutput[i]=true -> i+1 not null.
	*/
	bool LevelOutput[50];

	/**
	* Name of the application to debug.
	*/
	RString App;

	/**
	* Name of the person which is generating the debug.
	*/
	RString Author;

public:

	/**
	* Construct a debugger output.
	* @param app            The name of the application.
	* @param author         The auhtor of the application.
	*/
	RDebug(RString app,RString author);

	/**
	* Begin a Tag with attributes. The attributes are added after.
	* @param Text           The name of the Tag.
	* @param NbAttr         The number of attributes.
	*/
	void BeginTag(RString Text,unsigned NbAttr=0,...);

	/**
	* Print some Text in the current tag.
	*/
	void PrintComment(RString Text);

	/**
	* End a Tag.
	*/
	void EndTag(RString Text);

	/**
	* Get the name of the current tag.
	* @return const char*
	*/
	RString GetCurrentTag(void) const { return(CurTag);}

	/**
	* Print an Info Tag.
	*/
	void PrintInfo(RString Text);

	/**
	* Begin a member function tag.
	* @param Name           The name of the member function.
	* @param Object         The name of the object.
	*/
	void BeginFunc(RString Name,RString Object);

	/**
	* End a member function tag.
	* @param Name           The name of the member function.
	* @param Object         The name of the object.
	*/
	void EndFunc(RString Name,RString Object);

	/**
	* Begin an application tag.
	*/
	void BeginApp(void);

	/**
	* End an application tag.
	*/
	void EndApp(void);

protected:

	/**
	* Add an attribute to buf.
	*/
	void AddAttribute(RString* Value,RString* Att);

	/**
	* Begin to write a Tag. This function must be implement.
	*/
	virtual void WriteBeginTag(RString tag,RString options=RString::Null)=0;

	/**
	* Write Text associate with current tag. This function must be implement.
	*/
	virtual void WriteText(RString text)=0;

	/**
	* End to write a Tag. This function must be implement.
	*/
	virtual void WriteEndTag(RString tag)=0;

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
* @author Pascal Francq
* @short Debug XML file.
*/
class RDebugXML : public RDebug
{
	/**
	* The name of the file.
	*/
	RString Name;

	/**
	* The handle of the file.
	*/
	RTextFile File;

	/**
	* Temporary Variables for Internal use.
	*/
	RString tmpTab;

public:

	/**
	* Construct the debug file.
	* @param name           The name of the file.
	* @param app            The name of the application.
	* @param author         The auhtor of the application.
	*/
	RDebugXML(RString name,RString app,RString author) throw(std::bad_alloc);

protected:

	/**
	* Begin to write a Tag.
	*/
	virtual void WriteBeginTag(RString tag,RString options=RString::Null);

	/**
	* Write Text associate with current tag.
	*/
	virtual void WriteText(RString text);

	/**
	* End to write a Tag.
	*/
	virtual void WriteEndTag(RString tag);

public:

	/**
	* Destruct the debug file.
	*/
	virtual ~RDebugXML(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
