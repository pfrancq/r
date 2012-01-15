/*

	R Project Library

	RQuickSort.h

	Quicksort algorithm - Header.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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



//-----------------------------------------------------------------------------
#ifndef RQuickSortH
#define RQuickSortH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * This class represent an implementation of the quicksort algorithm to sort a
 * given set of elements (stored as an array of pointers).
 * @tparam C                 The class of the elements to be sorted.
 *
 * To make the necessary comparisons, the template defines the Compare method.
 * This method works like the strcmp function from the standard C/C++ library.
 * The result returned specifies if a given element precedes (>0), is the same
 * (0) or is after (<0) another element. By default, the method uses the
 * Compare method of the C class to make the comparisons:
 * @code
 * int Compare(const C&) const;
 * @endcode
 * This method is virtual and can be re-implemented for specific needs.
 *
 * Here is a simple example:
 * @code
 * #include <rquicksort.h>
 * using namespace R;
 *
 * class MyElement
 * {
 * public:
 *    RString Name;
 *    MyElement(const RString& name) : Name(name) {}
 *    int Compare(const MyElement& name) const {return(Name.Compare(name.Name));}
 * };
 *
 * int main()
 * {
 * 	  RContainer<MyElement,true,false> Tab(5);
 *    Tab.InsertPtr(new MyElement("Str2"));
 *    Tab.InsertPtr(new MyElement("Str5"));
 *    Tab.InsertPtr(new MyElement("Str1"));
 *    RQuickSort<MyElement> Q(Tab);
 *    Q.Order();
 * }
 * @endcode
 * @warning This implementation is slower than the qsort method of the C/C++
 * library. It is therefore only useful, if the comparison method between two
 * elements cannot not be implemented as a static method of the class C.
 * @author Pascal Francq
 * @short Quicksort template.
*/
template<class C> class RQuickSort
{
	/**
	 * The array to sort.
	 */
	C** Tab;

	/**
	 * The number of elements in the array.
	 */
	size_t Max;

public:

	/**
	 * Constructor of the algorithm.
	 * @param tab            Array to sort.
	 * @param max            Number of elements in the array.
	 */
	RQuickSort(C** tab=0,size_t max=0);

	/**
	 * Constructor of the algorithm.
	 * @param cont           Container to sort.
	 */
	RQuickSort(RContainer<C,true,false>& cont);

	/**
	 * Constructor of the algorithm.
	 * @param cont           Container to sort.
	 */
	RQuickSort(RContainer<C,false,false>& cont);

private:

	/**
	 * Compare method that should work like the strcmp function of the C
	 * library. By default, it calls the Compare method of C, but it can be
	 * override.
	 * @param obj1           First object to compare with.
	 * @param obj2           Second object to compare with.
	 * @return a number depending if obj1 must be sorted before obj2 (<0), obj1
	 * and obj2 are equal (==0) or obj1 must be sorted after obj2 (>0).
	 */
	virtual int Compare(C* obj1,C* obj2);

	/**
	 * Manage a partition.
	 * @param left           First element.
	 * @param right          Last element.
	 * @param pivotIndex     Index of the pivot.
	 * @return Index of the last element sorted before the pivot.
	 */
	size_t Partition(size_t left,size_t  right,size_t  pivotIndex);

	/**
	 * Do a quicksort.
	 * @param left           First element.
	 * @param right          Second element.
	 */
	void Quicksort(size_t left, size_t right);

public:

	/**
	 * Order the current array.
	 */
	inline void Order(void)
	{
		if(!Max)
			return;
		Quicksort(0,Max-1);
	}

	/**
	 * Order an array.
	 * @param tab            Array to sort.
	 * @param max            Number of elements in the array.
	 */
	inline void Order(C** tab,size_t max)
	{
		Tab=tab;
		Max=max;
		if(!Max)
			return;
		Quicksort(0,Max-1);
	}

	/**
	 * Destructor.
	 */
	virtual ~RQuickSort(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rquicksort.hh>


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
