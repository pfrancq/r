/*

  RDebug.h

  Debugging file in XML format (eXtended Markup Language) - Header

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


//---------------------------------------------------------------------------
// Standard libraries includes
#include <new.h>
#include <string.h>
#ifdef unix
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Generic Debug File (XML format)
class RDebug
{
  char *FileName;
  int FileHandle;
  unsigned Inc;
public:
  RDebug(void);
  void Init(char*) throw(bad_alloc);
  inline void PrintBlanks(void);
  inline void PrintNL(void);
  void Print(char*);                                // Print an Info
  void PrintTag(char*);                             // Print in a Tag
  void BeginTag(char *,unsigned NbAttr=0,...);      // Begin a Tag
  void EndTag(char *);                              // End a Tag
  void AddAttribute(char *Value,char *Att);         // Add an attribute
  void BeginFunc(char *Name,char *Object);          // Begin a Tag "Object.Name"
  void EndFunc(char *Name,char *Object);            // End a Tag "Object.Name"
  void BeginApp(char *App,char *Author);            // Begin a Tag "App"
  void EndApp(char *App,char *Author);              // End a Tag "App"
  void Done(void);
  ~RDebug(void);
};


}//------- End of namespace nDGA --------------------------------------------


//---------------------------------------------------------------------------
#endif
