/*

  RMySQL.h

  MySQL C++ Classes - Header.

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
#ifndef RMySQLH
#define RMySQLH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstring.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for MySQL
#ifdef unix
	#include <mysql/mysql.h>
#else
	#define WIN32
	#include <winsock.h>
	#include <mysql/mysql.h>
#endif


//---------------------------------------------------------------------------
namespace RMySQL{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class RError
{
public:
	RString Error;
	
	RError(const RString &error);	
};



//---------------------------------------------------------------------------
class RDb
{
	MYSQL* connection;
	MYSQL mysql;
public:
	RDb(const RString &host,const RString &user,const RString &pwd,const RString &db) throw(RError);
	int GetProtocolVersion(void) {return(mysql.protocol_version);}
	~RDb(void);
	
	friend class RQuery;
};



//---------------------------------------------------------------------------
class RQuery
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	unsigned int nbrows,nbcols;
public:
	RQuery(RDb *db,const RString &sql) throw(RError);
	unsigned int NbRows(void) { return(nbrows); }
	bool IsMore(void) { return(row); }	
	void Begin(void);
	RQuery& operator++(int);
	char* operator[](unsigned int index);
	~RQuery(void);
};



}  //-------- End of namespace RMySQL ---------------------------------------


//---------------------------------------------------------------------------
#endif
