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
#include "rmysql.h"
using namespace RMySQL;



//---------------------------------------------------------------------------
//
// RError
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RError::RError(const RString &error)
	: Error(error)
{
}



//---------------------------------------------------------------------------
//
// RDb
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RDb::RDb(const RString &host,const RString &user,const RString &pwd,const RString &db) throw(RError)
{
	mysql_init(&mysql);
	connection=mysql_real_connect(&mysql,host(),user(),pwd(),db(),0,0,0);
	if(!connection) throw(new RError(mysql_error(&mysql)));
}


//---------------------------------------------------------------------------
RDb::~RDb(void)
{
	if(connection) mysql_close(connection);
}



//---------------------------------------------------------------------------
//
// RQuery
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RQuery::RQuery(RDb *db,const RString &sql) throw(RError)
{
	RString Up(sql);
	
	if(!db||!db->connection) throw(new RError("Database not initialize"));
	if(mysql_real_query(db->connection,sql(),strlen(sql())))
		throw(new RError(mysql_error(&db->mysql)));
	Up.StrUpr();	
	if(strstr(Up(),"SELECT")&&!strstr(Up(),"INSERT"))
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


//---------------------------------------------------------------------------	
void RQuery::Begin(void)
{
	row=mysql_fetch_row(result);
}


//---------------------------------------------------------------------------
RQuery& RQuery::operator++(int)
{
	row=mysql_fetch_row(result);
	return(*this);
}


//---------------------------------------------------------------------------	
char* RQuery::operator[](unsigned int index)
{
	if(index>=nbcols)
		throw(RError("Index out of range"));
	if(!row)
		throw(RError("Treated set"));
	return(row[index]);
}


//---------------------------------------------------------------------------
RQuery::~RQuery(void)
{
	if(result) mysql_free_result(result);
}
