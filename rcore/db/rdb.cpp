/*

	R Project Library

	RDb.cpp

	Generic Database - Implementation.

	Copyright 2000-2010 by Pascal Francq (pascal@francq.info).
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
#include <rtextfile.h>
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


//-----------------------------------------------------------------------------
void RDb::RunSQLFile(const RURI& file)
{
	RString sql("");
	RString line("");
	bool endFound=false;

 	RTextFile File(file,"utf-8");
	File.Open(RIO::Read);

	while(!File.End())
	{
		line=File.GetLine();
		if(line.IsEmpty() || line.FindStr("/*!")>=0 || line.FindStr("--")>=0 || line.Find('#')>=0)
			continue;

		endFound=false;
		while(!File.End() && !endFound)
		{
			if(line.IsEmpty() || line.FindStr("--")>=0 || line.FindStr("--")>=0 || line.Find('#')>=0)
			{
				sql="";
				endFound=true;
				continue;
			}
			sql+=line;
			if(line.Find(';')>=0)
				endFound=true;
			else
				line=File.GetLine();
		}
		if(!sql.IsEmpty())
		{
			RQuery Sendquery(this,sql);
		}

		sql="";
	}
}


//------------------------------------------------------------------------------
RDb::~RDb(void)
{
}
