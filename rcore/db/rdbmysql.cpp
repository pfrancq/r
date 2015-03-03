/*

	R Project Library

	RDbMySQL.cpp

	MySQL Database - Implementation.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
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
#include <rdbmysql.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for MySQL
#if (defined(_BSD_SOURCE) || defined(__GNUC__) ) && !defined(WIN32) && !defined(__APPLE__)
    #include <mysql/mysql.h>
#else
    #include <mysql.h>
#endif



//------------------------------------------------------------------------------
//
// Query Structure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RQueryMySQL
{
public:
	MYSQL_RES* Result;
	MYSQL_ROW Row;   // Actual Row

	RQueryMySQL(void) : Result(0) {}
	~RQueryMySQL(void) {mysql_free_result(Result);}
};



//------------------------------------------------------------------------------
//
// RDbMySQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDbMySQL::RDbMySQL(const RString& db,const RString& host,const RString& user,const RString& pwd,const RCString& coding)
	: RDb(RDb::MySQL), Db(0), Coding(0)
{
	try
	{
		Coding=RTextEncoding::GetTextEncoding(coding);
	}
	catch(REncodingException& e)
	{
		throw RDbException(RString(e.GetMsg())+" for database "+db);
	}
	Db=static_cast<void*>(mysql_init(NULL));
	if((!Db)||(mysql_errno(static_cast<MYSQL*>(Db))))
		throw RDbException(mysql_error(static_cast<MYSQL*>(Db)));
	if(mysql_options(static_cast<MYSQL*>(Db),MYSQL_SET_CHARSET_NAME,coding))
		throw RDbException("Coding '"+RString(coding())+"' is not supported for database "+db);
	if((!mysql_real_connect(static_cast<MYSQL*>(Db),host,user,pwd,db,0,"",0))||(mysql_errno(static_cast<MYSQL*>(Db))))
		throw RDbException(mysql_error(static_cast<MYSQL*>(Db)));
}


//------------------------------------------------------------------------------
void RDbMySQL::Create(const RString& db,const RString& host,const RString& user,const RString& pwd)
{
	MYSQL ms;
	MYSQL* ret(mysql_init(&ms));
	if((!ret)||(mysql_errno(&ms)))
		throw RDbException(mysql_error(&ms));

	ret=mysql_real_connect(&ms,host,user,pwd,0,0,0,0);
	if((!ret)||(mysql_errno(&ms)))
		throw RDbException(mysql_error(&ms));

	RString sql="CREATE DATABASE IF NOT EXISTS ";
	sql+=db;
	if(mysql_query(&ms,sql.ToLatin1()))
		throw RDbException(mysql_error(&ms));
}


//------------------------------------------------------------------------------
size_t RDbMySQL::GetLastInsertId(void)
{
	RQuery LastInsert(this,"SELECT LAST_INSERT_ID()");
	LastInsert.Start();
	return(atoi(LastInsert[0]));
}


//------------------------------------------------------------------------------
void* RDbMySQL::InitQuery(const RString& sql,size_raw& nbcols)
{
	RQueryMySQL* Data(new RQueryMySQL());
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

	if(mysql_real_query(static_cast<MYSQL*>(Db),SQL_utf8,SQL_utf8.GetLen()))
		throw RDbException("Error in query "+sql+": "+mysql_error(static_cast<MYSQL*>(Db)));

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
		Data->Result=mysql_store_result(static_cast<MYSQL*>(Db));
		if(!Data->Result)
			throw RDbException(mysql_error(static_cast<MYSQL*>(Db)));
		nbcols=mysql_num_fields(Data->Result);
	}
	else
	{
		nbcols=0;
		Data->Row=0;
		Data->Result=0;
	}
	return(Data);
}


//------------------------------------------------------------------------------
void RDbMySQL::ReleaseQuery(void* data)
{
	delete static_cast<RQueryMySQL*>(data);
}


//------------------------------------------------------------------------------
bool RDbMySQL::EndQuery(const void* data)
{
	return(!static_cast<const RQueryMySQL*>(data)->Row);
}


//------------------------------------------------------------------------------
void RDbMySQL::StartQuery(void* data)
{
	static_cast<RQueryMySQL*>(data)->Row=mysql_fetch_row(static_cast<RQueryMySQL*>(data)->Result);
}


//------------------------------------------------------------------------------
void RDbMySQL::NextQuery(void* data)
{
	static_cast<RQueryMySQL*>(data)->Row=mysql_fetch_row(static_cast<RQueryMySQL*>(data)->Result);
}


//------------------------------------------------------------------------------
RString RDbMySQL::GetField(const void* data,size_t index)
{
	const char* val;
	RString res;

	if(!static_cast<const RQueryMySQL*>(data)->Row)
		throw RDbException("Treated set");
	val=static_cast<const RQueryMySQL*>(data)->Row[index];
	if(val)
	{
		res=Coding->ToUnicode(val,strlen(val));
	}
	return(res);
}


//------------------------------------------------------------------------------
RDbMySQL::~RDbMySQL(void)
{
	if(Db)
		mysql_close(static_cast<MYSQL*>(Db));
}
