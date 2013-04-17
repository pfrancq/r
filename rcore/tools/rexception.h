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
#ifndef RExceptionH
#define RExceptionH


//------------------------------------------------------------------------------
// include files for R
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RException class provides a basic representation for an exception.
* @author Pascal Francq
* @short Basic Exception.
*/
class RException
{
    /*
     * Null member for internal purposes.
     */
    void Null(void);

protected:

	/**
	* Message holding some information.
	*/
	RString Msg;

public:

	/**
	* Construct an exception.
	*/
	RException(void);

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	RException(const RString& str);

	/**
	* Construct an exception with the message "func [where]: str". A typical use is:
	* @code
	* if(!ptr)
	* 	throw RException(__PRETTY_FUNCTION__,__LINE__,"ptr cannot be a null pointer");
	* @endcode
	* @see The mThrowRException marco.
	* @param func                     Function producing the error.
	* @param where                    Line position of the error.
	* @param str                      Message of the error.
	*/
	RException(const RString& func,long where,const RString& str);

	/**
	* Get the content of the exception.
	* @returns Pointer to a C String.
	*/
	RString GetMsg(void) const {return(Msg);}

protected:

	/**
	* Set the error message.
	* @param str                      Message of the error.
	*/
	void SetMsg(const RString& str);

	/**
	* Set the error message.
	* @param func                     Function producing the error.
	* @param where                    Line position of the error.
	* @param str                      Message of the error.
	*/
	void SetMsg(const RString& func,long where,const RString& str);

public:

	/**
	* Destructor.
	*/
	virtual ~RException(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
// Macro to generate a RException
#define mThrowRException(msg) throw RException(__PRETTY_FUNCTION__,__LINE__,msg)


//------------------------------------------------------------------------------
// Macro to create a new exception directly derived from RException
#define mNewRException(name)                                                  \
class name : public R::RException                                             \
{                                                                             \
public:                                                                       \
	name(void) throw() : R::RException() {}                                    \
	name(const RString& str) throw() : R::RException(str) {}                   \
}


//------------------------------------------------------------------------------
#endif
