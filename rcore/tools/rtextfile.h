/*

  RTextFile.h

  Text File - Header.

  (C) 1999-2000 by P. Francq.

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
#ifndef RTEXTFILE_H
#define RTEXTFILE_H


//---------------------------------------------------------------------------
#include "rstring.h"
using namespace RStd;


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class RTextFile
{
  int Mode;           // How to file has to be used
	RString Name;       // Name of the File
	int handle;         // Internal Handle of the file
	char *Buffer;       // Buffer containing the file (Used only if Mode==modRead);
	char *ptr;          // Internal pointer used to read the file (Used only if Mode==modRead);
	bool All;           // All the file reads at once  (Only true is supported yet)
  bool NewLine;       // At NewLine?
  
	void SkipSpaces(void);
public:
	RTextFile(const RString &name,int mode) throw(bad_alloc,RString);
	RTextFile(const RString &name,bool all=true) throw(bad_alloc,RString);
  void Init(void) throw(bad_alloc,RString);

  // Methods for Reading
  void Begin(void) throw(RString);
	inline bool Eof(void) { return(!(*ptr)); }
	long int GetInt(void) throw(RString);
	float GetFloat(void) throw(RString);
	char *GetWord(void) throw(RString);
	char *GetLine(void) throw(RString);

  // Methods for Writing
  void WriteLine(void) throw(RString);
  void WriteLong(long nb) throw(RString);
  void WriteULong(unsigned long nb) throw(RString);
  void WriteStr(char *c) throw(RString);
  void WriteBool(bool b) throw(RString);
  void WriteTime(void) throw(RString);
  void WriteLog(char *entry) throw(RString);
  
	~RTextFile(void);

  static const int modRead=1;
  static const int modAppend=2;
  static const int modCreate=3;
};


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif
