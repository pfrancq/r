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
	int handle;
	char *Buffer;
	char *ptr;
	RString Name;
	bool All;
	
	inline void SkipSpaces(void);
public:
	RTextFile(const RString &name,bool all=true) throw(bad_alloc);
	inline bool Eof(void) { return(!(*ptr)); }
	long int GetInt(void) throw(RString);
	float GetFloat(void) throw(RString);
	char *GetWord(void);
	char *GetLine(void);		
	~RTextFile();
};


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif
