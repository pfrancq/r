/*

	R Project Library

	RMySQL.h

	MySQL C++ Classes - Header.

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
#ifndef RMySQL_H
#define RMySQL_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rcontainer.h>
#include <rcstring.h>
#include <rstring.h>
#include <rdate.h>


//------------------------------------------------------------------------------
// include files for MySQL
#ifdef WIN32
	#include <winsock.h>
	#include <memory> //for std::auto_ptr
#endif
#include <mysql/mysql.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RTextEncoding;


//------------------------------------------------------------------------------
/**
* Type representing an identifier in a database row/
*/
typedef unsigned long long size_raw;


//------------------------------------------------------------------------------
/**
* The RMySQL class provides a representation of an error that occurs while
* working with a MySQL database.
* @author Pascal Francq
* @short MySQL Error.
*/
class RMySQLError : public R::RException
{
public:

	/**
	* Constructor.
	*/
	RMySQLError(void) throw()
		: R::RException() {}

	/**
	* Constructor.
	* @param str                      Message of the error.
	*/
	RMySQLError(const char* str) throw()
		: R::RException(str) {}
};



//------------------------------------------------------------------------------
/**
* The RDb class provides a representation of a MySQL database.
*
* Here is a example:
*
* @code
* #include <iostream>
* #include <rmysql.h>
* using namespace R;
*
*
* //----------------------------------------------------------------------------
* void Load(void)
* {
* 	RDb db("host","me","mypassword","thedb");
* 	RQuery q(&db, "SELECT * FROM tbl");
*
* 	for(q.Start();!q.End();q.Next())
* 		cout<<"Col 1: "<<q[0]<<"   -    Col 2:"<<q[1]<<endl;
* }
*
*
* //----------------------------------------------------------------------------
* int main(void)
* {
* 	try
* 	{
* 		Load();
* 	}
* 	catch(RMySQLError &e)
* 	{
* 		cout<<"Error: "<<e.GetError()<<endl;
* 	}
* }
* @endcode
*
* @author Pascal Francq
* @short MySQL Database.
*/
class RDb
{
	/**
	* Internal structure.
	*/
	MYSQL* connection;

	/**
	* Internal structure.
	*/
	MYSQL mysql;

	/**
	* Coding used to read/write to MySQL.
	*/
	RTextEncoding* Coding;

public:

	/**
	* Connstruct a database.
	* @param host           Host of the database server.
	* @param user           User to connect with.
	* @param pwd            Password of the uzer.
	* @param db             Name of the database.
	* @param coding         Name of the coding.
	*/
	RDb(RString host,RString user,RString pwd,RString db,RString coding="latin1");

	/**
	* Create a new database
	* @param host           Host of the database server.
	* @param user           User to connect with.
	* @param pwd            Password of the user.
	* @param name           The name of the new database
	*/
	static void CreateDatabase(RString host,RString user,RString pwd,RString name);

	/**
	* Create a table that will be used to simulate transactions.
	* @param name           Name of the transaction.
	* @param nb             Number of paramters.
	* @param ...            Name of the parameters of the transaction (transid
	                        is reserved).
	*/
	void CreateTransactionTable(RString name,unsigned int nb,...);

	/**
	* Get the protocol version used.
	* @return a identifier.
	*/
	int GetProtocolVersion(void)
		{return(mysql.protocol_version);}

	/**
	* Destruct the database.
	*/
	virtual ~RDb(void);

	// Friend class.
	friend class RQuery;
};


//------------------------------------------------------------------------------
/**
* The RQuery class provides a representation of a query.
*
* Here is a example:
*
* @code
* #include <iostream>
* #include <rmysql.h>
* using namespace R;
*
*
* //----------------------------------------------------------------------------
* void Load(void)
* {
* 	RDb db("host","me","mypassword","thedb");
* 	RQuery q(&db, "SELECT * FROM tbl");
*
* 	for(q.Start();!q.End();q.Next())
* 		cout<<"Col 1: "<<q[0]<<"   -    Col 2:"<<q[1]<<endl;
* }
*
*
* //----------------------------------------------------------------------------
* int main(void)
* {
* 	try
* 	{
* 		Load();
* 	}
* 	catch(RMySQLError &e)
* 	{
* 		cout<<"Error: "<<e.GetError()<<endl;
* 	}
* }
* @endcode
* @author Pascal Francq
* @short MySQL Query.
*/
class RQuery
{
	/**
	* Internal structure.
	*/
	MYSQL_RES *result;

	/**
	* Actual row.
	*/
	MYSQL_ROW row;

	/**
	* Total number of rows returned by the query.
	*/
	size_raw nbrows;

	/**
	* Number of columns of the query.
	*/
	size_raw nbcols;

	/**
	* SQL query.
	*/
	RString SQL;

	/**
	* Database.
	*/
	RDb* DB;

public:

	/**
	* Construct a query.
	* @param db             Pointer to the corresponding database.
	* @param sql            String containing a SQL query.
	*/
	RQuery(RDb* db,RString sql);

	/**
	* Construct a query.
	* @param db             Reference to the corresponding database.
	* @param sql            String containing a SQL query.
	*/
	RQuery(RDb& db,RString sql);

	/**
	* Construct a query.
	* @param db             Smart pointer to the corresponding database.
	* @param sql            String containing a SQL query.
	*/
	RQuery(std::auto_ptr<R::RDb>& db,RString sql);

protected:

	/**
	* Does the work of initializing the query.
	*/
	void Init(void);

public:

	/**
	* Get the total number of rows of the query.
	* @returns Number of row.
	*/
	size_raw GetNb(void)
		{ return(nbrows); }

	/**
	* Look if all rows of the query were treated.
	* @return True if all rows were treated.
	*/
	bool End(void)
		{ return(!row); }

	/**
	* Put the query at the first row.
	*/
	void Start(void);

	/**
	* Increment the current row.
	*/
	void Next(void);

	/**
	* Return a specific field of the current row.
	* @param index          Index of the field in the query.
	*/
	RString operator[](unsigned int index) const;

	/**
	* Transform a string to be used in a SQL (add quotes before and after,
	* double quotes inside the string).
	* @param val            String containing the value.
	* @return a RString.
	*/
	static RString SQLValue(const RString val);

	/**
	* Transform a date into a SQL date (with quotes).
	* @param d              Date.
	* @return a RString.
	*/
	static RString SQLValue(const RDate& d);

	/**
	* Destruct the query.
	*/
	virtual ~RQuery(void);
};


//------------------------------------------------------------------------------
/**
* The RTransactionTable class provides a representation for a transaction table
* based on MySQL.
* @author Pascal Francq
* @short MySQL Transaction Table.
*/
class RTransactionTable
{
	/**
	* Name of the table.
	*/
	RString Name;

	/**
	* Parameters involved in a transaction.
	*/
	RContainer<RString,true,false> Params;

	/**
	* Database.
	*/
	RDb* DB;

public:

	/**
	* Constructor of a transaction table.
	* @param db             Database.
	* @param name           Name of the transaction table.
	*/
	RTransactionTable(RDb* db,RString name);

	/**
	* Constructor of a transaction table.
	* @param db             Database.
	* @param name           Name of the transaction table.
	* @param nb             Number of paramters.
	* @param ...            Name of the parameters of the transaction (transid
	                        is reserved).
	*/
	RTransactionTable(RDb* db,RString name,unsigned int nb,...);

	/**
	* Write a given transaction.
	* @param id             Identificator of the transaction (if null, a new is
	*                       created).
	* @param ...            Values of the parameters of the transaction (transid
	                        is reserved).
	* @return The identificator of the transaction.
	*/
	size_raw WriteTransaction(unsigned int id,...);

	/**
	* Get a transaction.
	* @param id             Identificator of the transaction. If null, all the
	*                       existing transactions are loaded.
	* @param wait           If true, the method waits until at least one
	*                       transaction arrived.
	* @return Pointer to a query containing the loaded transactions. This query
	*         should be destroyed by the caller.
	*/
	RQuery* ReadTransaction(size_raw id,bool wait);

	/**
	* Wait that a given transaction arrived.
	* @param id             Identificator of the transaction.
	*/
	void WaitTransaction(size_raw id);

	/**
	* Remove a given transaction.
	* @param id             Identificator of the transaction. If null, all the
	*                       existing transactions are removed.
	*/
	void RemoveTransaction(size_raw id);

	/**
	* Destructor.
	*/
	~RTransactionTable(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
