/*

	R Project Library

	RStd.h

	Rainbow Standard Library - Header.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef RStd_H
#define RStd_H


//------------------------------------------------------------------------------
// include files for Standard C/C++
#include <new>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;


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
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" ("<<__PRETTY_FUNCTION__                                 \
				  <<"): assertion failed: "<<#expr<<endl;                     \
		}

		#define RAssertNotReached()                                           \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" ("<<__PRETTY_FUNCTION__                                 \
				  <<"): should not be reached"<<endl;                         \
		}

		#define RAssertMsg(msg)                                               \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" ("<<__PRETTY_FUNCTION__                                 \
				  <<"): "<<#msg<<endl;                                        \
		}

	#else 	// !__GNUC__

		#define RAssert(expr)                                                 \
		if(!(expr))                                                           \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" : assertion failed: "<<#expr<<endl;                     \
		}

		#define RAssertNotReached()                                           \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" : should not be reached"<<endl;                         \
		}

		#define RAssertMsg(msg)                                               \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<": "<<#msg<<endl;                                         \
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
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" ("<<__PRETTY_FUNCTION__                                 \
				  <<"): check failed: "<<#expr<<endl;                         \
			return;                                                           \
		}

		#define RReturnValIfFail(expr,val)                                    \
		if(!(expr))                                                           \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" ("<<__PRETTY_FUNCTION__                                 \
				  <<"): check failed: "<<#expr<<endl;                         \
			return(val);                                                      \
		}

	#else 	// !__GNUC__

		#define RReturnIfFail(expr)                                           \
		if(!(expr))                                                           \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				<<": line "<<__LINE__                                         \
				<<" : check failed: "<<#expr<<endl;                           \
			return;                                                           \
		}

		#define RReturnValIfFail(expr,val)                                    \
		if(!(expr))                                                           \
		{                                                                     \
			cerr  <<"file "<<__FILE__                                         \
				  <<": line "<<__LINE__                                       \
				  <<" : check failed: "<<#expr<<endl;                         \
			return(val);                                                      \
		}


	#endif 	// !__GNUC__

#endif 		// !__RDISABLECHECKS__

/**
* \namespace RStd
* \brief Standard Classes.
*
* This namespace declares some generic classes.
*/


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* Basic type for representing an identificator.
*/
typedef unsigned int tId;


//------------------------------------------------------------------------------
/**
* NullId represents a undefined identificator reference.
*/
const tId NullId=0xFFFFFFFF;


//------------------------------------------------------------------------------
/**
* This function provides a way to handle temporary objects needed when using
* standard operators with non standard classes.
* @author Pascal Francq
* @param C                  The type of class.
* @param Max                Maximal size of the array (neested operators).
*/
template<class C,unsigned long Max>
	inline C* GetTemporaryObject(void)
{
	static C tab[Max];
	static long act=0;

	if(act==Max) act=0;
	return(&tab[act++]);
}


//------------------------------------------------------------------------------
/**
* The RException class provides a basic representation for an exception.
* @author Pascal Francq
* @short Basic Exception.
*/
class RException
{
	/**
	* Message holding some information.
	*/
	char* Msg;

public:

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	RException(const char* str) throw(bad_alloc);

	/**
	* Get the content of the exception.
	* @returns Pointer to a C String.
	*/
	const char* GetMsg(void) const {return(Msg);}

	/**
	* Destructor.
	*/
	virtual ~RException(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
