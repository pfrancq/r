/*

	Rainbow Library Project

	RDebug.h

	Debugging structure and file in XML format (eXtended Markup Language) - Header

	(C) 1998-2000 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef RDebugH
#define RDebugH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


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
	char tmpOpt[2000];

	/**
	* Number of options.
	*/
	unsigned int NbOptions;

protected:

	/**
	* Deep Level (if -1 -> Nothing).
	*/
	int Deep;

	/**
	* If LevelOutput[i]=true -> i+1 not null.
	*/
	bool LevelOutput[50];

public:

	/**
	* Construct a debugger output.
	*/
	RDebug(void);

	/**
	* Begin a Tag with attributes.
	* @param Text			The name of the Tag.
	* @param NbAttr		The number of attributes.
	* The attributes are added after.
	*/
	void BeginTag(char *Text,unsigned NbAttr=0,...);

	/**
	* Print some Text in the current tag.
	*/
	void PrintComment(char *Text);

	/**
	* End a Tag.
	*/
	void EndTag(char *Text);

	/**
	* Print an Info Tag.
	*/
	void PrintInfo(char *Text);                             								

	/**
	* Begin a member function tag.
	* @param Name 		The name of the member function.
	* @param Object		The name of the object.		
	*/
	void BeginFunc(char *Name,char *Object);

	/**
	* End a member function tag.
	* @param Name 		The name of the member function.
	* @param Object		The name of the object.		
	*/
	void EndFunc(char *Name,char *Object);

	/**
	* Begin an application tag.
	* @param App			The name of the application.
	* @param Author		The auhtor of the application.
	*/
	void BeginApp(char *App,char *Author);

	/**
	* End an application tag.
	* @param App			The name of the application.
	* @param Author		The auhtor of the application.
	*/
	void EndApp(char *App,char *Author);

protected:

	/**
	* Add an attribute to buf.
	*/
	void AddAttribute(char* buf,char *Value,char *Att);

	/**
	* Begin to write a Tag. This function must be implement.
	*/
	virtual void WriteBeginTag(char *tag,char* options=0)=0;

	/**
	* Write Text associate with current tag. This function must be implement.
	*/
	virtual void WriteText(char *text)=0;

	/**
	* End to write a Tag. This function must be implement.
	*/
	virtual void WriteEndTag(char *tag)=0;

public:

	/**
	* Destruct the debugger output.
	*/
	virtual ~RDebug(void);
};


//-----------------------------------------------------------------------------
/**
* \ingroup GA
* This class implement a debuuger file in XML format.
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
	int Handle;

	/**
	* Temporary Variables for Internal use.
	*/
	char tmpTab[50];

	/**
	* Temporary Variables for Internal use.
	*/
	char tmpNL[3];

	/**
	* Temporary Variables for Internal use.
	*/
	char tmpLenNL;	

public:

	/**
	* Construct the debug file.
	* @param name		The name of the file.
	*/
	RDebugXML(const RString &name) throw(bad_alloc);	

protected:

	/**
	* Begin to write a Tag.
	*/
	virtual void WriteBeginTag(char *tag,char* options=0);

	/**
	* Write Text associate with current tag.
	*/
	virtual void WriteText(char *text);

	/**
	* End to write a Tag.
	*/
	virtual void WriteEndTag(char *tag);

public:

	/**
	* Destruct the debug file.
	*/
	virtual ~RDebugXML(void);
};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
