/*

	R Project Library

	RQt.cpp

	Generic function used for Qt compatibility - Implementation.

	Copyright 2004-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
/*	const RChar* ptr;
	size_t i;
	uint len(static_cast<uint>(str.GetLen()));
	QString ret;

	for(i=0,ptr=str();i<len;ptr++,i++)
		ret+=ptr->Unicode();
	return(ret);*/
	return(QString::fromUtf8(str));
}


//------------------------------------------------------------------------------
RString R::FromQString(const QString& str)
{
	RString ret;
	const RChar* ptr;
	size_t i;
	uint len(str.length());

	ret.SetLen(len);
	for(i=0,ptr=ret();i<len;i++,ptr++)
		(*const_cast<RChar*>(ptr))=str.at(i).unicode();
	return(ret);
}
