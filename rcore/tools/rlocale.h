/*

	R Project Library

	RLocale.h

	Internationalisation - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef RLocaleH
#define RLocaleH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>



//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RLocale class provides a representation for a particular location.
* @author Pascal Francq
* @short Locale.
*/
class RLocale
{
	/**
	* The name of the locale.
	*/
	RString Name;

public:

	/**
	* Construct the locale.
	* @param name                     Name.
	*/
	RLocale(const RString& name);

	/**
	* Construct the locale.
	* @param name                     Name.
	*/
	RLocale(const char* name);

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RLocale& l) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RLocale* l) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString& n) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const char* n) const;

	/**
	* Equal operator.
	*/
	bool operator==(const RLocale& l) const;

	/**
	* Equal operator.
	*/
	bool operator==(const RString& n) const;

	/**
	* Equal operator.
	*/
	bool operator==(const char* n) const;

	/**
	* Destruct the locale.
	*/
	virtual ~RLocale(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
