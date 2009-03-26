/*

	R Project Library

	RQuery.cpp

	Generic Query - Implementation.

	Copyright 2000-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Library
#include <rquery.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
//
// RQuery
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RQuery::RQuery(RDb* db,const RString& sql)
	: Db(db), SQL(sql), Data(0)
{
	if(!Db)
		throw RDbException("RQuery::RQuery(RDb*,const RString&) : Null database");
	Data=Db->InitQuery(sql,NbCols);
}


//------------------------------------------------------------------------------
RString RQuery::SQLValue(const RString val)
{
	RString ret("'");
	const RChar* ptr;

	for(ptr=val();!ptr->IsNull();ptr++)
	{
		if((*ptr)==RChar('\''))
			ret+='\'';
		if((*ptr)==RChar('\\'))
			ret+='\\';
		ret+=(*ptr);
	}
	ret+=RChar('\'');
	return(ret);
}


//------------------------------------------------------------------------------
RString RQuery::SQLValue(const RDate& d)
{
	return("'"+d+"'");
}


//------------------------------------------------------------------------------
RQuery::~RQuery(void)
{
	Db->ReleaseQuery(Data);
}
