/*

        R Project Library

        RDbPostgreSQL.h

       PostgreSQL Database - Header.

        Copyright 2000-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef RDbPostgreSQL_Hs
#define RDbPostgreSQL_H


//------------------------------------------------------------------------------
// include files for PostgreSQL
#include <postgresql/libpq-fe.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextencoding.h>
#include <rquery.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RDbPostgreSQL class provides a representation of connection to a
 * PostgreSQL database.
* @see RQuery
* @short PostgreSQL Database.
*/
class RDbPostgreSQL : public RDb
{
protected:

	/**
	 * Database.
	 */
	PGconn* Db;

	/**
	 * Coding used to read/write to PostgreSQL.
	 */
	RTextEncoding* Coding;

public:

	/**
	 * Construct a connection to the PostgreSQL database.
	 * @param db             Name of the database.
	 * @param host           Host.
	 * @param user           User.
	 * @param pwd            Password.
	 * @param coding         Name of the coding used.
	 */
	RDbPostgreSQL(const RString& db,const RString& host,const RString& user,const RString& pwd,const RCString& coding="utf-8");

	/**
	 * Create an empty PostgreSQL database.
	 * @param db             Name of the database.
	 * @param host           Host.
	 * @param user           User.
	 * @param pwd            Password.
	 */
	static void Create(const RString& db,const RString& host,const RString& user,const RString& pwd);

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
	 * @param data            Data to release (must be cast).
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
	virtual ~RDbPostgreSQL(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
