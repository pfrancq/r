/*

	R Project Library

	RLang.h

	Language - Header.

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
#ifndef RLangH
#define RLangH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RInter{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RLang class provides a representation for a particular language.
* @author Pascal Francq
* @short Language.
*/
class RLang
{
	/**
	* Name of the Language.
	*/
	RString Lang;

	/**
	* Code of the language.
	*/
	char Code[3];

public:

	/**
	* Construct a language.
	* @param lang                     Name of the language.
	* @param code                     Code of the language.
	*/
	RLang(const RString& lang,const char* code) throw(bad_alloc);

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RLang& lang) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RLang* lang) const;

	/**
	* Destructor of the language.
	*/
	virtual ~RLang(void);
};


}  //-------- End of namespace RInter -----------------------------------------


//-----------------------------------------------------------------------------
#endif