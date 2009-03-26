/*

	R Project Library

	RDb.cpp

	Generic Database - Implementation.

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
// include files for ANSI C/C++
#include <stdarg.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rdb.h>
#include <rquery.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RDb
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDb::RDb(Db type)
	: Type(type)
{
}


//------------------------------------------------------------------------------
/*void RDb::CreateMySQLDatabase(const RString& host,const RString& user,const RString& pwd,const RString& name)
{
	MYSQL ms;
	MYSQL* ret(mysql_init(&ms));
	if((!ret)||(mysql_errno(&ms)))
		throw RDbException(mysql_error(&ms));

	ret=mysql_real_connect(&ms,host,user,pwd,0,0,0,0);
	if((!ret)||(mysql_errno(&ms)))
		throw RDbException(mysql_error(&ms));

	RString sql="CREATE DATABASE IF NOT EXISTS ";
	sql+=name;
	if(mysql_query(&ms,sql.Latin1()))
		throw RDbException(mysql_error(&ms));
}*/


//------------------------------------------------------------------------------
void RDb::CreateTransactionTable(const RString& name,size_t nb,...)
{
	va_list ap;
	RString sSql;

	// Create table if it doesn't already exist
	sSql="CREATE TABLE IF NOT EXISTS "+name+" (transid INT(11) PRIMARY KEY AUTO_INCREMENT,";
	va_start(ap,nb);
	while(nb--)
	{
		sSql+=RString(va_arg(ap,char*))+" TEXT";
		if(nb)
			sSql+=",";
	}
	va_end(ap);
	sSql+=")";
	RQuery Create(this,sSql);
}


//------------------------------------------------------------------------------
RDb::~RDb(void)
{
}
