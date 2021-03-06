/*

	R Project Library

	RLang.h

	Language - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef RLang_H
#define RLang_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RLang class provides a representation for a particular language.
* @short Language.
*/
class RLang
{
protected:

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
	* @param lang            Name of the language.
	* @param code            Code of the language.
	*/
	RLang(const RString& lang,const char* code);

	/**
	* Compare two langs and return an integer less than, equal to, or greater
	* than zero if this is less than, equal to, or greater than lang.
	* @param lang            Language.
	* @see R::RContainer.
	*/
	int Compare(const RLang& lang) const;

	/**
	* Compare a lang and a code and return an integer less than, equal to, or
	* greater than zero if this is less than, equal to, or greater than code.
	* @param code            Code used for the comparaison.
	*/
	int Compare(const char* code) const;

	/**
	* Get the code of the language.
	* @returns Pointer to a C string containing the code.
	*/
	const char* GetCode(void) const {return(Code);}

	/**
	* Get the name of the language.
	* @returns Pointer to a C string containing the name.
	*/
	RString GetName(void) const {return(Lang);}

	/**
	* Destructor of the language.
	*/
	virtual ~RLang(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
