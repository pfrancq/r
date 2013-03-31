/*

	R Project Library

	RVector.h

	Class representing vector - Header

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
 * specialized class inheriting from RNumContainer since a vector is supposed
 * to have a fixed size.
 * @author Pascal Francq
 * @short RVector
 */
class RVector : private RNumContainer<double,false>
{
public:

	using RNumContainer<double,false>::Init;

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
	 * Compare method used by RContainer.
    * @param vector          Vector to compare with.
    * @return always -1.
    */
	int Compare(const RVector& vector) const;

	/**
	 * Initialize all the elements of the vector to a given value.
	 * @param val             Value to assign.
    */
	virtual void Init(double val=0.0);

	/**
	* Test if two vectors have exactly the same values.
	* @param vector           Vector used for the comparison.
	* @return True if the vectors are the same.
	*/
	inline bool IsSame(const RVector& vector) const {return(RNumContainer<double,false>::IsSame(vector));}

	/**
	* Verify if a value is in the vector.
	* @param value            Value to test.
	* @return True if the value is in the vector, false else.
	*/
	inline bool IsIn(double value) const {return(RNumContainer<double,false>::IsIn(value));}

	/**
	* Reorder the vector in ascending order.
	*/
	inline void ReOrder(void) {RNumContainer<double,false>::ReOrder();}

	/**
	* Randomize the vector.
	* @param rand             Random number generator to use.
	* @param nb               Number of first element to randomize. If null,
	*                         all values are randomized.
	*/
	inline void Randomize(RRandom* rand,size_t nb=0) {RNumContainer<double,false>::Randomize(rand,nb);}

	/**
	* Assignment operator.
	* @param src              Vector used for the assignment.
	*/
	inline RVector& operator=(const RVector& src) {RNumContainer<double,false>::operator=(src); return(*this);}

	/**
	 * Get the values corresponding to the vector.
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
	inline double operator[](size_t i) const {return(RNumContainer<double,false>::operator[](i));}

	/**
	* Return the value at position i. The first value is at position 0.
	* @param i               Index.
	*/
	inline double& operator[](size_t i) {return(RNumContainer<double,false>::operator[](i));}

	/**
	* Get the number of values in the list.
	* @return size_t
	*/
	size_t GetNb(void) const {return(NbInt);}

	/**
	* Add a vector to the current one.
	* @param vector          Vector to add.
	*/
	RVector& operator+=(const RVector& vector);

	/**
	* Subtract a vector from the current one.
	* @param vector          Vector to subtract.
	*/
  	RVector& operator-=(const RVector& vector);

	/**
	* Multiply a vector with a given number.
	* @param arg             Number.
	*/
	RVector& operator*=(const double arg);

	/**
	* Divide a vector with a given number.
	* @param arg             Number.
	*/
	RVector& operator/=(const double arg);

	/**
	 * Print the content of the vector.
	 * @tparam S             Stream class that implements the << operator.
	 * @param stream         Stream.
	 * @param name           Name of the vector.
	 * @param format         Format used to print the number. By default, it is "%E".
	 * @param colsize        Size of a column. By default, it is 12.
	 */
	template<class S> void Print(S& stream,const RString& name,const char* format="%E",size_t colsize=12) const
	{
		stream<<name<<"=[";
		for(size_t i=0;i<NbInt;i++)
		{
				RString Str(RString::Number((*this)[i],format));
				Str.SetLen(colsize," ");
				if(i!=NbInt-1)
					Str+=" ";
				stream<<Str;
		}
		stream<<"]"<<std::endl;
	}

	/**
	 * Destruct the vector.
	 */
	virtual ~RVector(void);

	friend class RMatrix;
	friend class RLowerTriangularMatrix;
};



//------------------------------------------------------------------------------
/**
* Addition of two vectors.
* @param arg1                Vector.
* @param arg2                Vector.
*/
RVector operator+(const RVector& arg1,const RVector& arg2);


//------------------------------------------------------------------------------
/**
* Subtraction of two vectors.
* @param arg1                Vector.
* @param arg2                Vector.
*/
RVector operator-(const RVector& arg1,const RVector& arg2);


//------------------------------------------------------------------------------
/**
* Multiplication of a vector and a number.
* @param arg1                Vector.
* @param arg2                Vector.
*/
RVector operator*(const RVector& arg1,const double arg2);


//------------------------------------------------------------------------------
/**
* Multiplication of a vector and a number.
* @param arg1                Number.
* @param arg2                Vector.
*/
RVector operator*(const double arg1,const RVector& arg2);


//------------------------------------------------------------------------------
/**
* Division of a vector and a number.
* @param arg1                Vector.
* @param arg2                Vector.
*/
RVector operator/(const RVector& arg1,const double arg2);


//------------------------------------------------------------------------------
/**
* Division of a vector and a number.
* @param arg1                Number.
* @param arg2                Vector.
*/
RVector operator/(const double arg1,const RVector& arg2);


//------------------------------------------------------------------------------
/**
* Multiplication of two vectors, i.e. compute the scalar product.
* @param arg1                Vector.
* @param arg2                Vector.
* @return the scalar product.
*/
double operator*(const RVector& arg1,const RVector& arg2);


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
