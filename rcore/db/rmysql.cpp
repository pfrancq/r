/*

	RMySQL.cpp

  MySQL C++ Classes - Implementation.

  (C) 2000 by P. Francq.

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



//---------------------------------------------------------------------------
#include <string.h>
#include <rmysql/rmysql.h>
using namespace RMySQL;
using namespace RStd;



//---------------------------------------------------------------------------
//
// RMySQLError
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RMySQL::RMySQLError::RMySQLError(const char* error)
	: Error(error)
{
}



//---------------------------------------------------------------------------
//
// RDb
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RMySQL::RDb::RDb(const char* host,const char* user,const char* pwd,const char* db) throw(RMySQLError)
{
	MYSQL* ret;

	ret=mysql_init(&mysql);
	if((!ret)||(mysql_errno(&mysql)))
		throw RMySQLError(mysql_error(&mysql));
	connection=mysql_real_connect(&mysql,host,user,pwd,db,0,"",0);
	if((!connection)||(mysql_errno(&mysql)))
		throw RMySQLError(mysql_error(&mysql));
}


//---------------------------------------------------------------------------
RMySQL::RDb::~RDb(void)
{
	if(connection) mysql_close(connection);
}



//---------------------------------------------------------------------------
//
// RQuery
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RMySQL::RQuery::RQuery(RDb* db,const char* sql) throw(RMySQLError)
{
	RString Up(sql);
	bool bSelect;
	bool bInsert;
	bool bLastInsertId;

	if((!db)||!(db->connection))
		throw RMySQLError("Database not initialize");
	if(mysql_real_query(db->connection,sql,strlen(sql)))
		throw RMySQLError(mysql_error(&db->mysql));
	Up.StrUpr();
	bSelect=strstr(Up(),"SELECT");
	if(bSelect)
	{
		bLastInsertId=strstr(Up(),"LAST_INSERT_ID()");
		bInsert=strstr(Up(),"INSERT");
		if(!bInsert||(bInsert&&bLastInsertId))
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
	else
	{
		nbcols=nbrows=0;
		row=0;
		result=0;
	}
}


//---------------------------------------------------------------------------	
void RMySQL::RQuery::Begin(void)
{
	row=mysql_fetch_row(result);
}


//---------------------------------------------------------------------------	
void RMySQL::RQuery::Start(void)
{
	row=mysql_fetch_row(result);
}


//---------------------------------------------------------------------------
RQuery& RMySQL::RQuery::operator++(int)
{
	row=mysql_fetch_row(result);
	return(*this);
}


//---------------------------------------------------------------------------
void RMySQL::RQuery::Next(void)
{
	row=mysql_fetch_row(result);
}


//---------------------------------------------------------------------------	
const char* RMySQL::RQuery::operator[](unsigned int index) const throw(RMySQLError)
{
	if(index>=nbcols)
		throw RMySQLError("Index out of range");
	if(!row)
		throw RMySQLError("Treated set");
	return(row[index]);
}


//---------------------------------------------------------------------------
RMySQL::RQuery::~RQuery(void)
{
	if(result) mysql_free_result(result);
}
