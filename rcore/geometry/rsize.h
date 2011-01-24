/*

	R Project Library

	RSize.h

	Size - Header

	Copyright 2006-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your osizeion) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANHeight WARRANTHeight; without even the implied warranty of
	MERCHANTABILITHeight or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	Heightou should have received a copy of the GNU Library General Public
	License along with this library, as a file COPHeightING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/


//------------------------------------------------------------------------------
#ifndef RSize_H
#define RSize_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a size.
* @author Pascal Francq
* @short Size.
*/
class RSize
{
	/**
	* This is the width.
	*/
	tCoord Width;

	/**
	* This is the height.
	*/
	tCoord Height;

public:

	/**
	* Construct a null size.
	*/
	RSize(void);

	/**
	* Construct a given size.
	*/
	RSize(const tCoord w,const tCoord h);

	/**
	* Copy constructor.
	* @param size            Size.
	*/
	RSize(const RSize& size);

	/**
	* The equal operator.
	*/
	inline bool operator==(const RSize& size) const {return((Width==size.Width)&&(Height==size.Height));}

	/**
	* The non-equal operator.
	*/
	inline bool operator!=(const RSize& size) const {return((Width!=size.Width)||(Height!=size.Height));}

	/**
	* Make a translation of the point.
	* @param size             The point representing the vector used.
	*/
	RSize& operator+=(const RSize& size) {Width+=size.Width;Height+=size.Height;return(*this);}

	/**
	* Make a translation of the point.
	* @param size             The point representing the vector used.
	*/
	RSize& operator-=(const RSize& size) {Width-=size.Width;Height-=size.Height;return(*this);}

	/**
	* Compare two points and return 0 if there are at the same position. This function
	* is used with the class RContainer.
	* @param size             Point used for the comparison.
	*/
	inline int Compare(const RSize& size) const { return((*this)!=size); }

	/**
	* Assignment operator.
	*/
	inline RSize& operator=(const RSize& size) {Width=size.Width;Height=size.Height;return(*this);}

	/**
	* Get the width.
	*/
	tCoord GetWidth(void) const {return(Width);}

	/**
	* Get the height.
	*/
	tCoord GetHeight(void) const {return(Height);}

	/**
	* Test if the point is a valid one.
	*/
	bool IsValid(void) const { return((Width!=cNoCoord)&&(Height!=cNoCoord));}

	/**
	* Set the size.
	* @param w	            Width.
	* @param h              Height.
	*/
	void Set(const tCoord w,const tCoord h) {Width=w; Height=h;}

	/**
	 * Represent an invalid point.
	 */
	static RSize Null;
};


//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* Add two sizes.
*/
inline RSize operator+(const RSize& arg1,const RSize& arg2) {return(RSize(arg1)+=arg2);}

//------------------------------------------------------------------------------
/**
* Substract two sizes.
*/
inline RSize operator-(const RSize& arg1,const RSize& arg2) {return(RSize(arg1)-=arg2);}


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
