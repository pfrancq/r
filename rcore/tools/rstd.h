/*

	R Project Library

	RStd.h

	R Standard Library - Header.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RStdH
#define RStdH


//------------------------------------------------------------------------------
// include files for Standard C/C++
#include <new>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <rportsmacro.h>
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <limits.h>
#include <limits>


//------------------------------------------------------------------------------
// Define __PRETTY_FUNCTION__ if necessary
#ifndef __GNUC__
	#define __PRETTY_FUNCTION__ __FUNCTION__
#endif


//------------------------------------------------------------------------------
// Assertion macros
#ifdef __GNUC__
	#define mInfo \
		"file "<<__FILE__<<": line "<<__LINE__<<" ("<<__PRETTY_FUNCTION__<<")"
#else 	// !__GNUC__
	#define mInfo \
		"file "<<__FILE__<<": line "<<__LINE__
#endif 	// __GNUC__


#ifdef __RDISABLEASSERT__

	#define mAssert(expr)
	#define mAssert2(expr)
	#define mAssertNotReached()

#else 		// !__RDISABLEASSERT__

	#define mAssert(expr)                                             \
	if(!(expr))                                                       \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": assertion failed: "<<#expr<<std::endl;           \
	}

	#define mAssert2(expr,msg)                                        \
	if(!(expr))                                                       \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": assertion failed: "<<#expr                       \
					<<" ("<<msg<<")"<<std::endl;                          \
	}

	#define mAssertNotReached()                                       \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": should not be reached"<<std::endl;               \
	}

	#define mAssertMsg(msg)                                           \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": "<<#msg<<std::endl;                              \
	}

#endif 		// !__RDISABLEASSERT__


//------------------------------------------------------------------------------
// Check macros
#ifdef __RDISABLECHECKS__

	#define mReturnIfFail(expr)
	#define mReturnIfFail2(expr,msg)
	#define mReturnValIfFail(expr,val)
	#define mReturnValIfFail3(expr,val,msg)

#else 		// !__RDISABLECHECKS__

	#define mReturnIfFail(expr)                                       \
	if(!(expr))                                                       \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": check failed: "<<#expr<<std::endl;               \
		return;                                                        \
	}

	#define mReturnIfFail2(expr,msg)                                  \
	if(!(expr))                                                       \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": check failed: "<<#expr                           \
					<<" ("<<msg<<")"<<std::endl;                          \
		return;                                                        \
	}

	#define mReturnValIfFail(expr,val)                                \
	if(!(expr))                                                       \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": check failed: "<<#expr<<std::endl;               \
		return(val);                                                   \
	}

	#define mReturnValIfFail3(expr,val,msg)                           \
	if(!(expr))                                                       \
	{                                                                 \
		std::cerr<<mInfo                                               \
					<<": check failed: "<<#expr                           \
					<<" ("<<msg<<")"<<std::endl;                          \
		return(val);                                                   \
	}

#endif 		// !__RDISABLECHECKS__


//------------------------------------------------------------------------------
// To-Do Macros
#ifdef __RDISABLECHECKS__

	#define mToImplement()
	#define mToDo(expr)

#else 		// !__RDISABLECHECKS__

	#define mToImplement()                                             \
	std::cout<<mInfo                                                   \
				<<": to implement"<<std::endl;                            \

	#define mToDo(expr)                                                \
	std::cout<<mInfo                                                   \
				<<") to do: "<<#expr<<std::endl;                          \

#endif 		// !__RDISABLECHECKS__


//-----------------------------------------------------------------------------
// MAX SIZE of size_t
#ifndef __APPLE__
	#ifndef WIN32
		#if __WORDSIZE == 64
			#define SIZE_MAX		(18446744073709551615UL)
		#else
			#define SIZE_MAX		(4294967295U)
		#endif
	#endif
#endif



//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The cNoRef constant represented an undefined identifier reference.
*/
const size_t cNoRef=std::numeric_limits<size_t>::max();


//------------------------------------------------------------------------------
/**
* The tCoord type represents a coordinate.
*/
typedef double tCoord;


//------------------------------------------------------------------------------
/**
 * The function returns the absolute value of a coordinate.
 */
inline tCoord Abs(tCoord x) {return(x<0.0?-x:x);}


//------------------------------------------------------------------------------
/**
 * The function compares two identifiers. Useful for R::RContainer.
 * @param id1                First identifier.
 * @param id2                Second identifier.
 * @return
 */
inline int CompareIds(size_t id1,size_t id2)
{
	if(id1>id2) return (1);
	else if(id1<id2) return(-1);
	return(0);
}


//------------------------------------------------------------------------------
/**
 * The function compares two pointers. Useful for R::RContainer.
 * @tparam C                 Class to compare.
 * @param ptr1               First pointer.
 * @param ptr2               Second pointer.
 */
template<class C>
	inline int ComparePtrs(C* ptr1,C* ptr2)
{
	if(ptr1>ptr2) return (1);
	else if(ptr1<ptr2) return(-1);
	return(0);
}


//------------------------------------------------------------------------------
/**
* The cNoCoord constant represent an invalid coordinate. It practice, it is the
* maximal positive value that a coordinate can have.
*/
const tCoord cNoCoord=std::numeric_limits<tCoord>::max();


//------------------------------------------------------------------------------
/**
 * The cMaxCoord represent the maximal acceptable value for a coordinate.
 */
const tCoord cMaxCoord=std::numeric_limits<tCoord>::max();


//------------------------------------------------------------------------------
/**
 * The cMinCoord represent the minimal acceptable value for a coordinate.
 */
const tCoord cMinCoord=std::numeric_limits<tCoord>::min();


//------------------------------------------------------------------------------
/**
* Represent the different orientation of a given polygon.
*/
enum tOrientation
{
	oNormal,                 /** Normal. */
	oNormalX,                /** Horizontal mirror. */
	oNormalY,                /** Vertical mirror. */
	oNormalYX,               /** Horizontal and vertical mirrors. */
	oRota90,                 /** 90° rotation. */
	oRota90X,                /** 90° rotation with a horizontal mirror. */
	oRota90Y,                /** 90° rotation with a vertical mirror. */
	oRota90YX                /** 90° rotation with a horizontal and vertical
	                             mirror. */
};


//------------------------------------------------------------------------------
/**
* Represent different directions.
*/
enum tDirection
{
	dNoDirection,            /** No direction. */
	dLeft,                   /** Left. */
	dRight,                  /** Right. */
	dDown,                   /** Down. */
	dUp,                     /** Up. */
	dBehind,                 /** Behind. */
	dBeyond,                 /** Beyond.*/
	dOrigin,                 /** Origin.*/
	dDestination,            /** Specific destination.*/
	dBetween,                /** Between two positions.*/
	dLeftDown,               /** Left-Down.*/
	dLeftUp,                 /** Left-Up.*/
	dRightDown,              /** Right-Down.*/
	dRightUp	                /** Right-Up.*/
};


//------------------------------------------------------------------------------
/**
* This variable represents a tolerance for calculations. if a calculation (for
* example the difference between two coordinates) is less than cEpsi, it is
* considered as zero.
*/
const double cEpsi=0.000001;


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
