/*

	R Project Library

	RBoolCursor.h

	Cursor over a container of numbers - Header

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RBoolCursorH
#define RBoolCursorH


//------------------------------------------------------------------------------
// include files for R Project
#include <rboolvector.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RBoolCursor class provides a cursor over a vector of boolean values.
*
* @code
* RBoolVector a(3);
* a[0]=1.0;
* a[1]=2.0;
* a[2]=3.0;
* RBoolCursor Cur(a);
* for(Cur.Start();!Cur.End();Cur.Next())
* 	cout<<Cur()<<endl;
* @endcode
* @author Pascal Francq.
* @short Boolean Vector Cursor.
*/
class RBoolCursor
{
	/**
	* Number of values in the vector.
	*/
	size_t NbBool;

	/**
	* The array representing the values.
	*/
	bool* List;

	/**
	* Pointer used to parse the vector.
	*/
	bool* Parse;

	/**
	* Current position parsed.
	*/
	size_t Pos;

	/**
	* The first position in the array handled by the cursor.
	*/
	size_t First;

	/**
	* The last position in the array handled by the cursor.
	*/
	size_t Last;

public:

	/**
	* Construct a null cursor.
	*/
	RBoolCursor(void);

	/**
	* Copy constructor.
	* @param cur             Cursor to copy.
	*/
	RBoolCursor(const RBoolCursor& cur);

	/**
	 * Construct a cursor over a given vector.
	 * @param vector         Vector.
	 * @param min            Minimum position of the elements to iterate.
 	 * @param max            Maximum position of the elements to iterate (included max).
 	 *                       If SZE_MAX, iterate until the end of the container.
	 */
	RBoolCursor(const RBoolVector& vector,size_t min=0,size_t max=SIZE_MAX);

	/**
	 * Set a cursor to a given vector.
	 * @param vector         Vector.
	 * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate (included max).
	 *                       If SZE_MAX, iterate until the end of the container.
	 */
	void Set(const RBoolVector& vector,size_t min=0,size_t max=SIZE_MAX);

	/**
	* Get the number of values in the list.
	* @return size_t
	*/
	size_t GetNb(void) const  {return(Last-First);}

	/**
	* Start the iterator to go trough the vector.
	*/
	void Start(void);

	/**
	 * Start the iterator at the end of the vector.
	 */
	void StartFromEnd(void);

	/**
	* Test if the end of the vector is reached.
	*/
	inline bool End(void) const {return(Pos==Last);}

	/**
	* Test if the begin of the vector is reached.
	*/
	inline bool Begin(void) const {return(Pos==cNoRef);}

	/**
	* Go to the i-th position of the cursor.
	* @param idx             Index of the element to get.
	*/
	void GoTo(size_t idx);

	/**
	* Go to a given number of next elements. If the end is reached, go to the
	* beginning.
	* @param inc             Number of elements to go to.
	*/
	void Next(size_t inc=1);

	/**
	* Go to a given number of previous elements. If the begin is reached, go to the
	* end.
	* @param inc             Number of elements to go to.
	*/
	void Prev(size_t inc=1);

	/**
	* Return the current element (const version).
	*/
	bool operator()(void) const {return(*Parse);}

	/**
	* Return the current element.
	*/
	bool& operator()(void) {return(*Parse);}

	/**
	* Return the actual position in the cursor.
	*/
	inline size_t GetPos(void) const {return(Pos-First);}
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
