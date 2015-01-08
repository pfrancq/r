/*

	R Project Library

	RDb.h

	Generic Database - Header.

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
#ifndef RDb_H
#define RDb_H



//------------------------------------------------------------------------------
// include files for R Project
#include <ruri.h>
#include <rdate.h>
#include <rexception.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RQuery;


//------------------------------------------------------------------------------
/**
* Type representing an identifier in a database row.
*/
typedef unsigned long long size_raw;


//------------------------------------------------------------------------------
/**
* The RDbExcetion class provides a representation of an error that occurs while
* working with a database.
* @author Pascal Francq
* @short Database Error.
*/
class RDbException : public R::RException
{
public:

	/**
	* Constructor.
	*/
	RDbException(void) : RException() {}

	/**
	* Constructor.
	* @param str                      Message of the error.
	*/
	RDbException(const RString& str) : RException(str) {}
};



//------------------------------------------------------------------------------
/**
* The RDb pure class provides a representation of connection to a database.
* Actually, MySQL and SQLite child classes are supported.
*
* @author Pascal Francq
* @short Generic Database.
*/
class RDb
{
public:

	/**
	 * Type of the databases supported.
	 */
	enum Db
	{
		MySQL,           /** MySQL database. */
		SQLite,            /** SQLite database. */
                                                PostgreSQL    /** PostgreSQL database.*/
	};

protected:

	/**
	 * Type of the database.
	 */
	Db Type;

public:

	/**
	* Construct a connection to the database.
	* @param type            Type of the database.
	*/
	RDb(Db type);

	/**
	 * Get the type of the database.
	 */
	Db GetType(void) const {return(Type);}

	/**
	* Create a table that will be used to simulate transactions.
	* @param name           Name of the transaction.
	* @param nb             Number of parameters.
	* @param ...            Name of the parameters of the transaction (transid
	                        is reserved).
	*/
	virtual void CreateTransactionTable(const RString& name,size_t nb,...);

	/**
	 * Get the last auto-increment identifier inserted.
	 */
	virtual size_t GetLastInsertId(void)=0;

	/**
	 * Initialize a query.
	 * @param sql            SQL query.
	 * @param nbcols         Number of columns of the query (assigned by the
	 *                       method).
	 * @return Pointer to a structure allocated by the database.
	 */
	virtual void* InitQuery(const RString& sql,size_raw& nbcols)=0;

	/**
	 * Release a query.
	 * @param data            Database-dependent data.
	 */
	virtual void ReleaseQuery(void* data)=0;

	/**
	 * @param data            Database-dependent data.
	 * @return true if all the rows of a query are treated.
	 */
	virtual bool EndQuery(const void* data)=0;

	/**
	 * Start a query.
	 * @param data            Database-dependent data.
	 */
	virtual void StartQuery(void* data)=0;

	/**
	 * Read the next row of the query.
	 * @param data            Database-dependent data.
	 */
	virtual void NextQuery(void* data)=0;

	/**
	* Return a specific field of the current row.
	* @param data            Database-dependent data.
	* @param index           Index of the field in the query.
	*/
	virtual RString GetField(const void* data,size_t index)=0;

	/**
	 * Run file containing SQL commands. The file is supposed to be encoded in
	 * utf-8. Lines starting with C comment with "!", "--" or "#" are considered as comments
	 * and not send to the database.
	 * @param file           File to execute.
	 * @param coutit         If yes, each SQL query is printed on screen.
	 */
	void RunSQLFile(const RURI& file,bool coutit=false);

	/**
	* Destruct the connection to the database.
	*/
	virtual ~RDb(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
