/*

	R Project Library

	RQuery.h

	Generic Query - Header.

	Copyright 2000-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef RQuery_H
#define RQuery_H



//------------------------------------------------------------------------------
// include files for R Project
#include <rdb.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RQuery class provides a representation of a query for a given database.
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
* void LoadMySQL(void)
* {
* 	RDbMySQL db("host","myuser","mypassword","thedb"));
* 	RQuery q(db,"SELECT * FROM tbl");
* 	for(q.Start();!q.End();q.Next())
* 		cout<<"Col 1: "<<q[0]<<"   -    Col 2:"<<q[1]<<endl;
* }
*
*
* //----------------------------------------------------------------------------
* void LoadSQlite(void)
* {
* 	RDbSQLite db("/path/to/db.sqlite");
* 	RQuery q(db,"SELECT * FROM tbl");
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
* 		LoadMySQL();
* 		LoadMySQLite();
* 	}
* 	catch(RDbException &e)
* 	{
* 		cout<<"Error: "<<e.GetError()<<endl;
* 	}
* }
* @endcode
* @author Pascal Francq
* @short Query.
*/
class RQuery
{
protected:
	/**
	 * Database on which the query is done.
	 */
	RDb* Db;

	/**
	* SQL query.
	*/
	RString SQL;

	/**
	* Number of columns of the query.
	*/
	size_raw NbCols;

	/**
	 * Data depending of the database. This data is allocated by the
	 * corresponding RDb::InitQuery.
	 */
	void* Data;

public:

	/**
	* Construct a query.
	* @param db              Database on which the query is done.
	* @param sql             String containing a SQL query.
	*/
	RQuery(RDb* db,const RString& sql);

	/**
	* @returns Get the total number of columns of the query.
	*/
	size_raw GetNbCols(void) {return(NbCols);}

	/**
	* Look if all rows of the query were treated.
	* @return True if all rows were treated.
	*/
	inline bool End(void) const {return(Db->EndQuery(Data));}

	/**
	* Put the query at the first row.
	*/
	inline void Start(void) {Db->StartQuery(Data);}

	/**
	* Increment the current row.
	*/
	inline void Next(void) {Db->NextQuery(Data);}

	/**
	* Return a specific field of the current row.
	* @param index          Index of the field in the query.
	*/
	inline RString operator[](size_t index) const
	{
		if(index>=NbCols)
			throw RDbException("RQuery::operator[] const : column "+RString::Number(index)+" outside range [0,"+RString::Number(NbCols-1)+"]");
		return(Db->GetField(Data,index));
	}

	/**
	* Transform a string to be used in a SQL (add quotes before and after,
	* double quotes inside the string).
	* @param val            String containing the value.
	* @return a SQL value or NULL if the string is empty.
	*/
	static RString SQLValue(const RString val);

	/**
	* Transform an integer into a SQL date (with quotes).
	* @param val             Value.
	* @return a RString.
	*/
	static RString SQLValue(int val);

	/**
	* Transform an unsigned integer into a SQL date (with quotes).
	* @param val             Value.
	* @return a RString.
	*/
	static RString SQLValue(unsigned int val);

	/**
	* Transform an integer into a SQL date (with quotes).
	* @param val             Value.
	* @return a RString.
	*/
	static RString SQLValue(long val);

	/**
	* Transform an unsigned integer into a SQL date (with quotes).
	* @param val             Value.
	* @return a RString.
	*/
	static RString SQLValue(unsigned long val);

	/**
	* Transform a double into a SQL date (with quotes).
	* @param val             Value.
	* @return a RString.
	*/
	static RString SQLValue(double val);

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


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
