/*

	R Project Library

	RMySQL.cpp

	MySQL C++ Classes - Implementation.

	Copyright 2000-2003 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <string.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rstd/rstring.h>
#include <rdb/rmysql.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RDb
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDb::RDb(const char* host,const char* user,const char* pwd,const char* db) throw(RMySQLError)
{
	MYSQL* ret;

	ret=mysql_init(&mysql);
	if((!ret)||(mysql_errno(&mysql)))
		throw RMySQLError(mysql_error(&mysql));
	connection=mysql_real_connect(&mysql,host,user,pwd,db,0,"",0);
	if((!connection)||(mysql_errno(&mysql)))
		throw RMySQLError(mysql_error(&mysql));
}


//------------------------------------------------------------------------------
RDb::~RDb(void)
{
	if(connection) mysql_close(connection);
}



//------------------------------------------------------------------------------
//
// RQuery
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RQuery::RQuery(RDb* db,const char* sql) throw(RMySQLError)
{
	const char* ptr;
	char* tmp;
	unsigned int size;
	char cmd[10];

	if((!db)||!(db->connection))
		throw RMySQLError("Database not initialize");
	if(mysql_real_query(db->connection,sql,strlen(sql)))
		throw RMySQLError(mysql_error(&db->mysql));

	ptr=sql;
	if(!(*ptr))
		throw RMySQLError("Empty SQL");

	// Skip spaces
	while((*ptr)&&(isspace(*ptr))) ptr++;

	// Find the command
	tmp=cmd;
	size=0;
	while((*ptr)&&(isalpha(*ptr))&&(size<7))
	{
		(*(tmp++))=(*(ptr++));
		size++;
	}

	// It is a SELECT command -> retrieve results

	if((size<7)&&(!strncmp(cmd,"SELECT",6)))
	{
		result=mysql_store_result(db->connection);
		nbrows=mysql_num_rows(result);
		nbcols=mysql_num_fields(result);
	}
	else
	{
		nbcols=nbrows=0;
		row=0;
		result=0;
	}
}


//------------------------------------------------------------------------------
void RQuery::Begin(void)
{
	row=mysql_fetch_row(result);
}


//------------------------------------------------------------------------------
void RQuery::Start(void)
{
	row=mysql_fetch_row(result);
}


//------------------------------------------------------------------------------
RQuery& RQuery::operator++(int)
{
	row=mysql_fetch_row(result);
	return(*this);
}


//------------------------------------------------------------------------------
void RQuery::Next(void)
{
	row=mysql_fetch_row(result);
}


//------------------------------------------------------------------------------
const char* RQuery::operator[](unsigned int index) const throw(RMySQLError)
{
	if(index>=nbcols)
		throw RMySQLError("Index out of range");
	if(!row)
		throw RMySQLError("Treated set");
	return(row[index]);
}


//------------------------------------------------------------------------------
RQuery::~RQuery(void)
{
	if(result) mysql_free_result(result);
}
