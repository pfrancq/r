/*

	R Project Library

	RNumCursor.h

	Cursor over a container of numbers - Header

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#ifndef RNumCursorH
#define RNumCursorH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnumcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RNumCursor class provides a cursor over a container of number.
* @param I                   Type (int, double, etc.).
*
* @code
* RVector a(3);
* a[0]=1.0;
* a[1]=2.0;
* a[2]=3.0;
* RNumCursor<double> Cur(a);
* for(Cur.Start();!Cur.End();Cur.Next())
* 	cout<<Cur()<<endl;
* }
* @endcode
* @author Pascal Francq.
* @short Container of Numbers Cursor.
*/
template<class I>
	class RNumCursor
{
	/**
	* Number of values in the list.
	*/
	size_t NbInt;

	/**
	* The array representing the values.
	*/
	I* List;

	/**
	* Pointer used to parse the list.
	*/
	I* Parse;

	/**
	* Current position parsed.
	*/
	size_t Pos;

public:

	/**
	* Construct a null cursor.
	*/
	RNumCursor(void);

	/**
	* Copy constructor.
	* @param cur             Cursor to copy.
	*/
	RNumCursor(const RNumCursor& cur);

	/**
	 * Construct a cursor over a given container.
	 * @param cont           Container of numbers.
	 */
	template<bool a> RNumCursor(const RNumContainer<I,a>& cont);

	/**
	 * Set a cursor to a given container.
	 * @param cont           Container of numbers.
	 */
	template<bool a> void Set(const RNumContainer<I,a>& cont);

	/**
	* Get the number of values in the list.
	* @return size_t
	*/
	size_t GetNb(void) const {return(NbInt);}

	/**
	* Start the iterator to go trough the list.
	*/
	void Start(void) {Pos=0; Parse=List;}

	/**
	* Test if the end of the list is reached.
	*/
	bool End(void) const {return(Pos==NbInt);}

	/**
	* Goto the next element of the list.
	*/
	void Next(void) {Pos++; Parse++;}

	/**
	* Return the current element (const version).
	*/
	I operator()(void) const {return(*Parse);}

	/**
	* Return the current element.
	*/
	I& operator()(void) {return(*Parse);}

	/**
	* Get the current position parsed.
	* @return size_t
	*/
	size_t GetPos(void) const {return(Pos);}
};


//------------------------------------------------------------------------------
// Template implementation
#include <rnumcursor.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
