/*

	R Project Library

	RTransactionTable.cpp

	Transaction table allows multiple process to communicate - Implementation.

	Copyright 2000-2015 by the Pascal Francq (pascal@francq.info).
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
#include <rtransactiontable.h>
#include <rquery.h>
#include <rcursor.h>
using namespace R;
using namespace std;



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
RTransactionTable::RTransactionTable(RDb* db,RString name,size_t nb,...)
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
	RQuery Create(DB,sSql);
}


//------------------------------------------------------------------------------
size_raw RTransactionTable::WriteTransaction(size_t id,...)
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
	RQuery Insert(DB,sSql);

	// Get the id of the transaction
	if(id)
		return(id);
	sSql="SELECT LAST_INSERT_ID() FROM "+Name;
	RQuery Select(DB,sSql);
	Select.Start();
#ifndef WIN32
	return(atoll(Select[0]));
#else
	return(_atoi64(Select[0]));
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
		while((wait)&&(Access->End()));
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
		while(Access->End());
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
		RQuery Del(DB,sSql);
	}
	catch(...)
	{
	}
}


//------------------------------------------------------------------------------
RTransactionTable::~RTransactionTable(void)
{
}

