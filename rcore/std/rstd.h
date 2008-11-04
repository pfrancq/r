/*

	R Project Library

	RStd.h

	R Standard Library - Header.

	Copyright 1999-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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


//------------------------------------------------------------------------------
// Assertion macros
#ifdef __RDISABLEASSERT__

	#define RAssert(expr)
	#define RAssertNotReached()

#else 		// !__RDISABLEASSERT__

	#ifdef __GNUC__

		#define RAssert(expr)                                                 \
		if(!(expr))                                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" ("<<__PRETTY_FUNCTION__                           \
						<<"): assertion failed: "<<#expr<<std::endl;          \
		}

		#define RAssertNotReached()                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" ("<<__PRETTY_FUNCTION__                           \
						<<"): should not be reached"<<std::endl;              \
		}

		#define RAssertMsg(msg)                                               \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" ("<<__PRETTY_FUNCTION__                           \
						<<"): "<<#msg<<std::endl;                             \
		}

	#else 	// !__GNUC__

		#define RAssert(expr)                                                 \
		if(!(expr))                                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" : assertion failed: "<<#expr<<std::endl;          \
		}

		#define RAssertNotReached()                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" : should not be reached"<<std::endl;              \
		}

		#define RAssertMsg(msg)                                               \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<": "<<#msg<<std::endl;                              \
		}

	#endif 	// __GNUC__

#endif 		// !__RDISABLEASSERT__


//------------------------------------------------------------------------------
// Check macros
#ifdef __RDISABLECHECKS__

	#define RReturnIfFail(expr)
	#define RReturnValIfFail(expr,val)

#else 		// !__RDISABLECHECKS__

	#ifdef __GNUC__

		#define RReturnIfFail(expr)                                           \
		if(!(expr))                                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" ("<<__PRETTY_FUNCTION__                           \
						<<"): check failed: "<<#expr<<std::endl;              \
			return;                                                           \
		}

		#define RReturnValIfFail(expr,val)                                    \
		if(!(expr))                                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" ("<<__PRETTY_FUNCTION__                           \
						<<"): check failed: "<<#expr<<std::endl;              \
			return(val);                                                      \
		}

	#else 	// !__GNUC__

		#define RReturnIfFail(expr)                                           \
		if(!(expr))                                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" : check failed: "<<#expr<<std::endl;              \
			return;                                                           \
		}

		#define RReturnValIfFail(expr,val)                                    \
		if(!(expr))                                                           \
		{                                                                     \
			std::cerr	<<"file "<<__FILE__                                   \
						<<": line "<<__LINE__                                 \
						<<" : check failed: "<<#expr<<std::endl;              \
			return(val);                                                      \
		}


	#endif 	// !__GNUC__

#endif 		// !__RDISABLECHECKS__


//------------------------------------------------------------------------------
// To-Do Macros
#ifdef __RDISABLECHECKS__

	#define RToImplement()
	#define RToDo(expr)

#else 		// !__RDISABLECHECKS__

	#ifdef __GNUC__

		#define RToImplement()                                                \
		std::cout	<<"file "<<__FILE__                                       \
					<<": line "<<__LINE__                                     \
					<<" ("<<__PRETTY_FUNCTION__                               \
					<<"): to implement"<<std::endl;                           \

		#define RToDo(expr)                                                   \
		std::cout	<<"file "<<__FILE__                                       \
					<<": line "<<__LINE__                                     \
					<<" ("<<__PRETTY_FUNCTION__                               \
					<<"): to do: "<<#expr<<std::endl;                         \

	#else 	// !__GNUC__

		#define RToImplement()                                                \
		std::cout	<<"file "<<__FILE__                                       \
					<<": line "<<__LINE__                                     \
					<<" : to implement"<<std::endl;                           \

		#define RToDo(expr)                                                   \
		std::cout	<<"file "<<__FILE__                                       \
					<<": line "<<__LINE__                                     \
					<<" : to do: "<<#expr<<std::endl;                         \

	#endif 	// !__GNUC__

#endif 		// !__RDISABLECHECKS__


/**
* \namespace R
* \brief Standard Classes.
*
* This namespace declares some generic classes.
*/


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The cNoRef constant represented an undefined identifier reference.
*/
const size_t cNoRef=(size_t)(-1);


//------------------------------------------------------------------------------
/**
* The tCoord type represents a coordinate.
*/
typedef double tCoord;


//------------------------------------------------------------------------------
/**
 * Inline function that returns the absolute value of a coordinate.
 */
inline tCoord Abs(tCoord x) {return(x<0.0?-x:x);}


//------------------------------------------------------------------------------
/**
 * Inline function to compare two identifiers. Useful for R::RContainer.
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
 * Inline function to compare two pointers. Useful for R::RContainer.
 * @param ptr1               First pointer.
 * @param ptr2               Second pointer.
 * @return
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
* The MaxCoord constant represent the maximal positive value that a
* coordinate can have. It is used to indicate an invalid coordinate.
*/
const tCoord MaxCoord=3.40282347e+38;


//------------------------------------------------------------------------------
/**
* Represent the different orientation of a given polygon.
*/
enum ROrientation
{
	Normal,
	NormalX,
	NormalY,
	NormalYX,
	Rota90,
	Rota90X,
	Rota90Y,
	Rota90YX
};


//------------------------------------------------------------------------------
/**
* Represent different directions.
*/
enum RDirection
{
	NoDirection,
	Left,
	Right,
	Down,
	Up,
	Behind,
	Beyond,
	Origin,
	Destination,
	Between
};


//------------------------------------------------------------------------------
/**
* This variable represents a tolerance for calculations.
*/
extern double Epsi;


//------------------------------------------------------------------------------
/**
* The RException class provides a basic representation for an exception.
* @author Pascal Francq
* @short Basic Exception.
*/
class RException
{
protected:

	/**
	* Message holding some information.
	*/
	char Msg[1024];

public:

	/**
	* Construct an exception.
	*/
	RException(void) throw();

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	RException(const char* str) throw();

	/**
	* Get the content of the exception.
	* @returns Pointer to a C String.
	*/
	const char* GetMsg(void) const {return(Msg);}

	/**
	* Set the error message.
	* @param str                      Message of the error.
	*/
	void SetMsg(const char* str);

	/**
	* Destructor.
	*/
	virtual ~RException(void) throw();
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
// Macro to create a new exception directly derived from RException
#define NEWREXCEPTION(name)                                                   \
class name : public R::RException                                             \
{                                                                             \
public:                                                                       \
	name(void) throw() : R::RException() {}                                   \
	name(const char* str) throw() : R::RException(str) {}                     \
}


//------------------------------------------------------------------------------
#endif
