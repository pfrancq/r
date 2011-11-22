/*

	R Project Library

	RDbPostgreSQL.cpp

	MySQL Database - Implementation.

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
#include <rdbpostgres.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Query Structure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RQueryPostgresSQL
{
public:
	PGresult* Result;  // Results
	size_t NbRows;     // Number of rows
	size_t Row;        // Actual Row
	size_t NbFields;   // Number of fields

	RQueryPostgresSQL(void) : Result(0) {}
	~RQueryPostgresSQL(void) {if(Result) PQclear(Result);}
};



//------------------------------------------------------------------------------
//
// RDbPostgreSQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDbPostgreSQL::RDbPostgreSQL(const RString& db,const RString& host,const RString& user,const RString& pwd,const RCString& coding)
	: RDb(RDb::PostgreSQL), Db(0), Coding(0)
{
	try
	{
		Coding=RTextEncoding::GetTextEncoding(coding);
	}
	catch(EncodingNotSupported e)
	{
		throw RDbException(RString(e.GetMsg())+" for database "+db);
	}

	// Information to pass for the connection
	RString Info("dbname="+db+" user="+user+" password="+pwd);

	// Look if the host is an adress or a name
	bool HasLetters(false);
	RCharCursor Cur(host);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur().IsAlpha())
		{
			HasLetters=true;
			break;
		}
	if(HasLetters)
		Info+="host="+host;
	else
		Info+="hostaddr="+host;
	RCString Info_utf8;
	try
	{
		Info_utf8=Coding->FromUnicode(Info);
	}
	catch(...)
	{
		throw RDbException("Error in parameters");
	}

	Db=PQconnectdb(Info_utf8);
	if(PQstatus(Db)!=CONNECTION_OK)
		throw RDbException(PQerrorMessage(Db));
}


//------------------------------------------------------------------------------
void RDbPostgreSQL::Create(const RString&/* db*/,const RString&/* host*/,const RString&/* user*/,const RString&/*pwd*/)
{
	throw RDbException("Not implemented for PostgreSQL");
//	MYSQL ms;
//	MYSQL* ret(mysql_init(&ms));
//	if((!ret)||(mysql_errno(&ms)))
//		throw RDbException(mysql_error(&ms));
//
//	ret=mysql_real_connect(&ms,host,user,pwd,0,0,0,0);
//	if((!ret)||(mysql_errno(&ms)))
//		throw RDbException(mysql_error(&ms));
//
//	RString sql="CREATE DATABASE IF NOT EXISTS ";
//	sql+=db;
//	if(mysql_query(&ms,sql.Latin1()))
//		throw RDbException(mysql_error(&ms));
}


//------------------------------------------------------------------------------
size_t RDbPostgreSQL::GetLastInsertId(void)
{
	RQuery LastInsert(this,"SELECT LAST_INSERT_ID()");
	LastInsert.Start();
	return(atoi(LastInsert[0]));
}


//------------------------------------------------------------------------------
void* RDbPostgreSQL::InitQuery(const RString& sql,size_raw& nbcols)
{
	RQueryPostgresSQL* Data(new RQueryPostgresSQL());
	if(!Db)
		throw RDbException("Database not initialize");
	if(sql.IsEmpty())
		throw RDbException("Empty SQL");

	const RChar* ptr;
	size_t size,pos;
	RCString SQL_utf8;
	RString cmd;

	try
	{
		SQL_utf8=Coding->FromUnicode(sql);
	}
	catch(...)
	{
		throw RDbException("Error in encoded data: "+sql);
	}

	// Run the SQL data
	Data->Result=PQexec(Db,SQL_utf8);

	// Find the SQL cmd
	ptr=sql();
	pos=0;
	while((!ptr->IsNull())&&(ptr->IsSpace()))  // Skip Spaces
	{
		ptr++;
		pos++;
	}
	size=0;
	while((!ptr->IsNull())&&(ptr->IsAlpha())&&(size<7))
	{
		ptr++;
		size++;
	}
	cmd=sql.Mid(pos,size).ToUpper();

	// It is a SELECT, a DESC or a SHOW command -> retrieve results
	if((size<7)&&((cmd=="SELECT")||(cmd=="SHOW")||(cmd=="DESC")))
	{
		// Verify if the command retrieve something
	   if (PQresultStatus(Data->Result) != PGRES_TUPLES_OK)
		{
			delete Data;
			throw RDbException(PQerrorMessage(Db));
		}

		Data->NbRows=PQntuples(Data->Result);
		nbcols=Data->NbFields=PQnfields(Data->Result);
		Data->Row=0;
	}
	else
	{
		// Verify if the command retrieve something
		if(PQresultStatus(Data->Result)!=PGRES_COMMAND_OK)
		{
			delete Data;
			throw RDbException(PQerrorMessage(Db));
	    }

		Data->Row=0;
		Data->NbRows=0;
		nbcols=Data->NbFields=0;
	}
	return(Data);
}


//------------------------------------------------------------------------------
void RDbPostgreSQL::ReleaseQuery(void* data)
{
	RQueryPostgresSQL* Data(static_cast<RQueryPostgresSQL*>(data));
	delete Data;
}


//------------------------------------------------------------------------------
bool RDbPostgreSQL::EndQuery(const void* data)
{
	return(static_cast<const RQueryPostgresSQL*>(data)->Row>=static_cast<const RQueryPostgresSQL*>(data)->NbRows);
}


//------------------------------------------------------------------------------
void RDbPostgreSQL::StartQuery(void* data)
{
	static_cast<RQueryPostgresSQL*>(data)->Row=0;
}


//------------------------------------------------------------------------------
void RDbPostgreSQL::NextQuery(void* data)
{
	static_cast<RQueryPostgresSQL*>(data)->Row++;
}


//------------------------------------------------------------------------------
RString RDbPostgreSQL::GetField(const void* data,size_t index)
{
	char* val;
	RString res;

	if(static_cast<const RQueryPostgresSQL*>(data)->Row>=static_cast<const RQueryPostgresSQL*>(data)->NbRows)
		throw RDbException("Treated set");
	if(index>=static_cast<const RQueryPostgresSQL*>(data)->NbFields)
		throw RDbException("There is only "+RString::Number(static_cast<const RQueryPostgresSQL*>(data)->NbFields)+" fields");
	val=PQgetvalue(static_cast<const RQueryPostgresSQL*>(data)->Result,static_cast<const RQueryPostgresSQL*>(data)->Row,index);
	if(val)
	{
		res=Coding->ToUnicode(val,strlen(val));
	}
	return(res);
}


//------------------------------------------------------------------------------
RDbPostgreSQL::~RDbPostgreSQL(void)
{
	if(Db)
		PQfinish(Db);
}
