/*

	R Project Library

	RLocale.h

	Internationalisation - Header.

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
#ifndef RLocaleH
#define RLocaleH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>



//-----------------------------------------------------------------------------
namespace RInter{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
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
	RStd::RString Name;

public:

	/**
	* Construct the locale.
	* @param name                     Name.
	*/
	RLocale(const RStd::RString& name);

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
	int Compare(const RStd::RString& n) const;

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
	bool operator==(const RStd::RString& n) const;

	/**
	* Equal operator.
	*/
	bool operator==(const char* n) const;

	/**
	* Destruct the locale.
	*/
	virtual ~RLocale(void);
};


}  //-------- End of namespace RInter -----------------------------------------


//-----------------------------------------------------------------------------
#endif
