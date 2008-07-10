/*

	R Project Library

	RMySQL.cpp

	MySQL C++ Classes - Implementation.

	Copyright 2000-2005 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <string.h>
#include <stdarg.h>


//------------------------------------------------------------------------------
// include files for Databases
#include <mysql/mysql.h>
#include <sqlite3.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rcursor.h>
#include <rdb.h>
#include <rstring.h>
#include <rtextencoding.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// MySQL based classes
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RDbMySQL : public RDb
{
public:
	MYSQL* Connection;
	MYSQL MySQL;
	RTextEncoding* Coding; // Coding used to read/write to MySQL.

	RDbMySQL(Db type,const RString& host,const RString& user,const RString& pwd,const RString& db,const RString& coding);
	virtual RQuery* Query(const RString& sql);
	virtual ~RDbMySQL(void);
};


//------------------------------------------------------------------------------
class RQueryMySQL : public RQuery
{
public:
	MYSQL_RES* Result;
	MYSQL_ROW Row;   // Actual Row
	RDbMySQL* DB;

	RQueryMySQL(RDbMySQL* db,const RString& sql);
	virtual bool End(void) const {return(!Row);}
	virtual void Start(void);
	virtual void Next(void);
	virtual RString operator[](unsigned int index) const;
	virtual ~RQueryMySQL(void);
};



//------------------------------------------------------------------------------
//
// SQLite based classes
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RDbSQLite : public RDb
{
public:
	sqlite3* Connection;
	int Code;              // Result of the last operation

	RDbSQLite(Db type,const RString& db);
	virtual RQuery* Query(const RString& sql);
	virtual ~RDbSQLite(void);
};


//------------------------------------------------------------------------------
class RQuerySQLite : public RQuery
{
public:
	RDbSQLite* DB;
	sqlite3_stmt* Result;

	RQuerySQLite(RDbSQLite* db,const RString& sql);
	virtual bool End(void) const {return(DB->Code==SQLITE_DONE);}
	virtual void Start(void);
	virtual void Next(void);
	virtual RString operator[](unsigned int index) const;
	virtual ~RQuerySQLite(void);
};


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
void RDb::CreateMySQLDatabase(const RString& host,const RString& user,const RString& pwd,const RString& name)
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
}


//------------------------------------------------------------------------------
void RDb::CreateTransactionTable(const RString& name,unsigned int nb,...)
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
	auto_ptr<RQuery> create(this->Query(sSql));
}


//------------------------------------------------------------------------------
RDb* RDb::Get(Db type,const RString& db,const RString& host,const RString& user,const RString& pwd,const RString& coding)
{
	switch(type)
	{
		case MySQL:
			return(new RDbMySQL(MySQL,host,user,pwd,db,coding));
			break;
		case SQLite:
			return(new RDbSQLite(SQLite,db));
			return(0);
			break;
		default:
			return(0);
	}
}


//------------------------------------------------------------------------------
RDb::~RDb(void)
{
}



//------------------------------------------------------------------------------
//
// RDbMySQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDbMySQL::RDbMySQL(Db type,const RString& host,const RString& user,const RString& pwd,const RString& db,const RString& coding)
	: RDb(type), Coding(0)
{
	try
	{
		Coding=RTextEncoding::GetTextEncoding(coding);
	}
	catch(EncodingNotSupported e)
	{
		throw RDbException(RString(e.GetMsg())+" for database "+db);
	}
	MYSQL* ret=mysql_init(&MySQL);
	if((!ret)||(mysql_errno(&MySQL)))
		throw RDbException(mysql_error(&MySQL));
	Connection=mysql_real_connect(&MySQL,host,user,pwd,db,0,"",0);
	if((!Connection)||(mysql_errno(&MySQL)))
		throw RDbException(mysql_error(&MySQL));
}


//------------------------------------------------------------------------------
RQuery* RDbMySQL::Query(const RString& sql)
{
	return(new RQueryMySQL(this,sql));
}


//------------------------------------------------------------------------------
RDbMySQL::~RDbMySQL(void)
{
	if(Connection)
		mysql_close(Connection);
}



//------------------------------------------------------------------------------
//
// RDbSQLite
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDbSQLite::RDbSQLite(Db type,const RString& db)
	: RDb(type), Connection(0)
{
	Code=sqlite3_open(db,&Connection);
	if(Code)
		throw RDbException(sqlite3_errmsg(Connection));
}


//------------------------------------------------------------------------------
RQuery* RDbSQLite::Query(const RString& sql)
{
	return(new RQuerySQLite(this,sql));
}


//------------------------------------------------------------------------------
RDbSQLite::~RDbSQLite(void)
{
	if(Connection)
		sqlite3_close(Connection);
}



//------------------------------------------------------------------------------
//
// RQuery
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RQuery::RQuery(const RString& sql)
	: SQL(sql)
{
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
}



//------------------------------------------------------------------------------
//
// RQueryMySQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RQueryMySQL::RQueryMySQL(RDbMySQL* db,const RString& sql)
	: RQuery(sql), DB(db)
{
	if((!DB)||(!DB->Connection))
		throw RDbException("Database not initialize");
	if(SQL.IsEmpty())
		throw RDbException("Empty SQL");

	const RChar* ptr;
	unsigned int size,pos;
	RCString SQL_utf8;
	RString cmd;

	try
	{
		SQL_utf8=DB->Coding->FromUnicode(SQL);
	}
	catch(...)
	{
		throw RDbException("Error in encoded data: "+SQL);
	}

	if(mysql_real_query(DB->Connection,SQL_utf8,SQL_utf8.GetLen()))
		throw RDbException("Error in query "+SQL+": "+mysql_error(&DB->MySQL));

	// Find the SQL cmd
	ptr=SQL();
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
	cmd=SQL.Mid(pos,size).ToUpper();

	// It is a SELECT, a DESC or a SHOW command -> retrieve results
	if((size<7)&&((cmd=="SELECT")||(cmd=="SHOW")||(cmd=="DESC")))
	{
		Result=mysql_store_result(dynamic_cast<RDbMySQL*>(DB)->Connection);
		if(!Result)
			throw RDbException(mysql_error(&dynamic_cast<RDbMySQL*>(DB)->MySQL));
		NbRows=mysql_num_rows(Result);
		NbCols=mysql_num_fields(Result);
	}
	else
	{
		NbCols=NbRows=0;
		Row=0;
		Result=0;
	}
}


//------------------------------------------------------------------------------
void RQueryMySQL::Start(void)
{
	Row=mysql_fetch_row(Result);
}


//------------------------------------------------------------------------------
void RQueryMySQL::Next(void)
{
	Row=mysql_fetch_row(Result);
}


//------------------------------------------------------------------------------
RString RQueryMySQL::operator[](unsigned int index) const
{
	const char* val;
	RString res;

	if(index>=NbCols)
	{
		char tmp[80];
		sprintf(tmp,"RQuery::operator[] const : column %zu outside range [0,%zu]",index,NbCols-1);
		throw RDbException(tmp);
	}
	if(!Row)
		throw RDbException("Treated set");
	val=Row[index];
	if(val)
	{
		res=DB->Coding->ToUnicode(val,strlen(val));
	}
	return(res);
}


//------------------------------------------------------------------------------
RQueryMySQL::~RQueryMySQL(void)
{
}



//------------------------------------------------------------------------------
//
// RQuerySQLite
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RQuerySQLite::RQuerySQLite(RDbSQLite* db,const RString& sql)
	: RQuery(sql), DB(db), Result(0)
{
	if((!DB)||(!DB->Connection))
		throw RDbException("Database not initialize");
	if(SQL.IsEmpty())
		throw RDbException("Empty SQL");

	const void *pzTail;
	DB->Code=sqlite3_prepare16(DB->Connection,sql(),-1,&Result,&pzTail);
	if(DB->Code!=SQLITE_OK)
		throw RDbException(sqlite3_errmsg(DB->Connection));

	DB->Code=sqlite3_step(Result);
	NbRows=sqlite3_data_count(Result);
	NbCols=sqlite3_column_count(Result);
}


//------------------------------------------------------------------------------
void RQuerySQLite::Start(void)
{
}


//------------------------------------------------------------------------------
void RQuerySQLite::Next(void)
{
	DB->Code=sqlite3_step(Result);
}


//------------------------------------------------------------------------------
RString RQuerySQLite::operator[](unsigned int index) const
{
	if(index>=NbCols)
	{
		char tmp[80];
		sprintf(tmp,"RQuery::operator[] const : column %zu outside range [0,%zu]",index,NbCols-1);
		throw RDbException(tmp);
	}
	if(End())
		throw RDbException("Treated set");
	return(RString(static_cast<const RChar*>(sqlite3_column_text16(Result,index))));
}


//------------------------------------------------------------------------------
RQuerySQLite::~RQuerySQLite(void)
{
	if(Result)
		sqlite3_finalize(Result);
}



//------------------------------------------------------------------------------
//
// RTransactionTable
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTransactionTable::RTransactionTable(RDb* db,RString name)
	: Name(name), Params(20,10), DB(db)
{
	RString sSql;

	sSql="SHOW FIELDS FROM "+name;
	auto_ptr<RQuery> params(DB->Query(sSql));
	// Skip 'transid
	params->Start();
	for(params->Next();!params->End();params->Next())
		Params.InsertPtr(new RString(params->operator[](0)));
}


//------------------------------------------------------------------------------
RTransactionTable::RTransactionTable(RDb* db,RString name,unsigned int nb,...)
	: Name(name), Params(nb,5), DB(db)
{
	va_list ap;
	RString sSql;

	// Create table if it doesn't already exist
	sSql="CREATE TABLE IF NOT EXISTS "+name+" (transid INT(11) PRIMARY KEY AUTO_INCREMENT,";
	va_start(ap,nb);
	while(nb--)
	{
		sSql+=RString(va_arg(ap,char*))+" TEXT";
		Params.InsertPtr(new RString(va_arg(ap,char*)));
		if(nb)
			sSql+=",";
	}
	va_end(ap);
	sSql+=")";
	auto_ptr<RQuery> create(DB->Query(sSql));
}


//------------------------------------------------------------------------------
size_raw RTransactionTable::WriteTransaction(unsigned int id,...)
{
	va_list ap;
	RString sSql;

	// Insert the info in the table
	sSql="INSERT INTO "+Name+" SET ";
	if(id)
		sSql+="transid="+RString::Number(id)+",";
	va_start(ap,id);
	RCursor<RString> Cur(Params);
	for(Cur.Start();!Cur.End();)
	{
		sSql+=(*Cur())+"="+RQuery::SQLValue(va_arg(ap,char*));
		Cur.Next();
		if(!Cur.End())
			sSql+=",";
	}
	va_end(ap);
	auto_ptr<RQuery> insert(DB->Query(sSql));

	// Get the id of the transaction
	if(id)
		return(id);
	sSql="SELECT LAST_INSERT_ID() FROM "+Name;
	auto_ptr<RQuery> select(DB->Query(sSql));
	select->Start();
#ifndef WIN32
	return(atoll(select->operator[](0)));
#else
	return(_atoi64(select->operator[](0)));
#endif
}


//------------------------------------------------------------------------------
RQuery* RTransactionTable::ReadTransaction(size_raw id,bool wait)
{
	RString sSql;
	RQuery* Access=0;

	sSql="SELECT * FROM "+Name;
	if(id)
		sSql+=" WHERE transid="+RString::Number(id);
	try
	{
		do
		{
			// Remove Old access
			if(Access)
			{
				delete Access;
				Access=0;
			}
			Access=DB->Query(sSql);
			Access->Start();
		}
		while((wait)&&(!Access->GetNbRows()));
	}
	catch(...)
	{
		Access=0;
	}
	return(Access);
}


//------------------------------------------------------------------------------
void RTransactionTable::WaitTransaction(size_raw id)
{
	RString sSql;
	RQuery* Access=0;

	sSql="SELECT * FROM "+Name;
	if(id)
		sSql+=" WHERE transid="+RString::Number(id);
	try
	{
		do
		{
			// Remove Old access
			if(Access)
			{
				delete Access;
				Access=0;
			}

			Access=DB->Query(sSql);
			Access->Start();
		}
		while(!Access->GetNbRows());
		delete Access;
	}
	catch(...)
	{
	}
}


//------------------------------------------------------------------------------
void RTransactionTable::RemoveTransaction(size_raw id)
{
	RString sSql;

	sSql="DELETE FROM "+Name;
	if(id)
		sSql+=" WHERE transid="+RString::Number(id);
	try
	{
		auto_ptr<RQuery> del(DB->Query(sSql));
	}
	catch(...)
	{
	}
}


//------------------------------------------------------------------------------
RTransactionTable::~RTransactionTable(void)
{
}

