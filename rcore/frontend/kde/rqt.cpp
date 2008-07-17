/*

	RQt.cpp

	Generic function used for Qt compatibility - Implementation.

	Copyright 2004 by the Universit�Libre de Bruxelles.

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
// include files for R
#include <rqt.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Generic Functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QString R::ToQString(const RString& str)
{
	QString ret;
	const RChar* ptr;
	size_t i;
	size_t len=str.GetLen();

	ret.setLength(len);
	for(i=0,ptr=str();i<len;ptr++,i++)
		ret[i]=ptr->Unicode();
	return(ret);
}


//------------------------------------------------------------------------------
RString R::FromQString(const QString& str)
{
	RString ret;
	const RChar* ptr;
	size_t i;
	size_t len=str.length();

	ret.SetLen(len);
	for(i=0,ptr=ret();i<len;i++,ptr++)
		(*const_cast<RChar*>(ptr))=str[i].unicode();
	return(ret);
}
