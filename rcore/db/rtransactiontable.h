/*

	R Project Library

	RTransactionTable.h

	Transaction table allows multiple process to communicate - Header.

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
#ifndef RTransactionTable_H
#define RTransactionTable_H



//------------------------------------------------------------------------------
// include files for R Project
#include <rdb.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


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
	* Destruct the transaction table.
	*/
	~RTransactionTable(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
