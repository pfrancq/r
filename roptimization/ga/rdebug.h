/*

  RDebug.h

  Debugging structure and file in XML format (eXtended Markup Language) - Header

  (C) 1998-2000 by P. Francq.

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
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


//---------------------------------------------------------------------------
#ifndef RDebugH
#define RDebugH


#ifndef NULL
	#define NULL 0
#endif


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rstring.h"
using namespace RStd;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Generic Debug File
class RDebug
{
	char tmpOpt[2000];
	unsigned int NbOptions;
protected:
	int Deep;																													// Deep Level (if -1 -> Nothing)
	bool LevelOutput[50];																							// If LevelOutput[i]=true -> i+1 not null
public:
  RDebug(void);
  void BeginTag(char *,unsigned NbAttr=0,...);  	   								// Begin a Tag with attributes
	void PrintComment(char*);																					// Print a comment for the tag
  void EndTag(char *);                              								// End a Tag
  void PrintInfo(char*);                             								// Print an Info
  void BeginFunc(char *Name,char *Object);          								// Begin a Tag "Object.Name"
  void EndFunc(char *Name,char *Object);            								// End a Tag "Object.Name"
  void BeginApp(char *App,char *Author);            								// Begin a Tag "App"
  void EndApp(char *App,char *Author);              								// End a Tag "App"
protected:
  void AddAttribute(char* buf,char *Value,char *Att);								// Add an attribute to buf
  virtual void WriteBeginTag(char *tag,char* options=NULL)=0;				// Begin a Tag
	virtual void WriteText(char *text)=0;															// Write Text associate with current tag
  virtual void WriteEndTag(char *tag)=0;														// End a Tag
public:
  virtual ~RDebug(void);
};


//---------------------------------------------------------------------------
// Generic Debug File
class RDebugXML : public RDebug
{
	RString Name;
	int Handle;
	char tmpTab[50],tmpNL[3],tmpLenNL;	

public:
	RDebugXML(const RString &name) throw(bad_alloc);	

protected:
  virtual void WriteBeginTag(char *tag,char* options=NULL);					// Begin a Tag
	virtual void WriteText(char *text);																// Write Text associate with current tag
  virtual void WriteEndTag(char *tag);															// End a Tag

public:
	virtual ~RDebugXML(void);
};


}//------- End of namespace nDGA --------------------------------------------


//---------------------------------------------------------------------------
#endif
