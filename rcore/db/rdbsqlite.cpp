/*

	R Project Library

	RDbSQLite.cpp

	SQLite Database - Implementation.

	Copyright 2000-2011 by Pascal Francq (pascal@francq.info).
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
#include <rdbsqlite.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Query Structure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RQuerySQLite
{
public:
	sqlite3_stmt* Result;
	int Code;

	RQuerySQLite(void) : Result(0), Code(0) {}
	~RQuerySQLite(void) {if(Result) sqlite3_finalize(Result);}
};



//------------------------------------------------------------------------------
//
// RDbSQLite
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDbSQLite::RDbSQLite(const RURI& db)
	: RDb(RDb::SQLite), Connection(0)
{
	Code=sqlite3_open(db.GetPath(),&Connection);
	if(Code)
		throw RDbException(sqlite3_errmsg(Connection));
}


//------------------------------------------------------------------------------
size_t RDbSQLite::GetLastInsertId(void)
{
	return(sqlite3_last_insert_rowid(Connection));
}


//------------------------------------------------------------------------------
void* RDbSQLite::InitQuery(const RString& sql,size_raw& nbcols)
{
	RQuerySQLite* Data(new RQuerySQLite());
	if(!Connection)
		throw RDbException("Database not initialize");
	if(sql.IsEmpty())
		throw RDbException("Empty SQL");

	const void *pzTail;
	Data->Code=sqlite3_prepare16(Connection,sql(),-1,&Data->Result,&pzTail);
	if(Data->Code!=SQLITE_OK)
		throw RDbException(sqlite3_errmsg(Connection));

	Data->Code=sqlite3_step(Data->Result);
	nbcols=sqlite3_column_count(Data->Result);
	return(Data);
}


//------------------------------------------------------------------------------
void RDbSQLite::ReleaseQuery(void* data)
{
	delete static_cast<RQuerySQLite*>(data);
}


//------------------------------------------------------------------------------
bool RDbSQLite::EndQuery(const void* data)
{
	return(static_cast<const RQuerySQLite*>(data)->Code==SQLITE_DONE);
}


//------------------------------------------------------------------------------
void RDbSQLite::StartQuery(void*)
{
}


//------------------------------------------------------------------------------
void RDbSQLite::NextQuery(void* data)
{
	static_cast<RQuerySQLite*>(data)->Code=sqlite3_step(static_cast<RQuerySQLite*>(data)->Result);
}


//------------------------------------------------------------------------------
RString RDbSQLite::GetField(const void* data,size_t index)
{
	if(static_cast<const RQuerySQLite*>(data)->Code==SQLITE_DONE)
		throw RDbException("Treated set");
	return(RString(static_cast<const RChar*>(sqlite3_column_text16(static_cast<const RQuerySQLite*>(data)->Result,static_cast<int>(index)))));
}


//------------------------------------------------------------------------------
RDbSQLite::~RDbSQLite(void)
{
	if(Connection)
		sqlite3_close(Connection);
}
