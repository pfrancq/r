/*

	R Project Library

	RLang.h

	Language - Implementation.

	(c) 2001 by P. Francq.

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



//-----------------------------------------------------------------------------
// include file for HyperPRISME
#include "rinter/rlang.h"
using namespace RInter;



//---------------------------------------------------------------------------
//
// class RLang
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RInter::RLang::RLang(const RString& lang,const char* code) throw(bad_alloc)
  : Lang(lang)
{
  memcpy(Code,code,2*sizeof(char));
  Code[2]=0;
}


//---------------------------------------------------------------------------
int RInter::RLang::Compare(const RLang& lang) const
{
  return(Lang.Compare(lang.Lang));
}


//---------------------------------------------------------------------------
int RInter::RLang::Compare(const RLang *lang) const
{
  return(Lang.Compare(lang->Lang));
}


//---------------------------------------------------------------------------
RInter::RLang::~RLang(void)
{
}