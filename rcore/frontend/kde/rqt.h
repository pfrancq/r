/*

	R Project Library

	RQt.h

	Generic function used for Qt compatibility - Header.

	Copyright 2004 by the Université Libre de Bruxelles.

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
#ifndef RQT_H
#define RQT_H


//------------------------------------------------------------------------------
// include files for R
#include <rstring.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtCore/QString>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This function returns a QString (Qt library) corresponding to RString.
* @param str                String to transform.
* @returns QString.
*/
QString ToQString(const RString& str);


//------------------------------------------------------------------------------
/**
* This function returns a RString corresponding to a QString (Qt library).
* @param str                String to transform.
* @returns RString.
*/
RString FromQString(const QString& str);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif

