/*

	R Project Library

	RNumCursor.h

	Cursor over a container of numbers - Header

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
* @tparam I                  Type (int, double, etc.).
*
* @code
* RNumContainer<double,false> a(3);
* a[0]=1.0;
* a[1]=2.0;
* a[2]=3.0;
* RNumCursor<double> Cur(a);
* for(Cur.Start();!Cur.End();Cur.Next())
* 	cout<<Cur()<<endl;
* @endcode
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
	RNumCursor(void);

	/**
	* Copy constructor.
	* @param cur             Cursor to copy.
	*/
	RNumCursor(const RNumCursor& cur);

	/**
	 * Construct a cursor over a given container.
	 * @tparam o             Determine if the container is ordered.
	 * @param cont           Container of numbers.
	 * @param min            Minimum position of the elements to iterate.
 	 * @param max            Maximum position of the elements to iterate (included max).
 	 *                       If SZE_MAX, iterate until the end of the container.
	 */
	template<bool o> RNumCursor(const RNumContainer<I,o>& cont,size_t min=0,size_t max=SIZE_MAX);

	/**
	 * Set a cursor to a given container.
	 * @tparam o             Determine if the container is ordered.
	 * @param cont           Container of numbers.
	 * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate (included max).
	 *                       If SZE_MAX, iterate until the end of the container.
	 */
	template<bool o> void Set(const RNumContainer<I,o>& cont,size_t min=0,size_t max=SIZE_MAX);

	/**
	* Get the number of values in the list.
	* @return size_t
	*/
	size_t GetNb(void) const  {return(Last-First);}

	/**
	* Start the iterator to go trough the list.
	*/
	void Start(void);

	/**
	 * Start the iterator at the end of the container.
	 */
	void StartFromEnd(void);

	/**
	* Test if the end of the list is reached.
	*/
	inline bool End(void) const {return(Pos==Last);}

	/**
	* Test if the begin of the list is reached.
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
	I operator()(void) const {return(*Parse);}

	/**
	* Return the current element.
	*/
	I& operator()(void) {return(*Parse);}

	/**
	* Return the actual position in the cursor.
	*/
	inline size_t GetPos(void) const {return(Pos-First);}
};


//------------------------------------------------------------------------------
// Template implementation
#include <rnumcursor.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
namespace std{
//------------------------------------------------------------------------------


#ifndef __APPLE__
extern "C++"
{
#endif

//------------------------------------------------------------------------------
/**
 * Print the numbers the containers.
 * @param os                Output stream.
 * @param cont              Container to print.
 * @return the stream.
 */
template<class I,bool bOrder>
	extern std::ostream& operator<<(std::ostream& os,const R::RNumContainer<I,bOrder>& cont)
{
	R::RNumCursor<I> Cur(cont);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		os<<Cur();
		if(Cur.GetPos()<Cur.GetNb()-1)
			os<<";";
	}
   return(os);
}


#ifndef __APPLE__
}
#endif


}  //-------- End of namespace std ---------------------------------------------

//------------------------------------------------------------------------------
#endif
