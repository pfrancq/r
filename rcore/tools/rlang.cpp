/*

	R Project Library

	RLang.h

	Language - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <string.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rlang.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLang::RLang(const RString& lang,const char* code) throw(std::bad_alloc)
  : Lang(lang)
{
  memcpy(Code,code,2*sizeof(char));
  Code[2]=0;
}


//------------------------------------------------------------------------------
int RLang::Compare(const RLang& lang) const
{
  return(strcmp(Code,lang.Code));
}


//------------------------------------------------------------------------------
int RLang::Compare(const RLang *lang) const
{
  return(strcmp(Code,lang->Code));
}


//------------------------------------------------------------------------------
int RLang::Compare(const char* code) const
{
  return(strcmp(Code,code));
}


//------------------------------------------------------------------------------
RLang::~RLang(void)
{
}
