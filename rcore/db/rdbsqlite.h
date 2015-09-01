/*

	R Project Library

	RDbSQLite.h

	SQLite Database- Header.

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
#ifndef RDbSQLite_H
#define RDbSQLite_H


//------------------------------------------------------------------------------
// include files for SQLite
#include <sqlite3.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rquery.h>
#include <ruri.h>
#include <rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RDbSQLite class provides a representation of connection to a SQLite
* database.
* @see RQuery
* @short SQLite Database.
*/
class RDbSQLite : public RDb
{
	/**
	 * Connection to SQLite database.
	 */
	sqlite3* Connection;

	/**
	 * Result of the last operation.
	 */
	int Code;

public:

	/**
	* Construct a connection to the SQLite database.
	* @param db              URI of the database.
	*/
	RDbSQLite(const RURI& db);

	/**
	 * Get the last auto-increment identifier inserted.
	 */
	virtual size_t GetLastInsertId(void);

	/**
	 * Initialize a query.
	 * @param sql            SQL query.
	 * @param nbcols         Number of columns of the query (assigned by the
	 *                       method).
	 * @return Pointer to a structure allocated by the database.
	 */
	virtual void* InitQuery(const RString& sql,size_raw& nbcols);

	/**
	 * Release a query.
	 * @param data           Data to release (must be cast).
	 */
	virtual void ReleaseQuery(void* data);

	/**
	 * @param data            Database-dependent data.
	 * @return true if all the rows of a query are treated.
	 */
	virtual bool EndQuery(const void* data);

	/**
	 * Start a query.
	 * @param data            Database-dependent data.
	 */
	virtual void StartQuery(void* data);

	/**
	 * Read the next row of the query.
	 * @param data            Database-dependent data.
	 */
	virtual void NextQuery(void* data);

	/**
	* Return a specific field of the current row.
	* @param data            Database-dependent data.
	* @param index           Index of the field in the query.
	*/
	virtual RString GetField(const void* data,size_t index);

	/**
	* Destruct the connection to the database.
	*/
	virtual ~RDbSQLite(void);
};


//------------------------------------------------------------------------------
/**
* The RTransactionTable class provides a representation for a transaction table.
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
	RTransactionTable(RDb* db,RString name,size_t nb,...);

	/**
	* Write a given transaction.
	* @param id             Identifier of the transaction (if null, a new is
	*                       created).
	* @param ...            Values of the parameters of the transaction (transid
	                        is reserved).
	* @return The identifier of the transaction.
	*/
	size_raw WriteTransaction(size_t id,...);

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
