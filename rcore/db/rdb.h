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
#ifndef RDb_H
#define RDb_H



//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rcontainer.h>
#include <rcstring.h>
#include <rstring.h>
#include <rdate.h>



//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RTextEncoding;
class RQuery;


//------------------------------------------------------------------------------
/**
* Type representing an identifier in a database row.
*/
typedef unsigned long long size_raw;


//------------------------------------------------------------------------------
/**
* The RMySQL class provides a representation of an error that occurs while
* working with a MySQL database.
* @author Pascal Francq
* @short MySQL Error.
*/
class RDbException : public R::RException
{
public:

	/**
	* Constructor.
	*/
	RDbException(void) throw() : RException() {}

	/**
	* Constructor.
	* @param str                      Message of the error.
	*/
	RDbException(const char* str) throw() : RException(str) {}
};



//------------------------------------------------------------------------------
/**
* The RDb class provides a representation of connection to a database.
* Actually, MySQL and SQLite are supported.
*
* Here is a example:
*
* @code
* #include <iostream>
* #include <rdb.h>
* using namespace R;
* using namespace std;
*
*
* //----------------------------------------------------------------------------
* void Load(void)
* {
* 	auto_ptr<RDb> db(RDb::Get(RDb::MySQL,"host","me","mypassword","thedb"));
* 	auto_ptr<RQuery> q(db->Query("SELECT * FROM tbl");
*
* 	for(q->Start();!q->End();q->Next())
* 		cout<<"Col 1: "<<(*q)[0]<<"   -    Col 2:"<<(*q)[1]<<endl;
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
* 	catch(RDbException &e)
* 	{
* 		cout<<"Error: "<<e.GetError()<<endl;
* 	}
* }
* @endcode
*
* @author Pascal Francq
* @short Database.
*/
class RDb
{
public:

	enum Db
	{
		MySQL,
		SQLite
	};

	/**
	 * Type of the database.
	 */
	Db Type;

protected:

	/**
	* Construct a connection to the database.
	* @param type            Type of the database.
	*/
	RDb(Db type);

public:

	/**
	 * Get the type of the database.
	 */
	Db GetType(void) const {return(Type);}

	/**
	* Create a new MySQL database.
	* @param host           Host of the database server.
	* @param user           User to connect with.
	* @param pwd            Password of the user.
	* @param name           The name of the new database
	*/
	static void CreateMySQLDatabase(const RString& host,const RString& user,const RString& pwd,const RString& name);

	/**
	 * Get a connection to a database.
	 * @param type           Type of the database.
	 * @param db             Name of the Db.
	 * @param host           Host of the database.
	 * @param user           User used for the connection.
	 * @param pwd            Password used for the connection.
	 * @param coding         Coding of the database.
	 * @return Smart pointer on RDb.
	 */
	static RDb* Get(Db type,const RString& db,const RString& host=RString::Null,const RString& user=RString::Null,const RString& pwd=RString::Null,const RString& coding="latin1");

	/**
	* Create a table that will be used to simulate transactions.
	* @param name           Name of the transaction.
	* @param nb             Number of paramters.
	* @param ...            Name of the parameters of the transaction (transid
	                        is reserved).
	*/
	virtual void CreateTransactionTable(const RString& name,unsigned int nb,...);

	/**
	 * Create a query.
	 * @param sql
	 * @return Smart pointer on RQuery.
	 */
	virtual RQuery* Query(const RString& sql)=0;

	/**
	* Destruct the connection to the database.
	*/
	virtual ~RDb(void);
};


//------------------------------------------------------------------------------
/**
* The RQuery class provides a representation of a query.
*
* Here is a example:
*
* @code
* #include <iostream>
* #include <rdb.h>
* using namespace R;
* using namespace std;
*
*
* //----------------------------------------------------------------------------
* void Load(void)
* {
* 	auto_ptr<RDb> db(RDb::Get(RDb::MySQL,"host","me","mypassword","thedb"));
* 	auto_ptr<RQuery> q(db->Query("SELECT * FROM tbl");
*
* 	for(q->Start();!q->End();q->Next())
* 		cout<<"Col 1: "<<(*q)[0]<<"   -    Col 2:"<<(*q)[1]<<endl;
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
* 	catch(RDbException &e)
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
protected:

	/**
	* SQL query.
	*/
	RString SQL;

	/**
	* Total number of rows returned by the query.
	*/
	size_raw NbRows;

	/**
	* Number of columns of the query.
	*/
	size_raw NbCols;

	/**
	* Construct a query.
	* @param sql            String containing a SQL query.
	*/
	RQuery(const RString& sql);

public:

	/**
	* Get the total number of rows of the query.
	* @returns Number of rows.
	*/
	size_raw GetNbRows(void) {return(NbRows);}

	/**
	* Get the total number of columns of the query.
	* @returns Number of columns.
	*/
	size_raw GetNbCols(void) {return(NbCols);}

	/**
	* Look if all rows of the query were treated.
	* @return True if all rows were treated.
	*/
	virtual bool End(void) const=0;

	/**
	* Put the query at the first row.
	*/
	virtual void Start(void)=0;

	/**
	* Increment the current row.
	*/
	virtual void Next(void)=0;

	/**
	* Return a specific field of the current row.
	* @param index          Index of the field in the query.
	*/
	virtual RString operator[](unsigned int index) const=0;

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
* The RTransactionTable class provides a representation for a transaction table.
* @author Pascal Francq
* @short Transaction Table.
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
	* @param nb             Number of parameters.
	* @param ...            Name of the parameters of the transaction (transid
	                        is reserved).
	*/
	RTransactionTable(RDb* db,RString name,unsigned int nb,...);

	/**
	* Write a given transaction.
	* @param id             Identifier of the transaction (if null, a new is
	*                       created).
	* @param ...            Values of the parameters of the transaction (transid
	                        is reserved).
	* @return The identifier of the transaction.
	*/
	size_raw WriteTransaction(unsigned int id,...);

	/**
	* Get a transaction.
	* @param id             Identifier of the transaction. If null, all the
	*                       existing transactions are loaded.
	* @param wait           If true, the method waits until at least one
	*                       transaction arrived.
	* @return Pointer to a query containing the loaded transactions. This query
	*         should be destroyed by the caller.
	*/
	RQuery* ReadTransaction(size_raw id,bool wait);

	/**
	* Wait that a given transaction arrived.
	* @param id             Identifier of the transaction.
	*/
	void WaitTransaction(size_raw id);

	/**
	* Remove a given transaction.
	* @param id             Identifier of the transaction. If null, all the
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
