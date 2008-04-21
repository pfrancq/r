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
// include files for R Library
//#include <rstring.h>
#include <rcursor.h>
#include <rmysql.h>
#include <rstring.h>
#include <rtextencoding.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RDb
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDb::RDb(RString host,RString user,RString pwd,RString db,RString coding)
	: Coding(0)
{
	MYSQL* ret;

	try
	{
		Coding=RTextEncoding::GetTextEncoding(coding);
	}
	catch(EncodingNotSupported e)
	{
		throw RMySQLError(RString(e.GetMsg())+" for database "+db);
	}
	ret=mysql_init(&mysql);
	if((!ret)||(mysql_errno(&mysql)))
		throw RMySQLError(mysql_error(&mysql));
	connection=mysql_real_connect(&mysql,host,user,pwd,db,0,"",0);
	if((!connection)||(mysql_errno(&mysql)))
		throw RMySQLError(mysql_error(&mysql));
}


//------------------------------------------------------------------------------
void RDb::CreateDatabase(RString host,RString user,RString pwd,RString name)
{
	MYSQL* ret;
	MYSQL ms;

	ret=mysql_init(&ms);
	if((!ret)||(mysql_errno(&ms)))
		throw RMySQLError(mysql_error(&ms));

	ret=mysql_real_connect(&ms,host,user,pwd,0,0,0,0);
	if((!ret)||(mysql_errno(&ms)))
		throw RMySQLError(mysql_error(&ms));

	RString sql="CREATE DATABASE IF NOT EXISTS ";
	sql+=name;
	if(mysql_query(&ms,sql.Latin1()))
		throw RMySQLError(mysql_error(&ms));
}


//------------------------------------------------------------------------------
void RDb::CreateTransactionTable(RString name,unsigned int nb,...)
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
	RQuery create(this,sSql);
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
RQuery::RQuery(RDb* db,const RString sql)
	: SQL(sql), DB(db)
{
	if(!DB)
		throw RMySQLError("Database not initialize");
	Init();
}


//------------------------------------------------------------------------------
RQuery::RQuery(RDb& db,const RString sql)
	: SQL(sql), DB(&db)
{
	Init();
}


//------------------------------------------------------------------------------
RQuery::RQuery(std::auto_ptr<R::RDb>& db,RString sql)
	: SQL(sql), DB(db.get())
{
	Init();
}


//------------------------------------------------------------------------------
void RQuery::Init(void)
{
	const RChar* ptr;
	unsigned int size,pos;
	RCString SQL_utf8;
	RString cmd;

 	if(!DB->connection)
		throw RMySQLError("Database not initialize");
	if(SQL.IsEmpty())
		throw RMySQLError("Empty SQL");
	try
	{
		SQL_utf8=DB->Coding->FromUnicode(SQL);
	}
	catch(...)
	{
		throw RMySQLError("Error in encoded data: "+SQL);
	}

	if(mysql_real_query(DB->connection,SQL_utf8,SQL_utf8.GetLen()))
		throw RMySQLError("Error in query "+SQL+": "+mysql_error(&DB->mysql));

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
		result=mysql_store_result(DB->connection);
		if(!result)
			throw RMySQLError(mysql_error(&DB->mysql));
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
void RQuery::Start(void)
{
	row=mysql_fetch_row(result);
}


//------------------------------------------------------------------------------
void RQuery::Next(void)
{
	row=mysql_fetch_row(result);
}


//------------------------------------------------------------------------------
RString RQuery::operator[](unsigned int index) const
{
	const char* val;
	RString res;

	if(index>=nbcols)
	{
		char tmp[80];
		sprintf(tmp,"RQuery::operator[] const : column %zu outside range [0,%zu]",index,nbcols-1);
		throw RMySQLError(tmp);
	}
	if(!row)
		throw RMySQLError("Treated set");
	val=row[index];
	if(val)
	{
		res=DB->Coding->ToUnicode(val,strlen(val));
	}
	return(res);
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
	if(result) mysql_free_result(result);
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
	RQuery params(DB,sSql);
	// Skip 'transid
	params.Start();
	for(params.Next();!params.End();params.Next())
		Params.InsertPtr(new RString(params[0]));
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
	RQuery create(DB,sSql);
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
	RQuery insert(DB,sSql);

	// Get the id of the transaction
	if(id)
		return(id);
	sSql="SELECT LAST_INSERT_ID() FROM "+Name;
	RQuery select(DB,sSql);
	select.Start();
#ifndef WIN32
	return(atoll(select[0]));
#else
	return(_atoi64(select[0]));
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
			Access=new RQuery(DB,sSql);
			Access->Start();
		}
		while((wait)&&(!Access->GetNb()));
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

			Access=new RQuery(DB,sSql);
			Access->Start();
		}
		while(!Access->GetNb());
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
		RQuery del(DB,sSql);
	}
	catch(...)
	{
	}
}


//------------------------------------------------------------------------------
RTransactionTable::~RTransactionTable(void)
{
}
