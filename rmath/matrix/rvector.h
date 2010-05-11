/*

	R Project Library

	RVector.h

	Class representing vector - Header

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef RVectorH
#define RVectorH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnumcontainer.h>
#include <rnumcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RVector class provides a representation for a vector. It is a
 * specialized class inheriting from RNumContainer.
 * @author Pascal Francq
 * @short RVector
 */
class RVector : public RNumContainer<double,false>
{
public:

	/**
	* Construct a vector.Cols
	* @param max             Maximum number of values.
	*/
	RVector(size_t max);

	/**
	* Copy constructor.
	* @param vector          Vector to copy.
	*/
	RVector(const RVector& vector);

	/**
	 * Print the content of the vector.
	 * @tparam S             Stream class that implements the << operator.
	 * @param stream         Stream.
	 * @param name           Name of the vector.
	 */
	template<class S> void Print(S& stream,const RString& name)
	{
		stream<<name<<"=[";
		for(size_t i=0;i<NbInt;i++)
		{
			if(i)
				stream<<" ";
			stream<<(*this)[i];
		}
		stream<<"]"<<std::endl;
	}

	/**
	 * Destruct the vector.
	 * @return
	 */
	~RVector(void);
};


//------------------------------------------------------------------------------
/**
 * The RMatrixLine represents a vector corresponding to a line of a matrix. It
 * is a specialized class inheriting from RVector. The methods to insert and
 * delete numbers are hidden.
 * @short Vector representing a matrix line.
 * @author Pascal Francq.
 */
class RMatrixLine : private RVector
{
public:

	/**
	* Construct a vector.
	* @param max             Maximum number of values.
	*/
	RMatrixLine(size_t max);

	/**
	* Copy constructor.
	* @param vector          Vector to copy.
	*/
	RMatrixLine(const RMatrixLine& vector);

	/**
	* Get the number of values in the list.
	* @return size_t
	*/
	inline size_t GetNb(void) const {return(RVector::GetNb());}

	/**
	 * Get the columns corresponding to this line.
	 * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate (max included).
	 *                       If SIZE_MAX, iterate until the end of the container.
	 * @return Cursor over the numbers.
	 */
	inline RNumCursor<double> GetCols(size_t min=0,size_t max=SIZE_MAX) const {return(RNumCursor<double>(*this,min,max));}

	/**
	* Return the value at position i. The first value is at position 0.
	* @param i               Index.
	*/
	inline double operator[](size_t i) const {return(RVector::operator[](i));}

	/**
	* Return the value at position i. The first value is at position 0.
	* @param i               Index.
	*/
	inline double& operator[](size_t i) {return(RVector::operator[](i));}

	/**
	 * Destruct the vector.
	 * @return
	 */
	~RMatrixLine(void);

	friend class RMatrix;
	friend class RLowerTriangularMatrix;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
